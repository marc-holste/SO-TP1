# SO-TP1

To compile the project execute the following command:
    make all

To install minisat execute the following command:
    make install

To run the project execute the following command:
    make run

For debugging purposes the following commands can be used to produce test logs:
    make test-pvs-studio
    make test-cppcheck
    make test-valgrind

To execute all of these test commands at once, do:
    make test

Drop every file .cnf that is desired to be processed on the directory files.

Output of the process will be dunked in an output file named "outputfile.txt"

Output of the tests will be dunked in /test

Thanks for using SO-TP1