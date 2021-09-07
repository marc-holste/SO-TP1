#include "include/view.h"
#include "include/shared_mem.h"

#define MAX_NUM_LEN 10

void verror (const char *err) {
    fprintf(stderr, "[view] %s", err);
}

int main (int argc, char *argv[]) {
    //Here comes view
    int in = 0;
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
    }
    return 0;
}
