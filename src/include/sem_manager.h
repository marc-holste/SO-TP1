#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SEM_NAME "shm_semaphore"
#define SEM_MODE 0666

sem_t* create_semaphore(const char* sem_name, unsigned int value, mode_t mode);
sem_t* get_semaphore(const char* sem_name);
int wait_semaphore(sem_t* semaphore);
int signal_semaphore(sem_t* semaphore);
int get_semaphore_value(sem_t* semaphore, int* pvalue);
int close_semaphore(sem_t* semaphore);
int delete_semaphore(const char* sem_name);