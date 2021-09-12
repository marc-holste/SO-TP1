#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>

key_t generate_block_key(const char* pathname, int proj_id);
int create_block(const char *pathname, int proj_id, size_t size);
void* attach_block(int block_id);
void* attach_custom_block(int block_id, const void* block_addr, int block_flags);
int detach_block(const void* block_addr);
int delete_block(int block_id);
int get_block(key_t key, size_t size);