#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include "sem_manager.h"
#include "shared_mem.h"

#define PIPE_PATH_MAX 20

#define OUTPUT_NAME "out.txt"

void connectNamedPipe(int *fd, char path[PIPE_PATH_MAX], int flag) {
    if((*fd = open(path, flag)) == -1){
        perror("Error in open named pipe\n");
        exit(EXIT_FAILURE);
    }
    return;
}