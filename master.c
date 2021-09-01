#include <sys/types.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include "slave.h"

 void read_from_pipe (int file)
 {
   /* Read chars from pipe and printd it in stdout. */	 
   FILE *stream;
   int c;
   stream = fdopen (file, "r");
   while ((c = fgetc (stream)) != EOF) {
     printf ("%c", c);
   }
   fclose (stream);
 }

 int main (void)
 {
   pid_t pid;
   int mypipe[2];

   pipe (mypipe);
   pid = fork ();

   if (pid == 0) {
       /* I am the son (consumer). I don't need read end. */
       close (mypipe[0]);
       write_to_pipe (mypipe[1]);
   } else {
       /* I am the father (producer). I don't need write end. */
       close (mypipe[1]);
       read_from_pipe (mypipe[0]);
   }

   return EXIT_SUCCESS;
 }