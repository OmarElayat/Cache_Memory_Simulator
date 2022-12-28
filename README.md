## Overview

The Cache memory simulator is a C++ program that reads the sequence file and then implement the direct mapping cache method to simulate the behavior of the memory. The program supports separate caches for instructions and data. The program is decomposed into stages: 
1)	The program starts by initializing a memory simulator class where all the cache simulation procedures take place.
2)	In the constructor of the class, all the user defined data are initialized to the class local variables. User defined data are the sequence file name, cache size, cache line size, and the number of cycles needed to access the memory.
3)	Then in read_seq method, the program iterates on the sequence, parse the memory addresses and classifies according to its type (data or instructions).
4)	Afterwards, execute function parses the input memory address from the sequence file to byte offset bits, word offset bits, index bits, and tag bits.
5)	The program saves the tag and index of each memory address in data structures (dynamic pairs/maps)
6)	Then, for each memory address, its tag/index pair are compared with the contents of the previously saved data structures, if it can’t be found, the output be a miss else the output will be a hit.
7)	The process is repeated twice for both caches.
8)	The function “open write” opens csv files for containing outputs
9)	The function “write” writes the output in the CSV file and simulates the instruction and data caches after executing the sequence.  
10) The program writes all the required data in two excel files; one for the data cache, and the other for the instructions cache.

## Design Decision and Assumptions
To implement all Cache memory simulator. Some assumptions and design decisions had to be made. Including:
1)	All the addresses are written with no spaces between operands and one space after the address e.g.  123456 d
2)	Every address access occur in one line
3)	The program doesn’t tolerate spaces.


## Step-by-Step User Guide

**General Note:** 

1)	All the programs are in a folder called “programs”. In order to run them:
 * Move the two sequence files of the program you want to run to the same location as the source.cpp file.
 * Write the name of the sequence you want to run in the main.
 * Input the instruction cache size, length, and cycles needed for accessing the memory.
 * Enter the data cache size, length, and cycles needed for accessing the memory. 
2)	You can find the .csv output files produced in the same directory.

**Instructions Cache output by sequence (sequence2):**

![image](https://user-images.githubusercontent.com/107650627/209873737-46aa8236-dd89-4cf1-8e48-95a31787a64b.png)

**Instructions Cache output Organized(sequence2):**

![image](https://user-images.githubusercontent.com/107650627/209873750-17bb6d6a-2814-4a59-bdb6-ed78649b18fa.png)

**Data Cache output by sequence (sequence2):**
![image](https://user-images.githubusercontent.com/107650627/209873762-2b016e26-9927-4a23-831f-94348aed956e.png)


**Data Cache output organized(sequence2):**

![image](https://user-images.githubusercontent.com/107650627/209873775-be3ce0cc-94ac-4043-b054-1ccf1d101f50.png)


***You can two of the sequences we tried on our project attached***
