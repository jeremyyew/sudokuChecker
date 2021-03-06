#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int DIM = 9;
/**
 * load_sudoku return EXIT_SUCCESS when filename ccontains a valid sudoku and that this soduko is correctly loaded in memory.
 **/

int load_sudoku(char *filename, int *grid)
{

  FILE *fp = fopen(filename, "r");

  if (fp != NULL)
  {
    int idx = 0;
    do
    {
      char c = fgetc(fp);
      if (c != EOF && c != 10 && c != 32)
      {
        if (c < 49 || c > 57)
        {
          fprintf(stderr, "Invalid Character: %d. ", c);
          fclose(fp);
          return EXIT_FAILURE;
        }
        else
        {
          grid[idx] = c - 48;
          idx++;
        }
      }

    } while (feof(fp) == 0);

    fclose(fp);
  }
  else
  {
    fprintf(stderr, "Invalid File: %s", filename);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int getNthGridKthIndex(int n, int k)
{
  int gridHorizOffset = n % 3 * 3;
  int gridVertOffset = ((n - (n % 3)) / 3) * 27;
  int elementHorizOffset = k % 3;
  int elementVertOffset = ((k - (k % 3)) / 3) * 9;
  return (gridHorizOffset + gridVertOffset + elementHorizOffset + elementVertOffset);
};

int getNthRowKthIndex(int n, int k)
{
  return (n * 9) + k;
};

int getNthColKthIndex(int n, int k)
{
  return n + (k * 9);
};

int exactlyOneEach(int flags[])
{
  for (int i = 0; i < 9; i++)
  {
    if (flags[i] != 1)
    {
      return 0;
    }
  }
  return 1;
}

int checkEverySection(int *S, int (*getNthSectionKthIndex)(int, int), int start, int end)
{
  for (int n = start; n < end; n++)
  {
    int flags[9] = {0};
    for (int k = 0; k < 9; k++)
    {
      int val = S[getNthSectionKthIndex(n, k)];
      if (val >= 1 && val <= 9) //we already validate inputs, but just to make sure no seg fault
      {
        flags[val - 1] += 1;
      }
    }
    if (exactlyOneEach(flags) == 0)
    {
      // printf("Failed at %dth ", n + 1);
      return 0;
    }
  }
  return 1;
};

int checkEveryGrid(int *S, int start, int end)
{
  return checkEverySection(S, getNthGridKthIndex, start, end);
};

int checkEveryCol(int *S, int start, int end)
{
  return checkEverySection(S, getNthColKthIndex, start, end);
};

int checkEveryRow(int *S, int start, int end)
{
  return checkEverySection(S, getNthRowKthIndex, start, end);
};

int checkEveryRowColGridInRange(int *S, int start, int end)
{
  // printf("checking row %d to %d\n", start, end - 1);
  int rowsAreValid = checkEveryRow(S, start, end);
  if (!rowsAreValid)
  {
    // printf("row.\n");
  }
  // printf("checking col to %d to %d\n", start, end - 1);
  int colsAreValid = checkEveryCol(S, start, end);
  if (!colsAreValid)
  {
    // printf("column.\n");
  }
  // printf("checking grid to %d to %d\n", start, end - 1);
  int gridsAreValid = checkEveryGrid(S, start, end);
  if (!gridsAreValid)
  {
    // printf("grid.\n");
  }
  if (!rowsAreValid || !colsAreValid || !gridsAreValid)
  {
    return 0;
  }
  return 1;
}

/**
 * `generateSudoku` returns an integer pointer representing a sudoku grid. The grid can be valid or invalid based on the `valid` param. The `offset` param (value 0-18) allows us to produce 18 distinct valid or invalid grids, for a total of 36 grids. 
 **/
int *generateSudoku(int offset, int valid)
{
  int n_squared = (int)pow((double)9, 2);
  int *grid = malloc(n_squared * sizeof(int));
  if (valid == 1)
  {
    // printf("Valid Sudoku grid %d:\n\n", offset);
  }
  else
  {
    // printf("Invalid Sudoku grid %d:\n\n", offset);
  }

  for (int row = 0; row < 9; row++)
  {
    const int rowStart = row * 9;
    // printf("\t\t");

    int startVal = (row / 3);
    int vertOffset = (row % 3) * 3;
    for (int col = 0; col < 9; col++)
    {
      int currentIndex = rowStart + col;
      if (offset > 9)
      {
        //fill by column instead
        currentIndex = row + col * 9;
      }
      int val = ((startVal + vertOffset + col) + offset) % 9 + 1;
      // Switch the last two numbers. This makes last two columns incorrect.
      if (valid != 1 && currentIndex == n_squared - 1)
      {
        val--;
      }
      else if (valid != 1 && currentIndex == n_squared - 2)
      {
        val++;
      }

      grid[currentIndex] = val;
      // printf("%d ", val);
    };
    // printf("\n");
  };
  // printf("\n");
  return grid;
};
