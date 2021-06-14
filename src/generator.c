#include "generator.h"

#include "output.h"
#include "solver.h"

#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define MINUS_DIVIDE_PROBA 30
#define MULTIPLY_DEFAULT_PROBA 60

#define DIVIDING_TIME 1000000
#define MAX_BLOCK_SIZE 5
#define NB_OPERATORS 4
#define SEED 4

enum operators { PLUS, MINUS, MULTIPLY, DIVIDE };

/* Shuffles the indices of an array. */
static void shuffle(int *array, size_t n) {
  if (n > 1) {
    for (size_t i = 0; i < n - 1; ++i) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

static void copy_line(int *src, int *dest, int line_size) {
  for (size_t i = 0; i < line_size; ++i) {
    dest[i] = src[i];
  }
}

static void shuffle_grid(grid_t *grid, int *indices) {
  int size = grid->size;
  int tmp_row[size];
  int tmp_grid[size * size];

  shuffle(indices, size);

  /* Shuffle rows. */
  for (size_t i = 0; i < size; ++i) {
    copy_line(&grid->board[i * size], tmp_row, size);
    copy_line(&grid->board[indices[i] * size], &grid->board[i * size], size);
    copy_line(tmp_row, &grid->board[indices[i] * size], size);
  }

  /* Copy grid values in tmp_grid. */
  for (size_t i = 0; i < size * size; ++i) {
    tmp_grid[i] = grid->board[i];
  }

  shuffle(indices, size);

  /* Shuffle columns. */
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      grid->board[INDEX(j, i, grid->size)] =
      tmp_grid[INDEX(j, indices[i], grid->size)];
    }
  }
}

static void fill_values(grid_t *grid, bool vrb) {
  int size = grid->size;
  int pos = 0;

	int array[size];

	for (size_t k = 0; k < size; ++k)
		array[k] = k;

  /* Now, array contains values from 1 to the size of the grid. */

	for (size_t i = 0; i < size; ++i) {
	  for (size_t j = 0; j < size; ++j) {
		  grid->board[INDEX(i, j, grid->size)] = array[pos++] + 1;

			if (pos == size)
				pos = 0;
		}
		++pos;
	}
  /* Now, array contains indices. */

  if (vrb)
    printf("kenken: verbose: shuffle lines and columns %d times\n", SEED);

  for (size_t i = 0; i < SEED; ++i)
    shuffle_grid(grid, array);
}

static long calculate_goal(int opr, grid_t *grid, int num_block) {
  int nb_squares = grid->blocks[num_block].nb_squares;
  long num;

  int x = grid->blocks[num_block].squares[0].x;
  int y = grid->blocks[num_block].squares[0].y;
  long goal = grid->board[INDEX(x, y, grid->size)];

  for (size_t i = 1; i < nb_squares; ++i) {
    x = grid->blocks[num_block].squares[i].x;
    y = grid->blocks[num_block].squares[i].y;
    num = grid->board[INDEX(x, y, grid->size)];

    switch(opr) {
      case PLUS:
        goal += num;
        break;

      case MINUS:
        goal = abs(num - goal);
        break;

      case MULTIPLY:
        goal *= num;
        break;

      case DIVIDE:
        if (goal > num)
          goal = goal / num;
        else
          goal = num / goal;
        break;
    }
  }

  return goal;
}

static bool try_to_put_block(grid_t *grid, int row, int col, int num_block) {
  int size = grid->size;

  if ((row < size)
  && (col < size)
  && (grid->block_board[INDEX(row, col, grid->size)] == 0)) {
    grid->block_board[INDEX(row, col, grid->size)] = num_block;
  }
  else
    return false;

  return true;
}

static void fill_block_board(grid_t *grid, int difficulty, bool vrb) {
  int size = grid->size, num_block = 1, nb_squares = 1;
  int index = 0;
  int choice, tmp_max;
  int row, col;
  bool is_full = false;

  while (! is_full) {
    if (grid->block_board[index] != 0) {
      ++index;
      is_full = (index == (size * size));
      continue;
    }

    nb_squares = 1;
    grid->block_board[index] = num_block;
    row = index / size;
    col = index % size;

    tmp_max = (rand() % MAX_BLOCK_SIZE) + 1;
    while (nb_squares++ < tmp_max) {

      /* Used to stop when 2 squares are in a block.
       * The greater is the difficulty, the lowest is the chance to have
       * a block of size 2. */
      if ((nb_squares == 3) && ((rand() % 100) <= 50 - 5 * difficulty)) break;

      /* Used to have block with smaller size.
       * The greater is the diffculty, the lowest is the chance to have
       * a block of small. */
      if (rand() % 100 >= difficulty * 15) break;

      /* If choice is 1 : the next square will be on the same row. */
      choice = rand() % 2;

      if (! try_to_put_block(grid, row += (! choice),
      col += choice, num_block)) {
        row -= (! choice);
        col -= choice;
        if (! try_to_put_block(grid, row += choice,
        col += (! choice), num_block))
          break;
      }
    }

    if (vrb) printf("kenken: verbose: %d square(s) allocated to block %d\n",
    nb_squares - 1, num_block);

    ++num_block;
    ++index;

    is_full = (index == (size * size));
  }

  grid->nb_blocks = num_block - 1;

}

/* Checks if it is possible to put a divide operator in the block. */
static bool is_divide_possible(grid_t *grid, int num_block) {
  int a, b, x, y;

  x = grid->blocks[num_block].squares[0].x;
  y = grid->blocks[num_block].squares[0].y;
  a = grid->board[INDEX(x, y, grid->size)];

  x = grid->blocks[num_block].squares[1].x;
  y = grid->blocks[num_block].squares[1].y;
  b = grid->board[INDEX(x, y, grid->size)];

  if (a > b)
    return ((a % b) == 0);
  else
    return ((b % a) == 0);
}

errcode_t generate(grid_t *grid, int size, int difficulty, bool vrb) {
  if (vrb)
    printf("kenken: verbose: launch generation of %dx%d grid\n\n", size, size);

  errcode_t status;
  int opr;

  /* Variables used for verbose mode. */
  int size_stats[MAX_BLOCK_SIZE] = { 0 };
  int opr_stats[NB_OPERATORS] = { 0 };
  struct timeval start, end;

  if (vrb) gettimeofday(&start, NULL);
  srand(time(NULL) + getpid());

  if ((status = initialize_grid(grid, size))) return status;

  if (vrb) printf("kenken: verbose: fill grid with random values\n");
  fill_values(grid, vrb);
  if (vrb) printf("\n");

  if (vrb) printf("kenken: verbose: split grid in blocks\n");
  fill_block_board(grid, difficulty, vrb);
  if (vrb) printf("kenken: verbose: all blocks have been created\n\n");

  if ((status = initialize_blocks_array(grid, grid->nb_blocks))) return status;

  if ((status = fill_squares_array(grid))) return status;

  if (vrb) printf("kenken: verbose: allocate constraints to blocks\n");
  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    if (vrb) size_stats[grid->blocks[i].nb_squares - 1] += 1;
    if (grid->blocks[i].nb_squares == 1) {
      grid->blocks[i].opr = '+';
      opr = PLUS;
    }
    else {
      if (grid->blocks[i].nb_squares == 2
      && rand() % 100 >= MINUS_DIVIDE_PROBA) {
        if (rand() % 2)
          opr = MINUS;
        else
          opr = DIVIDE;
      }
      else {
        /* 60 because in case DIVIDE is not possible,
         * PLUS is default choice */
        if (rand() % 100 <= MULTIPLY_DEFAULT_PROBA)
          opr = MULTIPLY;
        else
          opr = PLUS;
      }
      switch (opr) {
        case PLUS:
          grid->blocks[i].opr = '+';
        	break;

        case MINUS:
          grid->blocks[i].opr = '-';
        	break;

        case MULTIPLY:
          grid->blocks[i].opr = 'x';
        	break;

        case DIVIDE:
          if ((grid->blocks[i].nb_squares > 2)
          || (! is_divide_possible(grid, i))) {
            grid->blocks[i].opr = '+';
            opr = PLUS;
          }
          else
            grid->blocks[i].opr = '/';
        	break;
      }
    }

    if (vrb) opr_stats[opr] += 1;
    grid->blocks[i].goal = calculate_goal(opr, grid, i) ;

    if (vrb) printf("kenken: verbose: %ld%c allocated to block %ld\n",
    grid->blocks[i].goal, grid->blocks[i].opr, i + 1);
  }
  if (vrb) printf("kenken: verbose: all constraints have been allocated\n\n");

  if (vrb) {
    gettimeofday(&end, NULL);
    printf("kenken: verbose: grid generated in %f sec.\n\n",
    difftime(end.tv_sec, start.tv_sec) + (double)(end.tv_usec - start.tv_usec)
    / DIVIDING_TIME);

    printf("Statistics:\n  %d blocks generated\n", grid->nb_blocks);

    for (size_t i = 0; i < MAX_BLOCK_SIZE; ++i)
      printf("  %d block(s) of size %lu\n", size_stats[i], i + 1);

    for (size_t i = 0; i < NB_OPERATORS; ++i) {
      printf("  %d block(s) with operator ", opr_stats[i]);
      switch(i) {
        case PLUS:
          printf("+\n");
          break;
        case MINUS:
          printf("-\n");
          break;
        case MULTIPLY:
          printf("x\n");
          break;
        case DIVIDE:
          printf("/\n");
          break;
      }
    }
    printf("\n");
  }
  return SUCCESS;
}
