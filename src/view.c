// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/view.h"
#include "include/shared_mem.h"
#include "include/sem_manager.h"

#define MAX_NUM_LEN 10
#define PERMISSION_FLAGS    0666               // Could also or all predefined permission flags, but less practical

void verror (const char *err) {
    fprintf(stderr, "[view] %s", err);
}

int main (int argc, char *argv[]) {
    //Here comes view
    int inNum = -1;

    if (isatty(fileno(stdin))) { //caso consola
        if (argc != 2){
            verror((argc > 2)?"Too many arguments\n":"Missing file count\n");
            return -1;
        } else {
            inNum = atoi(argv[1]);
        }
    } else { //caso pipe
        if (argc != 1){
            verror ("Error in pipe, missing count\n");
            return -1;
        } else {
            scanf("%10d", &inNum);          // An integer can´t have more than 10 digits
        }                                   // Used to prevent possible crashes with huge input of data
    }
    if (inNum > -1){
        int printed = 0;
        char* shmp;

        int shmid;
        if((shmid = get_block(PERMISSION_FLAGS)) == -1){
            perror("[view] get_block");
            return EXIT_FAILURE;
        }

        char* shm_base = attach_block(shmid);
        if ((long long)shm_base == -1){
            perror("[view] attach_block");
            return EXIT_FAILURE;
        }
        shmp = shm_base;

        sem_t* semaphore_count = get_semaphore(COUNT_SEM_NAME);       // Semaphore should be previously created by master
        if(semaphore_count == SEM_FAILED){
            perror("[view] couldn't open count semaphore");
            return EXIT_FAILURE;
        }
        
        while(printed < inNum){
            if(semaphore_wait(semaphore_count) == -1){
                perror("[view] semaphore_wait");
                return EXIT_FAILURE;
            }
            shmp += printf("%s", shmp) + 2;             // Adds 2 bc of '\0' and '\n'
            printed++;
        }

        // Deletes shared memory and semaphore used, bc by now master already finished processing all files.
        if(detach_block(shm_base) == -1){
            perror("[view] detach_block");
            return EXIT_FAILURE;
        }
        if(delete_block(shmid) == -1){
            perror("[view] delete_block");
            return EXIT_FAILURE;
        }
        if(close_semaphore(semaphore_count) == -1){
            perror("[view] close_semaphore");
            return EXIT_FAILURE;
        }
        if(delete_semaphore(COUNT_SEM_NAME) == -1){
            perror("[view] delete_semaphore");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
