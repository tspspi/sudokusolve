# Simple (backtracking) sudoku solver

Just a quick hack (don't expect good coding practice or style, took around 15 minutes
to code - writing this README took longer than the code) that solves standard
9x9 Sudokus with 3x3 subfields that are composed by numbers from 1 to 9. Takes
around 3 minutes for a difficult sudoku on a decent machine (no optimizations,
no heuristics, no parallelization - just plain brute force).

## Compiling

Since the code is ANSI C99 any platform should work. The ```Makefile``` uses the C
compiler configured by ```CC``` environment variable and defaults to ```clang```.
Currently the ```Makefile``` uses GNU make on any compatible Unix like system:

```
gmake
```

## Data file

The sudoku solver requires a data file as input. This file consists of 9 by 9
numbers in a plain ASCII file. Empty fields are filled with 0. For example:

```
0 3 0 0 0 0 0 0 0
0 0 0 1 9 5 0 0 0
0 0 8 0 0 0 0 6 0
8 0 0 0 6 0 0 0 0
4 0 0 8 0 0 0 0 1
0 0 0 0 2 0 0 0 0
0 6 0 0 0 0 2 8 0
0 0 0 4 1 9 0 0 5
0 0 0 0 0 0 0 7 0
```

## Running

One can simply launch the solver by running ```./sudokusolv SOURCEFILE```. This
reads the sudoku from SOURCEFILE and prints the output to standard output.
Optionally a third parameter can be passed in which case the result will be
written into the specified destination file (```./sudokusolv SOURCEFILE DESTFILE```).

```
$ ./sudokusolv ../testdata/test01.dat
Input field:
------------

  3               
      1 9 5       
    8         6   
8       6         
4     8         1
        2         
  6         2 8   
      4 1 9     5
              7   


Trying to solve ...
Tried 1000000 combinations
Tried 2000000 combinations
Tried 3000000 combinations
Tried 4000000 combinations
Tried 5000000 combinations
Tried 6000000 combinations
Tried 7000000 combinations
Tried 8000000 combinations
Tried 9000000 combinations
Tried 10000000 combinations
Tried 11000000 combinations
Tried 12000000 combinations
Tried 13000000 combinations
Success (0 hours, 1 minutes, 38 seconds)
5 3 4 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
1 9 8 3 4 2 5 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 5 3 7 2 8 4
2 8 7 4 1 9 6 3 5
3 4 5 2 8 6 1 7 9
```
