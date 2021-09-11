#include "include/sem_manager.h"

sem_t* create_semaphore(const char* sem_name, unsigned int value, mode_t mode){
    sem_unlink(sem_name);
    return sem_open(sem_name, O_CREAT, mode, value);
}

sem_t* get_semaphore(const char* sem_name, unsigned int value){
    return sem_open(sem_name, value);
}

int get_semaphore_value(sem_t* semaphore, int* pvalue){
    return sem_getvalue(semaphore, pvalue);
}

int wait_semaphore(sem_t* semaphore){
    return sem_wait(semaphore);
}

int signal_semaphore(sem_t* semaphore){
    return sem_post(semaphore);
}

int close_semaphore(sem_t* semaphore){
    return sem_close(semaphore);
}

int delete_semaphore(const char* sem_name){
    return sem_unlink(sem_name);
}