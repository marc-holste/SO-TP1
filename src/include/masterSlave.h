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

#define OUTPUT_NAME "out.txt"

int slave(char *fifo_read,char *fifo_write);