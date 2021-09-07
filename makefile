CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -g

SOURCES_MASTER=src/master.c
SOURCES_SLAVE=src/slave.c
SOURCES_VIEW=src/view.c

OUTPUT_MASTER=bin/solve
OUTPUT_SLAVE=bin/slave
OUTPUT_VIEW=bin/view

INPUT_FILES=files/*
OUTPUT_FILE=out.txt

all: $(OUTPUT_MASTER) $(OUTPUT_SLAVE) $(OUTPUT_VIEW)

$(OUTPUT_MASTER): $(SOURCES_MASTER)
	$(CC) $(CFLAGS) $^ -o $@

$(OUTPUT_SLAVE): $(SOURCES_SLAVE)
	$(CC) $(CFLAGS) $^ -o $@

$(OUTPUT_VIEW): $(SOURCES_VIEW)
	$(CC) $(CFLAGS) $^ -o $@

compile: all

install:
	mkdir bin; apt-get install minisat

clean:
	rm -f bin/* $(OUTPUT_FILE)

delete:
	rm -f bin/*

run:
	rm -f $(OUTPUT_FILE); ./$(OUTPUT_MASTER) $(INPUT_FILES)

.PHONY: all compile install clean delete run