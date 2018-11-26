## Assignment 2 Pthreads

## Table of Contents

1. Multithreaded Grep
	1. Concurrency
	2. Performance
	3. Hints
	4. Deliverables
	5. Grading

#### Multithreaded Grep

Grep is a classic UNIX tool for finding patterns in files. In this assignment, you will be developing a parallel version of grep, which we shall call pargrep.

You are encouraged to use the pthreads library for multi-threading. Your solution should be able read a single (large) file, and print all the lines containing the matching pattern. To keep things simple, you are only required to support very rudimentary pattern mathcing, and not full-blown regular expression.

As with grep, the basic command arguments of pargrep are :

    $> pargrep [-t] word [file]

    Searches for <word> in provided <file> or standard input.

    Pargrep options: 
    -t          number of threads
    word        string to be searched in the file
    file        file to lookup or search

The output will be all the lines (in order!) that contain the word.

For example, if file is:

    This is the first line containing a word. 
    Words are overrated anyway. //Note that grep is case sensitive by default 
    The last word. 

The output should be:

    This is the first line containing a word. 
    The last word. 

Your output should be the exactly what grep outputs. Note also that your pargrep should be able to process input from either a file or piped input. For example :

    $> pargrep word ./myfile

OR

    $> cat ./myfile | pargrep word 

This is how your program will be evaluated for correctness.

#### Concurrency

The overall design of pargrep is up to you. However, some key questions to consider:

1. How to split the input?
2. How many threads to use?
3. How to get the output in order?

#### Performance

You are expected to compare the performance of your implementation with different parameters. Change the number of threads, etc., and record all times. You are welcome to experiment with these parameters via command line arguments. However, your program should have sane defaults that allows it to work with no extra parameters (other than the pattern and the file).

#### Hints

You can use the getline and strstr C library functions to read lines from the file and search for the word.

For testing, you can use any large text file. Download a few books from Project Gutenberg, for instance. https://www.gutenberg.org/browse/scores/top

Pthreads: https://computing.llnl.gov/tutorials/pthreads/

#### Deliverables

Your submission must have:

1. A Makefile that generates the binary, named "pargrep".
2. A report outlining your design. Explain clearly:
	1. The concurrency design that you chose, and why.
	2. Compare performance with standard (non-parallel) grep changing the number of threads, etc.

Bonus points will be given to the fastest grep implementation, and for particularly clever and creative solutions.

#### Grading
1. Correct output (50 pts)
2. pargrep gives correct output for piped input (10 pts)
3. Report explaining design clearly and compares performance (40 pts)
4. Bonus: Performance. How much faster is it compared to conventional, single-threaded tools out there? 20% faster => 20 points. Max 50 points.
