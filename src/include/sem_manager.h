#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SEM_NAME "shm_semaphore"
#define SEM_MODE 0666