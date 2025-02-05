# ThreadedPrimeSearch

This is a simple program that searches for prime numbers up to a y number given x threads.

## Configuration

There are two different types of configurations to run the program, and they are as follows:
1. Different printing variations
- IMMEDIATE: Print immediately
- WAIT_ALL: _Wait until all threads are done then print everything
2. Different task division schemes
- RANGE: Straight division of search range. (ie for 1 - 1000 and 4 threads the division will be 1-250, 251-500, and so forth) 
- LINEAR: The search is linear but the threads are for divisibility testing of individual numbers.

These configurations, alongisde x threads and y number can be set in the 'config.txt' file with the following format:
- threadPrintVariation [IMMEDIATE/WAIT_ALL]
- threadTaskDivision [RANGE/LINEAR]
- x [number of threads]
- y [prime search up to this number]

Example 'config.txt':
```
threadPrintVariation IMMEDIATE
threadTaskDivision RANGE
x 4
y 1000
```

## Running the program

1. To run the program, ensure that the 'config.txt' file is in the same directory as the source files.
2. Ensure that the 'config.txt' file is correctly formatted.
3. Run 'main.cpp' on a C++ compiler (recommended: Visual Studio).

Once it runs, the program will only output prime numbers, as well as the thread number that found the prime number and its timestamp.