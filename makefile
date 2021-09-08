CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -g

SOURCES_FOLDER=src
SOURCES_MASTER=$(SOURCES_FOLDER)/master.c
SOURCES_SLAVE=$(SOURCES_FOLDER)/slave.c
SOURCES_VIEW=$(SOURCES_FOLDER)/view.c

OUTPUT_FOLDER=bin
OUTPUT_MASTER=$(OUTPUT_FOLDER)/solve
OUTPUT_SLAVE=$(OUTPUT_FOLDER)/slave
OUTPUT_VIEW=$(OUTPUT_FOLDER)/view

TEST_FOLDER=test

PVS_LIC_PATH=../.config/PVS-Studio/PVS-Studio.lic

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
	mkdir -p $(OUTPUT_FOLDER); apt-get install minisat

clean:
	rm -rf $(OUTPUT_FOLDER)/* $(OUTPUT_FILE) $(TEST_FOLDER)/*

delete:
	rm -rf $(OUTPUT_FOLDER)/*

run:
	rm -rf $(OUTPUT_FILE); $(OUTPUT_MASTER) $(INPUT_FILES)

# TODO: Add pvs-studio analysis to make test
# ---------------------------------------------------------------------------------------------------------------------------------
# EXTRA: To install license for PVS-Studio, do:
# pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
# ---------------------------------------------------------------------------------------------------------------------------------
# rm -rf $(OUTPUT_FOLDER)/* $(OUTPUT_FILE) test/*
# pvs-studio-analyzer trace -- make;
# pvs-studio-analyzer analyze --compiler $(CC) -j2 -l $(PVS_LIC_PATH) -o test/pvs-studio/PVS-Studio.log;
# plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o test/pvs-studio/report.tasks test/pvs-studio/PVS-Studio.log;
# ---------------------------------------------------------------------------------------------------------------------------------

test:
	mkdir -p $(TEST_FOLDER);
	mkdir -p $(TEST_FOLDER)/valgrind;
	mkdir -p $(TEST_FOLDER)/cppcheck;
	mkdir -p $(TEST_FOLDER)/pvs-studio;
	valgrind --log-file="$(TEST_FOLDER)/valgrind/valgrind-report.txt" $(OUTPUT_MASTER) $(INPUT_FILES);
	cppcheck --quiet --enable=all --force --inconclusive $(SOURCES_FOLDER) 2> $(TEST_FOLDER)/cppcheck/cppcheck-report.txt

.PHONY: all compile install clean delete run test