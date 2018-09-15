#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sudoku.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

static int *shared;

int main(int argc, char **argv)
{

    int DIM = 9;
    int NUM_TEST_CASES = DIM;
    if (argc == 2)
    {
        NUM_TEST_CASES = atoi(argv[1]);
    }
    else
    {
        printf("Please provide argument for number of valid sudoku solutions to test (max 18).\n");
        return 0;
    }

    // int **validTestInputs = malloc(NUM_TEST_CASES * sizeof(int *));
    // int **invalidTestInputs = malloc(NUM_TEST_CASES * sizeof(int *));
    printf("\nGenerating %d sudoku grids for test cases...\n\n", NUM_TEST_CASES * 2);
    printf("Testing %d positive cases...\n", NUM_TEST_CASES);
    int FAILURES = 0;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        int *validSudoku = generateSudoku(i, 1);
        if (!checkSudoku(validSudoku))
        {
            printf("Positive test case %d/%d failed!\n", i + 1, NUM_TEST_CASES);
            FAILURES++;
            continue;
        }
        printf("Positive test case %d/%d passed!\n", i + 1, NUM_TEST_CASES);
    };
    printf("Testing %d negative cases...\n", NUM_TEST_CASES);
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        int *invalidSudoku = generateSudoku(i, 0);
        if (checkSudoku(invalidSudoku))
        {
            printf("Negative test case %d/%d failed!\n", i + 1, NUM_TEST_CASES);
            FAILURES++;
            continue;
        }
        printf("Negative test case %d/%d passed!\n", i + 1, NUM_TEST_CASES);
    };
    if (FAILURES > 0)
    {
        printf("%d/%d test cases failed.\n", FAILURES, NUM_TEST_CASES * 2);
    }
    else
    {
        printf("\n*******************\nAll %d/%d test cases passed!\n*******************\n", NUM_TEST_CASES * 2, NUM_TEST_CASES * 2);
    }
};
