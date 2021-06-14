#include "solver.h"

#include <math.h>
#include "output.h"

static bool check_plus_case(grid_t *grid, bool optimized, int val,
location_t val_loc, int block_num) {
  int total = val;
  bool is_full = true;
  location_t curr_loc;
  int goal = grid->blocks[block_num].goal;

  /* Optimization rule : if the goal minus the value we try to put is greater
   * than the remaining square of the blocks, then the value is impossible.
   * For example, we cant put 4 in a block of size 3 and of goal 5.  */
  if (optimized
  && (val + (grid->blocks[block_num].nb_squares - 1) * grid->size  < goal || val > goal))
    return false;

  for (size_t i = 0; i < grid->blocks[block_num].nb_squares; ++i) {
    curr_loc = grid->blocks[block_num].squares[i];
    /* Checking if we don't exceed the goal */
    if (grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)] + total <= goal)
        total += grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)];
    else
      return false;
      
    /* Checking if all the squares of the block are filled with a value. If not,
     * the boolean is_full is set to false, which means we won't check if the goal
     * is exactly reached.*/
    if (grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)] == 0
    && (curr_loc.x != val_loc.x || curr_loc.y != val_loc.y))
      is_full = false;
  }

  if (total != goal && is_full) return false;
  if (total <= goal)            return true;

  return false;
}

static bool check_multiply_case(grid_t *grid, bool optimized, int val,
location_t val_loc, int block_num) {
  int total = val;
  bool is_full = true;
  location_t curr_loc;
  int goal = grid->blocks[block_num].goal;

  /* Optimization rule : if the goal is not divisble by the value,
   * then return false */
  if (optimized && goal % val != 0)
    return false;

  for (size_t i = 0; i < grid->blocks[block_num].nb_squares; ++i) {
    curr_loc = grid->blocks[block_num].squares[i];
    if (curr_loc.x != val_loc.x || curr_loc.y != val_loc.y) {
      if (grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)] * total
      <= goal)
          total *= grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)];
      else
        return false;
    }

  if (grid->board[INDEX(curr_loc.x, curr_loc.y, grid->size)] == 0
  && (curr_loc.x != val_loc.x || curr_loc.y != val_loc.y))
    is_full = false;
  }

  if (total != goal && is_full) return false;
  if (total <= goal)            return true;

  return false;
}

static bool check_minus_case(grid_t *grid, bool optimized, int val,
location_t val_loc, int block_num) {
  int min = 0, max = 0;
  bool is_full = true;
  location_t loc1 = grid->blocks[block_num].squares[0];
  location_t loc2 = grid->blocks[block_num].squares[1];
  int val1 = grid->board[INDEX(loc1.x, loc1.y, grid->size)], val2 = 0;
  int goal = grid->blocks[block_num].goal;

  if (optimized && val <= goal && val + goal > grid->size)
    return false;

  if (loc1.x == val_loc.x && loc1.y == val_loc.y) {
    max = (int) fmax(val, val2);
    min = (int) fmin(val, val2);
    if (val2 == 0)
      is_full = false;
  }

  else if (loc2.x == val_loc.x && loc2.y == val_loc.y) {
    max = (int) fmax(val, val1);
    min = (int) fmin(val, val1);
    if (val1 == 0)
      is_full = false;
  }

  if (! is_full)
    return true;
  int total = max - min;

  if (total != goal && is_full) return false;
  if (total >= goal)            return true;

  return true;
}

static bool check_divide_case(grid_t *grid, bool optimized, int val,
location_t val_loc, int block_num) {
  int min = 0, max = 0;
  bool is_full = true;
  location_t loc1 = grid->blocks[block_num].squares[0];
  location_t loc2 = grid->blocks[block_num].squares[1];
  int val1 = grid->board[INDEX(loc1.x, loc1.y, grid->size)];
  int val2 = grid->board[INDEX(loc2.x, loc2.y, grid->size)];
  int goal = grid->blocks[block_num].goal;

  if (optimized && val < goal && val *goal > grid->size)
    return false;

  if (loc1.x == val_loc.x && loc1.y == val_loc.y) {
    max = (int) fmax(val, val2);
    min = (int) fmin(val, val2);
    if (val2 == 0)
      is_full = false;
  }

  else if (loc2.x == val_loc.x && loc2.y == val_loc.y) {
    max = (int) fmax(val, val1);
    min = (int) fmin(val, val1);
    if (val1 == 0)
      is_full = false;
  }

  if (! is_full)
    return true;
  if (max % min != 0)
    return false;
  int total = max / min;

  if (total != goal && is_full) return false;
  return true;
}

bool check_grid(grid_t *grid, bool optimized, int val,
location_t val_loc, int block_num) {
  switch (grid->blocks[block_num].opr) {
    case '+':
    return check_plus_case(grid, optimized, val, val_loc, block_num);

    case 'x':
    return check_multiply_case(grid, optimized, val, val_loc, block_num);

    case '-':
    return check_minus_case(grid, optimized, val, val_loc, block_num);

    case '/':
    return check_divide_case(grid, optimized, val, val_loc, block_num);
  }

  return false;
}
