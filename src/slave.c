// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/masterSlave.h"

// Minisat Path 
#define MINISAT_PATH      "minisat "
#define PARSE_VARCLAUS    " | grep -o -e \"Number of.*[0-9]\\+\""
#define PARSE_CPU         " -e \"CPU time.*\""
#define PARSE_VAL         " -e \".*SATISFIABLE\""

#define SLAVE_ID_MAX 23             // slaveID default message + MAX integer + \0 = 23 characters

void serror (const char *err) {
    fprintf(stderr, "[slave] %s", err);
}

void exec_minisat(char file_path[PIPE_BUF],int fd_write) {
    char slaveID[SLAVE_ID_MAX] = "";
    char *line = NULL;
    size_t len = 0;

    //saves slave ID
    snprintf(slaveID, PIPE_BUF, "Slave ID = %d\n", getpid());

    //prepares minisat comand
    FILE *minisat;
    char minisat_cmd[PATH_MAX] = MINISAT_PATH;
    strcat(minisat_cmd,file_path);
    strcat(minisat_cmd,PARSE_VARCLAUS);
    strcat(minisat_cmd,PARSE_CPU);
    strcat(minisat_cmd,PARSE_VAL);

    //execute minisat
    minisat = popen(minisat_cmd,"r");
    if(minisat == NULL) {
        serror("Minisat: error");
        exit(EXIT_FAILURE);
    }

    //prepare result
    char output[PIPE_BUF] = "Filename: ";
    strcat(output,file_path);
    strcat(output,"\n");
    strcat(output,slaveID);
    while(getline(&line,&len,minisat) > 0) {
        strcat(output,line);
    }
    strcat(output,"\n");

    //send result to master
    write(fd_write,output,strlen(output)+1);
    pclose(minisat);
    return;
}

//expected arguments:
//  argv[0] = ./bin/slave
//  argv[1] = fd_read
//  argv[2] = fd_write
int main(int argc, char *argv[]) {
    if(argc < 3) {
        serror("Not enough arguments");
        exit(EXIT_FAILURE);
    }
    int fd_read,fd_write;
    connectNamedPipe(&fd_read,argv[1],O_RDONLY);
    connectNamedPipe(&fd_write,argv[2],O_WRONLY);

    char file_paths[PIPE_BUF];
    char file_path[PIPE_BUF];
    int read_bytes;
    int index_file_path = 0;
    //reads until master closes pipe
    while((read_bytes = read(fd_read, file_paths, sizeof(file_paths))) > 0) {
        
        //if only one file_path was sent
        if(strlen(file_paths)+1 == (size_t)read_bytes) {
            exec_minisat(file_paths,fd_write);
        //if more then one file_path was sent
        } else {
            for(int i=0;i<read_bytes;i++) {
                if(file_paths[i] == 0) {
                    file_path[index_file_path] = 0;
                    index_file_path = 0;
                    exec_minisat(file_path,fd_write);
                } else {
                    file_path[index_file_path++] = file_paths[i];
                }
            }
        }
    }
    close(fd_write);
    close(fd_read);
    return EXIT_SUCCESS;
}