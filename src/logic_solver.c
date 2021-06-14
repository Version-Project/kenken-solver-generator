#include "solver.h"
#include "omp.h"

#include "output.h"

#define MIN_SUBSET_SIZE 2
#define MAX_SUBSET_SIZE 2

/* Initializes the bitmap with all the bits set for each square. */
static void make_possib_grid(grid_t *grid, bitmap_t *possib_grid) {
  for (size_t i = 0; i < grid->size; ++i) {
    for (size_t j = 0; j < grid->size; ++j) {
      possib_grid[INDEX(i, j, grid->size)] = 0 << (sizeof(bitmap_t) - 1);
      for (size_t k = 0; k < grid->size; ++k) {
        possib_grid[INDEX(i, j, grid->size)] =
        set_bit(possib_grid[INDEX(i, j, grid->size)], k);
      }
    }
  }
}

/* Returns the first square position which has at least one possibility left,
 * and stores the value at this position in the val variable. */
static int find_next_possibility_pos(grid_t *grid, bitmap_t *possib_grid,
int *val) {
  for (int i = 0; i < grid->size; ++i) {
    for (int j = 0; j < grid->size; ++j) {
      if (get_set_bits_number(possib_grid[INDEX(i, j, grid->size)]) != 0) {
        *val = get_rightmost_set_bit(possib_grid[INDEX(i, j, grid->size)]);
        return INDEX(i, j, grid->size);
      }
    }
  }
  return -1;
}

/* Does the same thing that the previous function, but returns the first square
 * position with just a single possibility. */
static int find_single_possibility_pos(grid_t *grid, bitmap_t *possib_grid,
int *val) {
  for (size_t i = 0; i < grid->size; ++i) {
    for (size_t j = 0; j < grid->size; ++j) {
      if (get_set_bits_number(possib_grid[INDEX(i, j, grid->size)]) == 1) {
        *val = get_rightmost_set_bit(possib_grid[INDEX(i, j, grid->size)]);
        return INDEX(i, j, grid->size);
      }
    }
  }
  return -1;
}

/* Removes the initial impossibles possibilites of the grid's blocks. */
static void remove_impossible_possibilities(grid_t *grid,
bitmap_t *possib_grid, int *nb_rm_possib, bool vrb) {
  int x, y;
  for (int i = 0; i < grid->nb_blocks; ++i) {
    for (int j = 0; j < grid->blocks[i].nb_squares; ++j) {
      x = grid->blocks[i].squares[j].x;
      y = grid->blocks[i].squares[j].y;

      for (int num = 1; num <= grid->size; ++num) {
        switch (grid->blocks[i].opr) {

          /* If the goal less the value we want to put is greater than the
           * remaining square(s) of the block, then the value is not a
           * possible choice. For instance, we can't put the value 4 in a
           * block of size 3 and of goal 5. */
          case '+':
            if (num > grid->blocks[i].goal
            || num  + ((grid->blocks[i].nb_squares - 1) * grid->size)
            < grid->blocks[i].goal
            || num + grid->blocks[i].nb_squares - 1 > grid->blocks[i].goal) {
              possib_grid[INDEX(x, y, grid->size)] =
              unset_bit(possib_grid[INDEX(x, y, grid->size)], num - 1);
            }
            break;

          /* Checks if the goal can be divide by the value we want to set. */
          case 'x':
            if ((grid->blocks[i].goal% num) != 0) {
              possib_grid[INDEX(x, y, grid->size)] =
              unset_bit(possib_grid[INDEX(x, y, grid->size)], num - 1);
            }
            break;

          /* See the divide explaination to understand this optimization. */
          case '-':
            if (num <= grid->blocks[i].goal
            && num + grid->blocks[i].goal > grid->size) {
              possib_grid[INDEX(x, y, grid->size)] =
              unset_bit(possib_grid[INDEX(x, y, grid->size)], num - 1);
            }
            break;

          /* If the number we want to put in the square is lower than the
           * block's goal, then it is the denominator. If the denominator
           * multiplied by the goal is greater than the grid size, it means
           * that the numerator is greater than the grid size and its
           * impossible, so we remove the number num of the possibilities. */
          case '/':
            if (num < grid->blocks[i].goal
            && num * grid->blocks[i].goal > grid->size) {
              possib_grid[INDEX(x, y, grid->size)] =
              unset_bit(possib_grid[INDEX(x, y, grid->size)], num - 1);
            }
            break;
        }
        if (vrb) ++(*nb_rm_possib);
      }
    }
  }
}

/* Removes the value at the position passed as parameter from all the squares
 * on its line and column. Equal to the naked subset rule for a subset of size
 * 1. */
static void apply_latin_squares_rule(grid_t *grid, bitmap_t *possib_grid,
int position) {
  size_t x = position / grid->size;
  size_t y = position % grid->size;
  int val = grid->board[position];
  possib_grid[position] &= ~possib_grid[position];
  for (size_t i = 0; i < grid->size; ++i) {
    possib_grid[INDEX(x, i, grid->size)] =
    unset_bit(possib_grid[INDEX(x, i, grid->size)], val - 1);
    possib_grid[INDEX(i, y, grid->size)] =
    unset_bit(possib_grid[INDEX(i, y, grid->size)], val - 1);
  }
}

/* Applies the naked subset rule, and return "true" if at least one possibility
 * changed. */
static void apply_naked_subset_rule(grid_t *grid, bitmap_t *possib_grid,
size_t subset_size) {
  bitmap_t naked_subset, reverted_subset;
  int cnt = 1; /* Counter for the number of subset we meet. */
  for (size_t row = 0; row < grid->size; ++row) {
    for (size_t col = 0; col < grid->size; ++col) {
      naked_subset = possib_grid[INDEX(row, col, grid->size)];
      reverted_subset = ~naked_subset;
      if (get_set_bits_number(naked_subset) == subset_size) {
        cnt = 1;
        for (size_t i = col + 1; i < grid->size; ++i) {
          if (naked_subset == possib_grid[INDEX(row, i, grid->size)]
          && ++cnt == subset_size) {
            /* Remove all the possibilities in the squares that are not in the
            * naked subset in this row. */
            for (size_t j = 0; j < grid->size; ++j) {
              if (j != col && j != i)
                possib_grid[INDEX(row, j, grid->size)] &= reverted_subset;
            }
          }
        }
        cnt = 1;
        for (size_t i = row + 1; i < grid->size; ++i) {
          if (naked_subset == possib_grid[INDEX(i, col, grid->size)]
          && ++cnt == subset_size) {
            /* Remove all the possibilities in the squares that are not in
             * the naked subset in this column. */
            for (int j = 0; j < grid->size; ++j) {
              if (j != row && j != i)
                possib_grid[INDEX(j, col, grid->size)] &= reverted_subset;
            }
          }
        }
      }
    }
  }
}

/* Recursive function of the logic solver. */
static bool solve_using_possibilities_sub(grid_t *grid, bitmap_t *possib_grid,
int position, int value, int *nb_hypo, int *nb_set, int *nb_unset, bool vrb) {

  location_t empty_square, val_square;
  val_square.x = position / grid->size;
  val_square.y = position % grid->size;
  int curr_pos, curr_val;

  /* Since the grid is passed by pointer at each function call, we need to
   * create a copy of this grid to allow the backtracking of the possibilities.
   * There is in fact no problem to undo the actions made on the grid, but that
   * is not so simple with the possibilites array. */
  if (! check_grid(grid, true, value, val_square,
  grid->block_board[position] - 1)) {
    return false;
  }

  bitmap_t possib_copy[grid->size * grid->size];
  for (int i = 0; i < grid->size * grid->size; ++i)
    possib_copy[i] = possib_grid[i];
  VRB_SET(vrb, nb_set, value, position / grid->size, position % grid->size);
  grid->board[position] = value;

  VRB_SUBSET(vrb);
  apply_latin_squares_rule(grid, possib_copy, position);
  for (size_t i = MIN_SUBSET_SIZE; i <= MAX_SUBSET_SIZE; ++i) {
    apply_naked_subset_rule(grid, possib_copy, i);
  }

  /* If there is a square with a single possibility, we call recursively the
   * function with the position of this square, and the value to set in it. */
  if ((curr_pos =
  find_single_possibility_pos(grid, possib_copy, &curr_val)) != -1) {
    if (solve_using_possibilities_sub(grid, possib_copy, curr_pos,
    curr_val, nb_hypo, nb_set, nb_unset, vrb))
      return true;
  }

  else {
    /* If there is no more possibilities nor empty squares, the grid is
     * complete, so we return true. In the other case, we return false. */
    if ((curr_pos =
    find_next_possibility_pos(grid, possib_copy, &curr_val)) == -1) {
      empty_square = find_empty_square_in_grid(grid);
      if (empty_square.x == -1 && empty_square.y == -1)
        return true;
    }
    /* Otherwise, if there are possibilites left, we are going to try each of
     * these possibilites and make hypothesis. */
    else {
      while ((curr_val = get_rightmost_set_bit(possib_copy[curr_pos])) != 0) {
        if (vrb) ++(*nb_hypo);
        if (solve_using_possibilities_sub(grid, possib_copy, curr_pos,
        curr_val, nb_hypo, nb_set, nb_unset, vrb)) {
          return true;
        }
        possib_copy[curr_pos] = unset_bit(possib_copy[curr_pos], curr_val - 1);
        VRB_UNSET(vrb, nb_unset, grid->board[curr_pos], curr_pos / grid->size,
        curr_pos % grid->size);
        grid->board[curr_pos] = 0;
      }
    }
  }
  VRB_UNSET(vrb, nb_unset, grid->board[curr_pos], curr_pos / grid->size,
  curr_pos % grid->size);

  grid->board[curr_pos] = 0;
  return false;
}

bool solve_using_possibilities(grid_t *grid, int *nb_rm_possib, int *nb_hypo,
int *nb_set, int *nb_unset, bool vrb) {

  bitmap_t possib_grid[grid->size * grid->size];
  make_possib_grid(grid, possib_grid);

  remove_impossible_possibilities(grid, possib_grid, nb_rm_possib, vrb);
  apply_naked_subset_rule(grid, possib_grid, 2);

  int value = -1, position = -1;
  bool is_solved = false;

  /* At first, we try to find a square with only one possibility.
   * In the other case, we search the first square with at least two
   * possibilities, and we'll run the recursive solver function by testing
   * all these square values. */
  if ((position =
  find_single_possibility_pos(grid, possib_grid, &value)) == -1) {
    position = find_next_possibility_pos(grid, possib_grid, &value);
    while (! is_solved) {
      if (vrb) ++(*nb_hypo);
      find_next_possibility_pos(grid, possib_grid, &value);
      if ((is_solved = solve_using_possibilities_sub(grid, possib_grid,
      position, value, nb_hypo, nb_set, nb_unset, vrb))) {
        return is_solved;
      }
      possib_grid[position] = unset_bit(possib_grid[position], value - 1);
    }
  }
  else {
    is_solved = solve_using_possibilities_sub(grid, possib_grid, position,
    value, nb_hypo, nb_set, nb_unset, vrb);
  }
  return is_solved;
}
