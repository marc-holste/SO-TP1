#include <shared_mem.h>

// Number used for key generation
static int proj_id = 551699;

/*
void set_block_seed(){
    srand(time(NULL));
    proj_id = rand();       // RAND_MAX = INT_MAX
}
*/

// Returns id of created block, or -1 in case of failure
int create_block(const char *pathname, size_t size){
    key_t key;
    if((key = ftok(pathname, proj_id)) == -1){
        // Error message goes here
        return -1;
    }
    return get_block(key, size);
}

// Returns id of block, or -1 in case of failure
static int get_block(key_t key, size_t size){
    return shmget(key, size, IPC_CREAT | 0666);
}

// Attaches a memory segment to the id sent, and returns it if the operation was succesful.
// In case of error, a (void)* -1 is returned
void* attach_block(int block_id){
    return attach_custom_block(create_block, NULL, 0);
}

// Attaches a memory segment to the id sent, and returns it if the operation was succesful.
// In case of error, a (void*)-1 is returned
void* attach_custom_block(int block_id, const void* block_addr, int block_flags){
    return shmat(block_id, block_addr, block_flags);
}

// Detaches memory block. Returns 0 in case of success, and -1 if an error ocurred
int detach_block(const void* block_addr){
    return shmdt(block_addr);
}

// Returns 0 if the block was destroyed and -1 if an error occurred
int delete_block(int block_id){
    return shmctl(block_id, IPC_RMID, NULL);
}