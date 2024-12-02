# OS_Lab3
EECE.4811 Lab 3

PROGRAM PARAMETERS:
The way this program runs can be modified by changing some definitions in *MMS.h*.

To change between first/best/worst fit, modify the value of *FIT_MODE*:
 * 1: First fit.
 * 2: Best fit.
 * 3: Worst fit.

To enable/disable defragmentation, modify the value of *DEFRAG_ENABLE*.
 * 1: Defrag is ON.
 * 2: Defrag is OFF.

To change the number of blocks in memory, change *MAX_BLOCKS*.

To change the memory arena size, change *MAX_SIZE*.

Once you've made your changes, run the program with the following commands in a Linux shell from the directory the c files and makefile are in:

1.) Run the makefile with the command `make`.
2.) Run the program with `./Lab_3 n`. Replace `n` with an integer for the number of threads you'd like to have run.

The user threads will each run through 3 requests before completing. The MMS thread runs indefinitely: To end the program, press `ctrl + c`.
