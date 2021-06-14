#include "solver.h"

#include "output.h"

/* Checks if val is a valid choice for the location val_loc. */
static bool is_valid(grid_t *grid, bool optimized, location_t val_loc,
int val) {
  int x = val_loc.x;
  int y = val_loc.y;
  return (check_grid(grid, optimized, val, val_loc,
  grid->block_board[INDEX(x, y, grid->size)] - 1)
  && check_latin_squares_rule(val, x, y, grid));
}

/* Orders blocks by their size in increasing order,
 * and stocks them in priority_list.  */
static void make_blocks_priority_list (grid_t *grid, block_t *priority_list) {
  int min_size = grid->size;
  int index = -1;

  bool checked_blocks[grid->nb_blocks];
  for (size_t i = 0; i < grid->nb_blocks; ++i)
  checked_blocks[i] = false;

  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    for (size_t j = 0; j < grid->nb_blocks; ++j) {
      if (! checked_blocks[j] && grid->blocks[j].nb_squares < min_size) {
        min_size = grid->blocks[j].nb_squares;
        index = j;
      }
    }

    priority_list[i] = grid->blocks[index];
    checked_blocks[index] = true;
    min_size = grid->size;
  }
}

/* Solves a grid using the blocksize sort method. */
static bool solve_using_blocksize_sort_sub(grid_t *grid,
block_t *priority_list, int *nb_set, int *nb_unset, bool vrb) {
  location_t empty;
  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    empty = find_empty_square_in_block(grid, priority_list[i]);
    if (empty.x != -1 && empty.y != -1) {
      for (size_t val = 1; val <= grid->size; ++val) {
        if (is_valid(grid, true, empty, val)) {
          VRB_SET(vrb, nb_set, (int) val, empty.x, empty.y);
          grid->board[INDEX(empty.x, empty.y, grid->size)] = val;
          if (solve_using_blocksize_sort_sub(grid, priority_list, nb_set,
          nb_unset, vrb)) {
            return true;
          }
          VRB_UNSET(vrb, nb_unset, (int) val, empty.x, empty.y);

          grid->board[INDEX(empty.x, empty.y, grid->size)] = 0;
        }
      }

      return false;
    }
  }
  if (vrb) printf("\n");
  return true;
}

bool solve_using_blocksize_sort(grid_t *grid, int *nb_set, int *nb_unset,
bool vrb) {
  block_t priority_list[grid->nb_blocks];
  if (vrb) printf("kenken: verbose:"
  " make priority list of blocks, sorted by their size\n");
  make_blocks_priority_list(grid, priority_list);
  return solve_using_blocksize_sort_sub(grid, priority_list, nb_set, nb_unset,
  vrb);
}

bool solve_using_bruteforce(grid_t *grid, bool optimized, int *nb_set,
int *nb_unset, bool vrb) {
  location_t empty = find_empty_square_in_grid(grid);
  if (empty.x != -1 && empty.y != -1) {
    for (size_t i = 1; i <= grid->size; ++i) {
      if (is_valid(grid, optimized, empty, i)) {
        VRB_SET(vrb, nb_set, (int) i, empty.x, empty.y);
        grid->board[INDEX(empty.x, empty.y, grid->size)] = i;
        if (solve_using_bruteforce(grid, optimized, nb_set, nb_unset, vrb)) {
          return true;
        }
        VRB_UNSET(vrb, nb_unset, (int) i, empty.x, empty.y);
        grid->board[INDEX(empty.x, empty.y, grid->size)] = 0;
      }
    }
    return false;
  }
  else {
    if (vrb) printf("\n");
    return true;
  }
}
