#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int create_block(size_t size, int permissions);
int get_block(int permissions);
void* attach_block(int block_id);
int detach_block(const void* block_addr);
int delete_block(int block_id);