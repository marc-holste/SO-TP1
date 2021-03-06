CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -g
SEM_LIBS=-lrt -pthread

SOURCES_FOLDER=src
SOURCES_MASTER=$(SOURCES_FOLDER)/master.c
SOURCES_SLAVE=$(SOURCES_FOLDER)/slave.c
SOURCES_VIEW=$(SOURCES_FOLDER)/view.c
SOURCES_SHM=$(SOURCES_FOLDER)/shared_mem.c
SOURCES_SEM=$(SOURCES_FOLDER)/sem_manager.c

INPUT_FOLDER=files
OUTPUT_FOLDER=bin
OUTPUT_MASTER=$(OUTPUT_FOLDER)/solve
OUTPUT_SLAVE=$(OUTPUT_FOLDER)/slave
OUTPUT_VIEW=$(OUTPUT_FOLDER)/view

# TEST_FOLDER=test
INCLUDE_FOLDER=$(SOURCES_FOLDER)/include

#VALGRIND_OUT_FOLDER=$(TEST_FOLDER)/valgrind
#CPP_OUT_FOLDER=$(TEST_FOLDER)/cppcheck
#PVS_LIC_PATH=../.config/PVS-Studio/PVS-Studio.lic
#PVS_OUT_FOLDER=$(TEST_FOLDER)/pvs-studio

INPUT_FILES=files/*
OUTPUT_FILE=out.txt

all: $(OUTPUT_MASTER) $(OUTPUT_SLAVE) $(OUTPUT_VIEW)

$(OUTPUT_MASTER): $(SOURCES_MASTER) $(SOURCES_SHM) $(SOURCES_SEM)
	$(CC) $(CFLAGS) -I$(INCLUDE_FOLDER) $^ -o $@ $(SEM_LIBS)

$(OUTPUT_SLAVE): $(SOURCES_SLAVE)
	$(CC) $(CFLAGS) -I$(INCLUDE_FOLDER) $^ -o $@

$(OUTPUT_VIEW): $(SOURCES_VIEW) $(SOURCES_SHM) $(SOURCES_SEM)
	$(CC) $(CFLAGS) -I$(INCLUDE_FOLDER) $^ -o $@ $(SEM_LIBS)

compile: all

install:
	mkdir -p $(OUTPUT_FOLDER); mkdir -p $(INPUT_FOLDER); apt-get install minisat

clean:
	rm -rf $(OUTPUT_FOLDER)/* $(OUTPUT_FILE)

delete:
	rm -rf $(OUTPUT_FOLDER)/*

# ---------------------------------------------------------------------------------------------------------------------------------
# To install license for PVS-Studio, do:
# apt-get update
# apt-get install pvs-studio
# pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
# ---------------------------------------------------------------------------------------------------------------------------------

#
#test-pvs-studio:
#	mkdir -p $(PVS_OUT_FOLDER);
#	make clean
#	pvs-studio-analyzer trace -- make
#	pvs-studio-analyzer analyze --compiler $(CC) -j2 -o $(PVS_OUT_FOLDER)/PVS-Studio.log
#	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o $(PVS_OUT_FOLDER)/report.tasks $(PVS_OUT_FOLDER)/PVS-Studio.log
#	mv strace_out $(PVS_OUT_FOLDER)/strace_out

#test-cppcheck:
#	mkdir -p $(CPP_OUT_FOLDER);
#	cppcheck --quiet --enable=all --force --inconclusive $(SOURCES_FOLDER) 2> $(CPP_OUT_FOLDER)/cppcheck-report.txt

#test-valgrind:
#	mkdir -p $(VALGRIND_OUT_FOLDER);
#	valgrind --log-file="$(VALGRIND_OUT_FOLDER)/master-report.txt" $(OUTPUT_MASTER) $(INPUT_FILES);
#	valgrind --log-file="$(VALGRIND_OUT_FOLDER)/view-report.txt" $(OUTPUT_VIEW) 13;

#test:
#make test-pvs-studio
#	make test-cppcheck
#	make test-valgrind

.PHONY: all compile install clean delete 
