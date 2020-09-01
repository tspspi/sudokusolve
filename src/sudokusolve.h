struct sudokuField {
	/*
		Index into the fields:

		col + row*9
	*/
	unsigned short		values[81];
};

static void printSudokuField(
        struct sudokuField* lpField
);

static int checkValidity(
	struct sudokuField* lpField
);

static struct sudokuField* sudokuLoadFile(
	char* lpFilename
);

static int solveRecursive(
	struct sudokuField* lpField,
	int col,
	int row
);

static void printSudokuField(
	struct sudokuField* lpField
);

static void storeSudokuField(
	struct sudokuField* lpField,
	char* lpFilename
);

int main(
	int argc,
	char* argv[]
);
