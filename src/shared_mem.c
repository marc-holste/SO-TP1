// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shared_mem.h>

#define PATH "./files"
#define SEED 188033

static key_t generate_block_key(){
    return ftok(PATH, SEED);
}

// Returns id of created block, or -1 in case of failure
int create_block(size_t size, int permissions){
    key_t key;
    if((key = generate_block_key()) == -1){    
        return -1;                                  // On error, ftok returns -1
    }
    return shmget(key, size, IPC_CREAT | permissions);
}

// Returns id of block, or -1 in case of failure
// As block was previously created, it's not necessary to send block_size
int get_block(int permissions){
    return shmget(generate_block_key(), 0, permissions);
}

// Attaches a memory segment to the id sent, and returns it if the operation was succesful.
// In case of error, a (void)* -1 is returned
void* attach_block(int block_id){
    return shmat(block_id, NULL, 0);
}

// Detaches memory block. Returns 0 in case of success, and -1 if an error ocurred
int detach_block(const void* block_addr){
    return shmdt(block_addr);
}

// Returns 0 if the block was destroyed and -1 if an error occurred
int delete_block(int block_id){
    return shmctl(block_id, IPC_RMID, NULL);
}