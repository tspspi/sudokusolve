#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./sudokusolve.h"

static unsigned long int tries;

/*
	Checks validity.
		-1 indicates error
		0 is a totally correct sudoku
		1 indicates missing data (i.e. some fields contain 0)
*/
static int checkValidity(
	struct sudokuField* lpField
) {
	unsigned short numbers;
	int iBlock;
	int iRow, iCol;
	int iZeros;
	unsigned short iVal;
	unsigned short iBit;
	int iRowStart, iColStart;

	iZeros = 0;
	if(lpField == NULL) { return -1; }

	#ifdef DEBUG
		printf("\n");
		printSudokuField(lpField);
		printf("\n");
	#endif

	/*
		We have to check that each number is
		contained only once in each and every
		square and that each number only occures
		in each and every row and column exactly
		once

		We keep track of the 9 numbers in a 16
		bit bitfield
	*/

	/*
		First do row and column checking one after
		each other.
	*/
	for(iRow = 0; iRow < 9; iRow=iRow+1) {
		numbers = 0;
		for(iCol = 0; iCol < 9; iCol=iCol+1) {
			iVal = lpField->values[iCol + iRow * 9];
			if(iVal == 0) {
				iZeros = 1;
			} else if(iVal > 9) {
				return -1;
			} else {
				iBit = 1 << (iVal - 1);
				if((numbers & iBit) != 0) {
					#ifdef DEBUG
						printf("\tFailed %s:%u\n", __FILE__, __LINE__);
					#endif
					return -1;
				}
				numbers = numbers | iBit;
			}
		}
	}

	for(iCol = 0; iCol < 9; iCol = iCol + 1) {
		numbers = 0;
		for(iRow = 0; iRow < 9; iRow=iRow+1) {
			iVal = lpField->values[iCol + iRow * 9];
			if(iVal == 0) {
				iZeros = 1;
			} else if(iVal > 9) {
				return -1;
			} else {
				iBit = 1 << (iVal -1);
				if((numbers & iBit) != 0) {
                                        #ifdef DEBUG
                                                printf("\tFailed %s:%u\n", __FILE__, __LINE__);
                                        #endif
					return -1;
				}
				numbers = numbers | iBit;
			}
		}
	}

	/*
		Now check in each subblock that each number occures exactly once

		Each block starts at:
			colStart = (iBlock % 3) * 3
			rowStart = (tBlock / 3) * 3
	*/

	for(iBlock = 0; iBlock < 9; iBlock = iBlock + 1) {
		numbers = 0;
		iRowStart = (iBlock % 3) * 3;
		iColStart = (iBlock / 3) * 3;

		for(iCol = iColStart; iCol < iColStart+3; iCol = iCol + 1) {
			for(iRow = iRowStart; iRow < iRowStart + 3; iRow = iRow + 1) {
				iVal = lpField->values[iCol + iRow * 9];
				if(iVal == 0) {
					iZeros = 1;
				} else if(iVal > 9) {
					return -1;
				} else {
					iBit = 1 << (iVal - 1);
					if((numbers & iBit) != 0) {
                	                        #ifdef DEBUG
        	                                        printf("\tFailed %s:%u\n", __FILE__, __LINE__);
	                                        #endif
						return -1;
					}
					numbers = numbers | iBit;
				}
			}
		}
	}

	#ifdef DEBUG
		printf("Ok\n");
	#endif
	if(iZeros == 1) {
		return 1;
	} else {
		return 0;
	}
}

static struct sudokuField* sudokuLoadFile(char* lpFilename) {
	FILE* fHandle;
	unsigned long int i, j;
	unsigned int data[9];
	int r;

	struct sudokuField* lpField;

	if(lpFilename == NULL) {
		return NULL;
	}

	lpField = (struct sudokuField*)malloc(sizeof(*lpField));
	if(lpField == NULL) {
		return NULL;
	}

	fHandle = fopen(lpFilename, "r");
	if(fHandle == NULL) {
		free(lpField);
		return NULL;
	}

	/* Read the field line by line (9 ints required) */
	for(i = 0; i < 9; i=i+1) {
		r = fscanf(fHandle, "%u %u %u %u %u %u %u %u %u\n", &(data[0]), &(data[1]), &(data[2]), &(data[3]), &(data[4]), &(data[5]), &(data[6]), &(data[7]), &(data[8]));

		if(r != 9) {
			free(lpField);
			return NULL;
		}

		for(j = 0; j < sizeof(data)/sizeof(unsigned int); j=j+1) {
			if(data[j] > 9) {
				free(lpField);
				return NULL;
			}

			lpField->values[i * 9 + j] = (unsigned short)data[j];
		}
	}

	return lpField;
}

/*
	-1	Error
	0	Success
	1	No fit
*/
static int solveRecursive(
	struct sudokuField* lpField,
	int col,
	int row
) {
	short iVal;
	int r;

	tries = tries + 1;
	if(tries % 1000000 == 0) {
		printf("Tried %lu combinations\n", tries);
	}

	if(row == 9) {
		return 0;
	}

	if(lpField == NULL) {
		return -1;
	}

	if(lpField->values[col + 9*row] != 0) {
		return solveRecursive(lpField, (col + 1) % 9, ((col + 1) / 9 == 0) ? row : row + 1);
	}

	for(iVal = 1; iVal < 10; iVal=iVal+1) {
		#ifdef DEBUG
			printf("Trying %u at %d %d\n", iVal, col, row);
		#endif
		lpField->values[col + row * 9] = iVal;
		r = checkValidity(lpField);
		if(r == 0) {
			return 0;
		} else if(r == 1) {
			r = solveRecursive(lpField, (col + 1) % 9, ((col + 1) / 9 == 0) ? row : row + 1);
			if(r == 0) {
				return 0;
			}
		}
	}

	lpField->values[col + row * 9] = 0;
	return -1;
}

static void printSudokuField(
	struct sudokuField* lpField
) {
	unsigned long int iRow, iCol;
	if(lpField == NULL) {
		return;
	}

	for(iRow = 0; iRow < 9; iRow = iRow + 1) {
		for(iCol = 0; iCol < 9; iCol = iCol + 1) {
			if(lpField->values[iCol + iRow * 9] != 0) {
				printf("%u ", lpField->values[iCol + iRow * 9]);
			} else {
				printf("  ");
			}
		}
		printf("\n");
	}
}

static void storeSudokuField(
	struct sudokuField* lpField,
	char* lpFilename
) {
	FILE* fHandle;
	unsigned long int iRow, iCol;

	if(lpField == NULL) {
		return;
	}

	fHandle = fopen(lpFilename, "w");
	if(fHandle == NULL) {
		return;
	}

	for(iRow = 0; iRow < 9; iRow = iRow + 1) {
		for(iCol = 0; iCol < 9; iCol = iCol + 1) {
			if(lpField->values[iCol + iRow * 9] != 0) {
				fprintf(fHandle, "%u ", lpField->values[iCol + iRow * 9]);
			} else {
				fprintf(fHandle, "  ");
			}
		}
		fprintf(fHandle, "\n");
	}

	fclose(fHandle);
}


int main(int argc, char* argv[]) {
	int r;

	time_t timeStart, timeEnd;
	unsigned long int th, tm, ts;

	/*
		Brute force sudoku solver
		-------------------------

		Solves classic sudoku with 81 squares
		and nine blocks by recursive backtracking.

		Not the most efficient method but no problem with
		81 blocks.
	*/
	if(argc < 1) {
		return 1;
	}

	struct sudokuField* lpField;

	lpField = sudokuLoadFile(argv[1]);
	if(lpField == NULL) {
		printf("Failed to load sudoku field\n");
		return 1;
	}

	printf("Input field:\n------------\n\n");
	printSudokuField(lpField);

	printf("\n\nTrying to solve ...\n");
	tries = 0;
	timeStart = time(&timeStart);
	r = solveRecursive(lpField, 0, 0);
	if(r == 0) {
		timeEnd = time(&timeEnd);

		ts = (timeEnd-timeStart) % 60;
		tm = ((timeEnd-timeStart) / 60) % 60;
		th = ((timeEnd-timeStart) / 3600);

		printf("Success (%lu hours, %lu minutes, %lu seconds)\n", th, tm, ts);

		printSudokuField(lpField);
		if(argc > 1) {
			storeSudokuField(lpField, argv[2]);
		}
		return 0;
	}

	printf("Failed");
	return 1;
}
