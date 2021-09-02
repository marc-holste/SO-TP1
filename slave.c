 #include <sys/types.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

#define MAX_FILENAME_LENGTH 256

//Declaration is in master
extern char *outputfilename;

//Demo
void process(char *filename) {
    FILE *stream = fopen (filename, "r");
    FILE *outputfile = fopen(outputfilename,"a");
    int c;

    while ((c = fgetc (stream)) != EOF) {
        fprintf(outputfile,"%c", c);
    }
    fprintf(outputfile,"%s finished proccesing\n",filename);

    fclose (stream);
    fclose(outputfile);
    return;
}

//Reads the filename and process it
int read_from_pipe (int file) {
    FILE *stream = fdopen (file, "r");
    int c;
    char filename[MAX_FILENAME_LENGTH]; 
    int i=0;
    while( (c = fgetc(stream)) != EOF && i<MAX_FILENAME_LENGTH-1) {
        filename[i++] = c;
    }
    filename[i]=0;
    if(i == MAX_FILENAME_LENGTH-1) {
        return EXIT_FAILURE;
    }
    fclose(stream);
    process(filename);
    return EXIT_SUCCESS;
}