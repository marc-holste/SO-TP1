#include <sem_manager.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_type create_semaphore(const char* sem_name, unsigned int value, int mode){
    if(sem_unlink(sem_name) == -1 && errno == EACCES){
        return SEM_FAILED;
    }
    return sem_open(sem_name, O_CREAT, mode, value);
}

sem_type get_semaphore(const char* sem_name){
    return sem_open(sem_name, 0);
}

int get_semaphore_value(sem_type semaphore, int* pvalue){
    return sem_getvalue(semaphore, pvalue);
}

int semaphore_wait(sem_type semaphore){
    return sem_wait(semaphore);
}

int semaphore_post(sem_type semaphore){
    return sem_post(semaphore);
}

int close_semaphore(sem_type semaphore){
    return sem_close(semaphore);
}

int delete_semaphore(const char* sem_name){
    return sem_unlink(sem_name);
}
