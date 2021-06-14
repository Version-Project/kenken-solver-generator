#include "output.h"

#include "errcode.h"
#include <string.h>


static void get_string_value(const grid_t *grid, int max_value, int value,
char *valstring) {
  char value_arr[BUFFER_SIZE];
  char max_value_arr[BUFFER_SIZE];

  sprintf(value_arr, "%d", value);
  sprintf(max_value_arr, "%d", max_value);

  for (size_t i = 0 ; i < strlen(max_value_arr) - strlen(value_arr) ; ++i) {
    strcat(valstring, " ");
  }
  strcat(valstring, value_arr);
  strcat(valstring, " ");
}

void print_board(grid_t *grid) {
  size_t size = grid->size;
  char valstring[BUFFER_SIZE] = { 0 };

  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      get_string_value(grid, size, grid->board[INDEX(i, j, size)], valstring);
      printf("%s", valstring);
      memset(valstring, 0, BUFFER_SIZE);
    }
    printf("\n");
  }
}

void print_block_board(grid_t *grid) {
  size_t size = grid->size;
  char valstring[BUFFER_SIZE] = { 0 };

  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      get_string_value(grid, grid->nb_blocks,
      grid->block_board[INDEX(i, j, size)], valstring);
      printf("%s", valstring);
      memset(valstring, 0, BUFFER_SIZE);
    }
    printf("\n");
  }
}

void print_solution(FILE *output_fp, bool is_solved, const grid_t *grid) {
  if (! is_solved)
    fprintf(output_fp, "Grid unsolved.\n");
  else {
    size_t size = grid->size;
    char valstring[BUFFER_SIZE] = { 0 };

    fprintf(output_fp, "Grid solved:\n");

    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        get_string_value(grid, size,
        grid->board[INDEX(i, j, size)], valstring);
        fprintf(output_fp, "%s", valstring);
        memset(valstring, 0, BUFFER_SIZE);
      }
      fprintf(output_fp, "\n");
    }
  }
}

void print_generated_grid(FILE *output_fp, const grid_t *grid) {
  fprintf(output_fp, "#Grid generated:\n# Rooms\n");

  size_t size = grid->size;
  size_t nb_blocks = grid->nb_blocks;
  char valstring[BUFFER_SIZE] = { 0 };

  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      get_string_value(grid, grid->nb_blocks,
      grid->block_board[INDEX(i, j, size)], valstring);
      fprintf(output_fp, "%s", valstring);
      memset(valstring, 0, BUFFER_SIZE);
    }
    fprintf(output_fp, "\n");
  }

  fprintf(output_fp, "\n# Constraints\n");

  for (size_t i = 0; i < nb_blocks; ++i) {
    fprintf(output_fp, "%d: %ld%c\n", (int) i + 1, grid->blocks[i].goal,
    grid->blocks[i].opr);
  }
}
