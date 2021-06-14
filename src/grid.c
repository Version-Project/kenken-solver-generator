#include "grid.h"

#include <string.h>
#include <math.h>

static errcode_t fill_square(grid_t *grid, int x, int y) {
  int block_num = grid->block_board[INDEX(x, y, grid->size)] - 1;
  if (block_num < 0 || block_num >= grid->nb_blocks)
    return ERRCODE_OUT_OF_BOUNDS;

  int square_num = grid->blocks[block_num].nb_squares;
  grid->blocks[block_num].squares[square_num].x = x;
  grid->blocks[block_num].squares[square_num].y = y;
  ++grid->blocks[block_num].nb_squares;
  return SUCCESS;
}

static bool check_row(int row, int num, grid_t *grid) {
  for (size_t col = 0; col < grid->size; ++col) {
    if (grid->board[INDEX(row, col, grid->size)] == num) {
      return false;
    }
  }
  return true;
}

static bool check_col(int col, int num, grid_t *grid) {
  for (size_t row = 0; row < grid->size; ++row) {
    if (grid->board[INDEX(row, col, grid->size)] == num) {
      return false;
    }
  }
  return true;
}

errcode_t initialize_grid(grid_t *grid, int size) {
  grid->size = size;
  grid->board = calloc(size * size, sizeof(int));
  if (grid->board == NULL) return ERRCODE_ALLOC;
  grid->block_board = calloc(size * size, sizeof(int));
  if (grid->block_board == NULL) return ERRCODE_ALLOC;

  return SUCCESS;
}

errcode_t initialize_blocks_array(grid_t *grid, int nb_blocks) {
  grid->nb_blocks = nb_blocks;
  grid->blocks = calloc(nb_blocks, sizeof(block_t));
  if (grid->blocks == NULL) return ERRCODE_ALLOC;

  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    grid->blocks[i].nb_squares = 0;
  }

  return SUCCESS;
}

errcode_t fill_squares_array(grid_t *grid) {
  int squares_array[grid->nb_blocks];
  memset(squares_array, 0, grid->nb_blocks * sizeof(int));
  int block_num = 0;
  for (size_t i = 0; i < grid->size; ++i)
    for (size_t j = 0; j < grid->size; ++j) {
      block_num = grid->block_board[INDEX(i, j, grid->size)] - 1;
      ++squares_array[block_num];
    }
  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    grid->blocks[i].squares = calloc(squares_array[i], sizeof(location_t));
    if (grid->blocks[i].squares == NULL) return ERRCODE_ALLOC;
  }
  for (size_t i = 0; i < grid->size; ++i)
    for (size_t j = 0; j < grid->size; ++j) {
      fill_square(grid, i, j);
    }

  return SUCCESS;
}

void clear_grid(grid_t *grid) {
  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    free(grid->blocks[i].squares);
  }
  free(grid->board);
  free(grid->block_board);
  free(grid->blocks);
}

errcode_t create_and_copy_grid(const grid_t *src_grid, grid_t *dest_grid) {
  errcode_t status;
  if ((status = initialize_grid(dest_grid, src_grid->size)))
    return status;
  if ((status = initialize_blocks_array(dest_grid, src_grid->nb_blocks)))
    return status;

  copy_grid(src_grid, dest_grid);

  return SUCCESS;
}

void copy_grid(const grid_t *src_grid, grid_t *dest_grid) {
  int size = src_grid->size;
  int nb_blocks = src_grid->nb_blocks;
  int nb_squares;
  dest_grid->size = size;
  dest_grid->nb_blocks = src_grid->nb_blocks;

  memcpy(dest_grid->board, src_grid->board, sizeof(int) * size * size);
  memcpy(dest_grid->block_board, src_grid->block_board,
  sizeof(int) * size * size);

  for (size_t i = 0; i < nb_blocks; ++i) {
    nb_squares = src_grid->blocks[i].nb_squares;
    dest_grid->blocks[i].squares = calloc(nb_squares, 2*sizeof(int));
    for (size_t j = 0; j < nb_squares; ++j) {
      dest_grid->blocks[i].squares[j].x = src_grid->blocks[i].squares[j].x;
      dest_grid->blocks[i].squares[j].y = src_grid->blocks[i].squares[j].y;
    }
    dest_grid->blocks[i].nb_squares = nb_squares;
    dest_grid->blocks[i].goal = src_grid->blocks[i].goal;
    dest_grid->blocks[i].opr = src_grid->blocks[i].opr;
  }
}

void reset_grid(grid_t *grid) {
  for (int i = 0 ; i < grid->size * grid->size ; ++i) {
    grid->board[i] = 0;
  }
}

location_t find_empty_square_in_block(grid_t *grid, block_t block) {
  location_t out_location = { .x = -1, .y = -1 };
  int x = -1, y = -1;

  for (size_t i = 0; i < block.nb_squares; ++i) {
    x = block.squares[i].x;
    y = block.squares[i].y;
    if (grid->board[INDEX(x, y, grid->size)] == 0) {
      out_location.x = x;
      out_location.y = y;
      return out_location;
    }
  }
  return out_location;
}

location_t find_empty_square_in_grid(grid_t *grid) {
  location_t out_location = { .x = -1, .y = -1 };

  for (size_t i = 0; i < grid->size; ++i) {
    for (size_t j = 0; j < grid->size; ++j) {
      if (grid->board[INDEX(i, j, grid->size)] == 0) {
        out_location.x = i;
        out_location.y = j;
        return out_location;
      }
    }
  }
  return out_location;
}

bool check_latin_squares_rule(int num, int row, int col, grid_t *grid) {
  return (check_row(row, num, grid) && check_col(col, num, grid));
}
