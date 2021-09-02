#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "slave.h"

char *outputfilename = "out.txt";

//master sends the filename to slave
void write_to_pipe(int file, char *filename) {
    FILE *stream = fdopen (file, "w");
    fprintf (stream,"%s",filename);
    fclose(stream);
    return;
}

 int main (int argc, char *argv[]) {
    //if files there are files to procces
    if(argc > 1) {  

        //creates outputfile
        FILE *outputfile = fopen(outputfilename,"w");
        fprintf(outputfile,"Output:\n");
        fclose(outputfile);

        //The amount of slaves is 5% of the amount of files to process(Example is 3)
        //int slaves_dim = argc/20 + 1;
        int slaves_dim = 3;

        //The amount of slaves working
        int slaves = 0;
        //index of files to process (Starts at 1 since argv[0] is the name of the program)
        int file = 1;
        int status;
        //while there are files to process
        while(file < argc) {
            //if all the slaves are working, master wait for one to finish
            if(slaves == slaves_dim) {
                wait(&status);
                slaves--;
            //if there are slaves available, master assigns a new job
            } else {
                //master creates a slave
                slaves++;
                pid_t pid;
                int mypipe[2];

                pipe(mypipe);
                pid = fork();
                if(pid == -1) {
                    printf("Error: creates of slave failed");
                    exit(EXIT_FAILURE);
                } 
                //slave
                if(pid == 0) {
                    close (mypipe[1]);
                    //Demo makes some files last longer than others
                    if(file % 3 == 0) {
                        sleep(2);
                    }
                    //slave reads filename form pipe and process it
                    if(read_from_pipe (mypipe[0]) == EXIT_FAILURE) {
                        return EXIT_FAILURE;
                    }
                    close(mypipe[0]);
                    //Lets master know its done
                    exit(EXIT_SUCCESS);
                //master
                } else {
                    close (mypipe[0]);
                    //Sends filename to slave to process
                    write_to_pipe (mypipe[1],argv[file]);
                    close(mypipe[1]);
                    //next file to process
                    file++;
                }
            }
        }
        //master wait for the slaves to finish when there are no more files to process
        while(slaves != 0) {
            wait(&status);
            slaves--;
        }
    } else {
        printf("No files to process found\n");
    }
    return EXIT_SUCCESS;
}