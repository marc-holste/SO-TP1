 #include <sys/types.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

void write_to_pipe (int file)
 {
   /* Writes in the pipe. */
   FILE *stream;
   stream = fdopen (file, "w");
   fprintf (stream, "Im writing through pipes\n");
   fprintf (stream, "Seems to work\n");
   fprintf (stream, "Makefile is done\n");
   fclose (stream);
 } 