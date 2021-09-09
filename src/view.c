// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/view.h"
#include "include/shared_mem.h"

#define MAX_NUM_LEN 10

#define SHM_SIZE 10000000
#define AUX_BLOCK_PATH "./files"
#define PROJ_ID 551699

void verror (const char *err) {
    fprintf(stderr, "[view] %s", err);
}

int main (int argc, char *argv[]) {
    //Here comes view
    int in = 0;
    int printed = 0;
    if (isatty(fileno(stdin))) { //caso consola
        if (argc != 2){
            verror ("Missing file count parameter\n");
        } else {
            in = atoi(argv[1]);
        }
    } else { //caso pipe
        if (argc != 1){
            verror ("Error in pipe, missing count\n");
        } else {
            scanf("%d", &in);
        }
    }
    if (in > 0){

        //el resto de las cosas
        printf("%d\n", in);
        int shmid = get_block(generate_block_key(AUX_BLOCK_PATH, PROJ_ID), SHM_SIZE); //get_last_created_id();
        char* shmp = attach_block(shmid);
        while(printed < in){
            shmp += printf("%s", shmp) + 3;
            printed++;
        }
        detach_block(shmp);
        delete_block(shmid);
    }
    return 0;
}
