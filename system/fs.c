#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#ifdef FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}


int fs_open(char *filename, int flags)
{	
	int i, j, success;
	
	if(flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR)
	{
		printf("Incorrect flag\n");
		return SYSERR;
	}

	for(i=0; i<fsd.root_dir.numentries; i++)
	{
		if(strncmp(fsd.root_dir.entry[i].name, filename, FILENAMELEN)== 0)
			break;
	}

	if(i >= fsd.root_dir.numentries)
	{
		printf("File not found\n");
		return SYSERR;
	}

	for(j=0; j<NUM_FD; j++)
	{
		if(oft[next_open_fd].state == FSTATE_CLOSED)
		{
			break;
		}
		next_open_fd++;
	}	
	if(next_open_fd >= NUM_FD)
		next_open_fd = 0;
	
	if(oft[next_open_fd].state == FSTATE_OPEN)
	{
		if(next_open_fd == 0)
			printf("Cannot open more files\n");
		else
			printf("The file is already open\n");
		return SYSERR;
	}

	oft[next_open_fd].state = FSTATE_OPEN;
	oft[next_open_fd].fileptr = 0;
	oft[next_open_fd].de = &fsd.root_dir.entry[i];
	
	success = fs_get_inode_by_num(dev0, fsd.root_dir.entry[i].inode_num, &oft[next_open_fd].in);
	if(success == SYSERR)
	{
		printf("Could not open file as could not get inode\n");
		return SYSERR;
	}

 	return next_open_fd;

}

int fs_close(int fd) {
	if(fd<0 || fd> NUM_FD)
	{
		printf("Invalid fd");
		return SYSERR;
	}
	
	if(oft[fd].state == FSTATE_CLOSED)
	{
		printf("File is not open");
		return SYSERR;
	}
	else if(oft[fd].state == FSTATE_OPEN)
	{
		oft[fd].state = FSTATE_CLOSED;
		oft[fd].fileptr = 0;
		return OK;
	}
	
  return SYSERR;
}

int fs_create(char *filename, int mode) {

	int i, success;
	struct inode *in;

	if(strlen(filename)>=FILENAMELEN)
	{
		printf("Filename exceeds max allowed length");
		return SYSERR;
	}

	if(mode != O_CREAT)
	{
		printf("Incorrect mode\n");
		return SYSERR;
	}
	
	
	if(fsd.root_dir.numentries != 0)
	{
	for(i=0; i<fsd.root_dir.numentries ;i++)
	{
		if(strncmp(fsd.root_dir.entry[i].name, filename, FILENAMELEN) == 0)
		{
			printf("File already exists\n");
			return SYSERR;
		} 
	}
	}

	if(fsd.inodes_used >= fsd.ninodes)
	{
		printf("Could not create file as all inodes are used\n");
		return SYSERR;
	}

	in =(void *) getmem(sizeof(struct inode));
	in->id = fsd.inodes_used++;
	in->type = INODE_TYPE_FILE;	
	in->size = 0;
	in->nlink = 1;
	in->device = dev0;
	
	success = fs_put_inode_by_num(dev0, fsd.inodes_used-1, in);
	if(success != OK)
	{
		printf("Could not create file as could not get inode\n");
		return SYSERR;
	}	

	fsd.root_dir.numentries++;
	i = fsd.root_dir.numentries-1;
	fsd.root_dir.entry[i].inode_num = fsd.inodes_used-1;
	strncpy(fsd.root_dir.entry[i].name, filename, FILENAMELEN);
	fsd.root_dir.numentries++;

	return fs_open(filename, O_RDWR);
	
}

int fs_seek(int fd, int offset) {
	
	int check;
	if(fd<0 || fd>=NUM_FD)
	{
		printf("Invalid fd");
		return SYSERR;
	}	
	
	check = oft[fd].fileptr+offset;
	if(check<0 || check>oft[fd].in.size)
	{
		printf("Invalid File pointer");
		return SYSERR;
	}

	oft[fd].fileptr += offset;
	
	return OK;
}

int fs_read(int fd, void *buf, int nbytes) 
{	
	int blockindex, blockno, offset, temp, count, success;
	char *buff = buf;
	
	if(fd<0 || fd>= NUM_FD)
	{
		printf("Invalid fd\n");
		return SYSERR;
	}	

	if(nbytes<1)
	{
		printf("Cannot read less than 1 byte\n");
		return SYSERR;
	}

	if(oft[fd].state==FSTATE_CLOSED)
	{
		printf("Could not read file as it is closed\n");
		return SYSERR;
	}
	
	blockindex=oft[fd].fileptr/dev0_blocksize;
	blockno=oft[fd].in.blocks[blockindex];
	offset=oft[fd].fileptr % dev0_blocksize;
	
	count = 0;
	temp = 0;

	while(nbytes>count)
	{
		if(dev0_blocksize - offset > nbytes)
			temp = nbytes;
		else
			temp = dev0_blocksize - offset;
		
		success = bs_bread(dev0, blockno , offset, &buff[count], temp);
		
		if(success == SYSERR)
		{
			printf("Couldn't read block");
			return SYSERR;
		}
		
		count += temp;	
		oft[fd].fileptr += temp;
		nbytes -= temp;
		blockindex += 1;
		offset = 0;
				
		if(blockno == -1)
		{
			printf("Couldn't read block\n");
			return SYSERR;
		}			
	}
	return count;	
}

int fs_write(int fd, void *buf, int nbytes) {
	int blockindex, offset, blockno, i, success, count, temp;
	char *buff = buf;
		
	if(fd<0 || fd>= NUM_FD)
	{
		printf("Invalid fd\n");
		return SYSERR;
	}

	if(nbytes < 1)
	{
		printf("Cannot write less than 1 byte\n");
		return SYSERR;
	}

	if(oft[fd].state != FSTATE_OPEN)
	{
		printf("Can't write to file as file is not open\n");
		return SYSERR;
	}

	oft[fd].in.size = oft[fd].fileptr;
	blockindex = oft[fd].fileptr/dev0_blocksize;
	offset = oft[fd].fileptr%dev0_blocksize;
	blockno = oft[fd].in.blocks[blockindex];

	if(blockno == -1)
	{
		for(i=0; i<dev0_numblocks;i++)
		{
			if(fs_getmaskbit(i)==0)
			{
				blockno = i;
				break;
			}
		}
		if(blockno == -1)
		{
			printf("No blocks are free");
			return SYSERR;
		}
		oft[fd].in.blocks[blockindex] = blockno;
	}
	
	count = 0;
	while(nbytes>0)
	{
		fs_setmaskbit(blockno);
		if(dev0_blocksize - offset > nbytes)
			temp = nbytes;
		else
			temp = dev0_blocksize - offset;
	
		success = bs_bwrite(dev0, blockno, offset, &buff[count], temp);
		if(success == SYSERR)
		{
			printf("Could not write to block");
			return SYSERR;
		}
	
		nbytes -= temp;
		count += temp;
		blockindex++;
		for(i=0; i<dev0_numblocks;i++)
		{
			if(fs_getmaskbit(i)==0)
			{
				oft[fd].in.blocks[blockindex] = i;
				break; 
			}
		}
		blockno = oft[fd].in.blocks[blockindex];					
	}
	oft[fd].in.size += count;
	oft[fd].fileptr = oft[fd].in.size;
 	return count;

}

#endif /* FS */
