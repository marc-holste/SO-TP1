// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/view.h"
#include "include/shared_mem.h"
#include "include/sem_manager.h"

#define MAX_NUM_LEN 10

#define PROJ_ID 551699

void verror (const char *err) {
    fprintf(stderr, "[view] %s", err);
}

int main (int argc, char *argv[]) {
    //Here comes view
    int inNum = -1;
    int inId = -1;
    int printed = 0;
    size_t auxP = 0;

    if (isatty(fileno(stdin))) { //caso consola
        if (argc != 3){
            verror((argc > 3)?"Too many arguments\n":"Too few arguments\n");
            return -1;
        } else {
            inNum = atoi(argv[1]);
            inId = atoi(argv[2]);
        }
    } else { //caso pipe
        if (argc != 1){
            verror ("Error in pipe, missing count\n");
            return -1;
        } else {
            scanf("%d %d", &inNum, &inId);
        }
    }
    if (inNum > -1 && inId > -1){
        char* shmp = attach_block(inId);
        if ((long long)shmp == -1){
            perror("[view] attach_block");
            return EXIT_FAILURE;
        }

        sem_t* semaphore_count = sem_open(COUNT_SEM_NAME, 0);       // Semaphore should be previously created by master
        if(semaphore_count == SEM_FAILED){
            perror("[view] couldn't open count semaphore");
            return EXIT_FAILURE;
        }
        
        while(printed < inNum){
            if(sem_wait(semaphore_count) == -1){
                perror("[view] sem_wait");
                return EXIT_FAILURE;
            }
            auxP += printf("%s", shmp+auxP) + 2;             // Adds 2 bc of '\0' and '\n'
            printed++;
        }

        // Deletes shared memory and semaphore used, bc by now master already finished processing all files.
        if(detach_block(shmp) == -1){
            perror("[view] detach_block");
            return EXIT_FAILURE;
        }
        if(delete_block(inId) == -1){
            perror("[view] delete_block");
            return EXIT_FAILURE;
        }
        if(sem_close(semaphore_count) == -1){
            perror("[view] sem_close");
            return EXIT_FAILURE;
        }
        if(sem_unlink(COUNT_SEM_NAME) == -1){
            perror("[view] sem_unlink");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
