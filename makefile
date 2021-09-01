CC=gcc
CFLAGS=-std=gnu99 -Wall
OUTPUT_EXEC=solve
OUTPUT_FILE=out.txt

all: compile clean

compile: 
	$(CC) $(CFLAGS) -c master.c
	$(CC) $(CFLAGS) -c slave.c;
	$(CC) $(CFLAGS) -o $(OUTPUT_EXEC) master.o slave.o 
	$(CC) $(CFLAGS) -o view view.c

clean:
	rm -f *.o $(OUTPUT_FILE) 
	
delete:
	rm -f $(OUTPUT_EXEC) view
run:
	./$(OUTPUT_EXEC) 