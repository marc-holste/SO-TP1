#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int create_block(const char *pathname, size_t size);
void* attach_block(int block_id);
void* attach_custom_block(int block_id, const void* block_addr, int block_flags);
int detach_block(const void* block_addr);
int delete_block(int block_id);