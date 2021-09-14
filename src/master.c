// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/masterSlave.h"
#include "include/shared_mem.h"
#include "include/sem_manager.h"

#define POUT_PATH "/tmp/pout"
#define PIN_PATH "/tmp/pin"
#define MAX_SLAVES 99
#define MAX_SLAVES_DIGIT 3                     // Max amount of slaves: 99 + '0' (end of string)

#define SHM_SIZE            1048576            // 1MB block size
#define PERMISSION_FLAGS    0666               // Could also or all predefined permission flags, but less practical
#define INIT_SLEEP_TIME     2

void merror (const char *err) {
    fprintf(stderr, "[master] %s", err);
}

void createNamedPipe(int slave, char path[PIPE_PATH_MAX]) {
    char digit[MAX_SLAVES_DIGIT]; 
    sprintf(digit,"%d",slave);
    strcat(path,digit);
    remove(path);
    if(mkfifo(path, S_IRUSR|S_IWUSR) == -1){
        merror("mkfifo path error");
        exit(EXIT_FAILURE);
    }
    return;
}

void createSlaves(int slaves_dim, int pout_set[slaves_dim], int pin_set[slaves_dim]) {
    for(int slave=1;slave<=slaves_dim;slave++) {
            
            char pout_path[PIPE_PATH_MAX] = POUT_PATH; 
            char pin_path[PIPE_PATH_MAX] = PIN_PATH;
            createNamedPipe(slave,pout_path);
            createNamedPipe(slave,pin_path);

            pid_t pid;
            pid = fork();
            if(pid == -1) {
                merror("fork error:");
                exit(EXIT_FAILURE);
            } 
            //slave
            if(pid == 0) {
                execl("./bin/slave","./bin/slave",pout_path,pin_path,NULL);
            }
            //master
            else {
                connectNamedPipe(&pout_set[slave-1],pout_path,O_WRONLY);
                connectNamedPipe(&pin_set[slave-1],pin_path,O_RDONLY);
            }
        }
        return;
}

// Returns the highest value of the set of file descriptors
int max_fd(int fd_slaves[],int dim) {
    if(dim <= 0) {
        return -1;
    }
    int maxfd = fd_slaves[0];
    for(int i=1;i<dim;i++) {
        if(maxfd<fd_slaves[i]) {
            maxfd = fd_slaves[i];
        }
    }
    return maxfd;
}

int main (int argc, char *argv[]) {
    if(argc < 2) {
        merror("No files to process\n");
        exit(EXIT_FAILURE);
    } else {
        setvbuf(stdout, NULL, _IONBF, 0);
        sleep(INIT_SLEEP_TIME);

        int shmid = create_block(SHM_SIZE, PERMISSION_FLAGS);
        if(shmid == -1){
            perror("[master] create_block");
            exit(EXIT_FAILURE);
        }

        char* shm_base = attach_block(shmid);
        if((long long)shm_base == -1){
            perror("[master] attach_block");
            exit(EXIT_FAILURE);
        }
        char* shmp = shm_base;                        // Used to change the direction pointed by shmp without altering its value

        // Creates semaphore, deleting any other with the same name, unless the user doesn´t have access to it
        sem_type semaphore = create_semaphore(COUNT_SEM_NAME, 0, SEM_PERMISSIONS);
        if(semaphore == SEM_FAILED){
            perror("[master] create_semaphore");
            exit(EXIT_FAILURE);
        }

        //the amount of files to process. Data for view
        printf("%d\n",argc-1);

        //creates outputfile
        FILE *outputfile = fopen(OUTPUT_NAME,"w");
        if(outputfile == NULL) {
            merror("Open output file error\n");
            exit(EXIT_FAILURE);
        }

        int slaves_dim = 2;
        //int slaves_dim = argc/20 + 1;
        if(slaves_dim > MAX_SLAVES) {
            slaves_dim = MAX_SLAVES;
        }
        int pout_set[slaves_dim];
        int pin_set[slaves_dim];
        createSlaves(slaves_dim,pout_set,pin_set);

        //file index (file 0 is ./solve)
        int files_sent = 0;
        int files_processed = 0;

        //data for select
        int maxfd = max_fd(pin_set,slaves_dim);
        fd_set fd_set_slaves;

        for(int slaves=1;slaves<=slaves_dim;slaves++) {
            for(int initial_load=1; initial_load <= slaves_dim;initial_load++) {
                write(pout_set[slaves-1],argv[files_sent+1],strlen(argv[files_sent+1])+1);
                files_sent++; 
            }
        }
        
        int read_bytes = 0;
        const size_t data_size = (SHM_SIZE / argc-1);         // MAX size of each block sent to view
        
        //while there are files to process
        while(files_sent != files_processed) {
            
            //reset set for select
            FD_ZERO(&fd_set_slaves);
            for(int slaves=1;slaves<=slaves_dim;slaves++) {
                FD_SET(pin_set[slaves-1], &fd_set_slaves);
            }

            //waits until a slave finishes
            if(select(maxfd+1,&fd_set_slaves,NULL,NULL,NULL) == -1) {
                perror("[master] select");
                exit(EXIT_FAILURE);
            }
            for(int slaves=1;slaves<=slaves_dim;slaves++) {
                if(FD_ISSET(pin_set[slaves-1],&fd_set_slaves)) {

                    read_bytes = read(pin_set[slaves-1], shmp, data_size);
                    if(read_bytes == -1){
                        perror("[master] read");
                        fclose(outputfile);
                        exit(EXIT_FAILURE);
                    }

                    fprintf(outputfile,"%s", shmp);
                    if(semaphore_post(semaphore) == -1){
                        perror("[master] semaphore_post");
                        exit(EXIT_FAILURE);
                    }
                    shmp += read_bytes + 1;
                    files_processed++;

                    if(files_sent < argc-1) {                        
                        write(pout_set[slaves-1],argv[files_sent+1],strlen(argv[files_sent+1])+1);
                        files_sent++;
                    }
                }
            }
        }
        for(int slaves=1;slaves<=slaves_dim;slaves++) {
            close(pout_set[slaves-1]);
            close(pin_set[slaves-1]);
        }
        fclose(outputfile);

        // Only detaches shared memory and closes semaphore instead of deleting both as view still needs them.
        if(detach_block(shm_base) == -1){
            perror("[master] detach_block");
            return EXIT_FAILURE;
        }
        if(close_semaphore(semaphore) == -1){
            perror("[master] close_semaphore");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}