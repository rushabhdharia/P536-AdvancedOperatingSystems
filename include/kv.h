#ifndef _KV_H_
#define _KV_H_

char* kv_get(char* key);
int kv_set(char* key, char* value);
bool kv_delete(char* key);
void kv_reset();
int kv_init();

int get_cache_info(char *kind);
char** most_popular_keys(int k); 

#endif
