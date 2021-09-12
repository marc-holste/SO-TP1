#include "include/masterTimer.h"

void timer (const char *err) {
    fprintf(stderr, "[slave] %s", err);
}

//expected arguments:
//  argv[0] = ./bin/timer
//  argv[1] = time_secs
int main(int argc, char *argv[]) {
    if(argc != 2) {
        serror("Not enough arguments");
        exit(EXIT_FAILURE);
    }
    int time_secs = atoi(argv[1]);

    sleep(time_secs);

}