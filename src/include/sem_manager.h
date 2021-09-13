#include <semaphore.h>

#define COUNT_SEM_NAME "shm_count_semaphore"
#define SEM_PERMISSIONS 0666

// typedef sem_t* sem_type;
typedef sem_t* sem_type;

sem_type create_semaphore(const char* sem_name, unsigned int value, int mode);
sem_type get_semaphore(const char* sem_name);
int get_semaphore_value(sem_type semaphore, int* pvalue);
int semaphore_wait(sem_type semaphore);
int semaphore_post(sem_type semaphore);
int close_semaphore(sem_type semaphore);
int delete_semaphore(const char* sem_name);