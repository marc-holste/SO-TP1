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

    if (isatty(fileno(stdin))) { //caso consola
        if (argc != 3){
            verror ("Missing file count parameter\n");
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
        sem_t* semaphore = get_semaphore(SEM_NAME, 0);

        if ((long)shmp == -1){
            verror ("Shared memory is not accesible\n");
            return -1;
        }
        while(printed < inNum){
            wait_semaphore(semaphore);
            shmp += printf("%s", shmp) + 2;
            signal_semaphore(semaphore);
            printed++;
        }
        detach_block(shmp);
        delete_block(inId);
        close_semaphore(semaphore);
        delete_semaphore(SEM_NAME);
    }
    return 0;
}
