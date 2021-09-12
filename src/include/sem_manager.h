#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define COUNT_SEM_NAME "shm_count_semaphore"
#define SEM_MODE 0666