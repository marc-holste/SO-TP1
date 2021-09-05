CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -g

SOURCES_MASTER_SLAVE=src/master.c src/slave.c
SOURCES_VIEW=src/view.c

OUTPUT_EXEC=bin/solve
OUTPUT_VIEW=bin/view
INPUT_FILES=files/*
OUTPUT_FILE=out.txt

all: $(OUTPUT_EXEC) $(OUTPUT_VIEW)

$(OUTPUT_EXEC): $(SOURCES_MASTER_SLAVE)
	$(CC) $(CFLAGS) $^ -o $@

$(OUTPUT_VIEW): $(SOURCES_VIEW)
	$(CC) $(CFLAGS) $^ -o $@

compile: all

install: 
	apt-get install minisat

clean:
	rm -f bin/* $(OUTPUT_FILE)

delete:
	rm -f $(OUTPUT_EXEC) $(OUTPUT_VIEW)

run:
	rm -f $(OUTPUT_FILE); ./$(OUTPUT_EXEC) $(INPUT_FILES)

.PHONY: all compile install clean delete run