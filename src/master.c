// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/masterSlave.h"
#include "include/shared_mem.h"
#include "include/sem_manager.h"

#define POUT_PATH "/tmp/pout"
#define PIN_PATH "/tmp/pin"
#define MAX_SLAVES 99
#define MAX_SLAVES_DIGIT 3 //max amount of slaves 99 + '0' (end of string)
#define PIPE_PATH_MAX 20

#define SHM_SIZE            1048576            // 1MB block size
#define PERMISSION_FLAGS    0666               // Could also or all predefined permission flags, but less practical

void merror (const char *err) {
    fprintf(stderr, "[master] %s", err);
}

//returns the highest value of the set of file descriptors
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

void toString(int num,char* resp){
	int digit=0;

    if(num!=0){
        // Builds the number in the array
		while(num!=0){
			resp[digit]=(num%10)+'0';
			num=num/10;
			digit++;
		}
        
        //inverts the number order
		for(int i=0, aux;i<digit/2;i++){
			aux=resp[i];
			resp[i]=resp[digit-i-1];
			resp[digit-i-1]=aux;
		}
		resp[digit]=0;
	}
    else{
		resp[digit++]='0';
	}
    //adds the final 0
    resp[digit]=0;
	return;
}

 int main (int argc, char *argv[]) {
    if(argc < 2) {
        merror("No files to process\n");
        exit(EXIT_FAILURE);
    } else {

        int shmid = create_block(SHM_SIZE, PERMISSION_FLAGS);
        if(shmid == -1){
            perror("[master] create_block");
            return EXIT_FAILURE;
        }

        char* shm_base = attach_block(shmid);
        if((long long)shm_base == -1){
            perror("[master] attach_block");
            return EXIT_FAILURE;
        }
        char* shmp = shm_base;                        // Used to change the direction pointed by shmp without altering its value

        // Creates semaphore, deleting any other with the same name, unless the user doesn´t have access to it
        sem_type semaphore = create_semaphore(COUNT_SEM_NAME, 0, SEM_PERMISSIONS);
        if(semaphore == SEM_FAILED){
            perror("[master] create_semaphore");
            return EXIT_FAILURE;
        }

        //the amount of files to process. Data for view
        printf("%d\n",argc-1);

        //creates outputfile
        FILE *outputfile = fopen(OUTPUT_NAME,"w");
        if(outputfile == NULL) {
            merror("Open output file error\n");
            exit(EXIT_FAILURE);
        }

        //The amount of slaves is 5% of the amount of files to process(Example is 3)
        //int slaves_dim = argc/20 + 1;
        int slaves_dim = 5;

        //Limit the amount of slaves to max at 99
        if(slaves_dim > MAX_SLAVES) {
            slaves_dim = MAX_SLAVES;
        }

        int pout_set[slaves_dim];
        int pin_set[slaves_dim];
        for(int slaves=1;slaves<=slaves_dim;slaves++) {
            
            //creating named pipes
            char digit[MAX_SLAVES_DIGIT]; 
            char pout_path[PIPE_PATH_MAX] = POUT_PATH; 
            char pin_path[PIPE_PATH_MAX] = PIN_PATH;
            toString(slaves,digit);
            strcat(pout_path,digit);
            strcat(pin_path,digit);
            remove(pout_path);
            remove(pin_path);
            if(mkfifo(pout_path, S_IRUSR|S_IWUSR) == -1){
                merror("mkfifo pout error");
                exit(EXIT_FAILURE);
            }
            if(mkfifo(pin_path, S_IRUSR|S_IWUSR) == -1){
                merror("mkfifo pin error");
                exit(EXIT_FAILURE);
            }
            //creating slaves
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
                //connects pipes with slave
                if((pout_set[slaves-1] = open(pout_path, O_WRONLY)) == -1){
                    merror("Error in Open Pout");
                    exit(EXIT_FAILURE);
                }
                if((pin_set[slaves-1] = open(pin_path, O_RDONLY)) == -1){
                    merror("Error in Open Pin");
                    exit(EXIT_FAILURE);
                }
            }
        }

        //file index (file 0 is ./solve)
        int files_sent = 0;
        int files_processed = 0;

        //data for select
        int maxfd = max_fd(pin_set,slaves_dim);
        fd_set fd_set_slaves;

        //send first set of files to process
        for(int slaves=1;slaves<=slaves_dim;slaves++) {
            write(pout_set[slaves-1],argv[files_sent+1],strlen(argv[files_sent+1])+1);
            files_sent++; 
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
                        return EXIT_FAILURE;
                    }

                    fprintf(outputfile,"%s", shmp);
                    if(semaphore_post(semaphore) == -1){
                        perror("[master] semaphore_post");
                        fclose(outputfile);
                        return EXIT_FAILURE;
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