#ifndef _GRID_H_
#define _GRID_H_

#include <stdbool.h>

#include "errcode.h"

#define INDEX(i, j, size) (i * size + j) /**< Function used to get the index of
* an one-dimension board array with the x & y coordinates of the square on the
* board. */
#define DEFAULT_SIZE 6 /**< Default size of a grid. */

/**
 * \file grid.h
 * \brief Contains grid structure and its initialization functions.
 *
 * The structured type grid_t is used in any other file of the project and the
 * initialization functions are used to initialize a grid_t in several steps.
**/

/**
 * \struct location_t
 * \brief Represents the location of a square on a KenKen grid.
**/
typedef struct {
  int x; /**< Position of the square on the x-axis (start from the left). */
  int y; /**< Position of the square on the y-axis (start from the top). */
} location_t;

/**
 * \struct block_t
 * \brief Represents a block in KenKen and its different features.
**/
typedef struct {
  location_t *squares; /**< array which contains the different squares
  * of the block. */
  long goal; /**< value to reach with the squares values and the operator. */
  int nb_squares; /**< number of squares the block contains. */
  char opr; /**< operator which will be used to compute the goal. */
} block_t;

/**
 * \struct grid_t
 * \brief Represents a KenKen grid and contains all the things that will be
 * helpful for the solver or the generator.
**/
typedef struct {
  block_t *blocks; /**< array of all the different blocks of the grid. */
  int *block_board; /**< board containing the block number of each square. */
  int *board; /**< board containing the current values of each square. */
  int nb_blocks; /**< number of blocks the grid contains. */
  int size; /**< size of the grid. */
} grid_t;

/**
 * \fn errcode_t initialize_grid(grid_t* grid, const int size)
 * \brief Initializes all the grid arrays which depend of its size.
 * \param grid grid to initialize.
 * \param size grid size, used to create some arrays of the structure.
 * \return an exit status managed by interpret_error_code().
**/
errcode_t initialize_grid(grid_t* grid, const int size);

/**
 * \fn errcode_t initialize_blocks_array(grid_t* grid, const int nb_blocks)
 * \brief Initializes the blocks array with its size nb_blocks.
 * \param grid grid to initialize.
 * \param nb_blocks number of blocks in the grid, and size of blocks array.
 * \return an exit status managed by interpret_error_code().
**/
errcode_t initialize_blocks_array(grid_t* grid, const int nb_blocks);

/**
 * \fn errcode_t fill_squares_array(grid_t* grid)
 * \brief Fills the squares array of each block with the information stored in
 * the block board of the grid.
 * \param grid grid whose squares arrays will be fill.
 * \return an exit status managed by interpret_error_code().
**/
errcode_t fill_squares_array(grid_t* grid);

/**
 * \fn void clear_grid(grid_t* grid)
 * \brief Frees memory space of the grid passed as parameter.
 * \param grid grid to clear.
**/
void clear_grid(grid_t* grid);

/**
 * \fn errcode_t create_and_copy_grid(const grid_t *src_grid,
 * grid_t *dest_grid)
 * \brief Initializes all grids arrays of dest_grid, then copies src_grid to
 * it.
 * \param src_grid grid to copy.
 * \param dest_grid grid to create, and which then will be the destination of
 * the copy.
 * \return an exit status managed by interpret_error_code().
**/
errcode_t create_and_copy_grid(const grid_t *src_grid, grid_t *dest_grid);

/**
 * \fn void copy_grid(const grid_t *src_grid, grid_t *dest_grid)
 * \brief Copies content of src_grid to dest_grid.
 * \param src_grid grid to copy.
 * \param dest_grid destination of the copy.
**/
void copy_grid(const grid_t *src_grid, grid_t *dest_grid);

/**
 * \fn void reset_grid(grid_t *grid)
 * \brief Resets all the board values.
 * \param grid grid which has the board to reset.
**/
void reset_grid(grid_t *grid);

/**
 * \fn location_t find_empty_square_in_block(grid_t *grid, block_t block)
 * \brief Finds the next empty square in a block.
 * \param grid grid used to check if the square we found is really empty.
 * \param block block in which we want to find an empty square.
 * \return if an empty square remains, it returns the first one.
 * Otherwise, the returned empty square x and y coordinates will be -1.
**/
location_t find_empty_square_in_block(grid_t *grid, block_t block);

/**
 * \fn location_t find_empty_square_in_grid(grid_t *grid)
 * \brief Finds the next empty square of the grid.
 * \param grid grid in which we want to find an empty square.
 * \return if an empty square remains, it returns the first one.
 * Otherwise, the returned empty square x and y coordinates will be -1.
**/
location_t find_empty_square_in_grid(grid_t *grid);

/**
 * \fn bool check_latin_squares_rule(int num, const int row, const int col,
 * grid_t *grid)
 * \brief Checks if the number we want to put in the square follows
 * the latin squares rule.
 * \param grid grid in which we want to put a number.
 * \param num number we want to put in the square.
 * \param row y-axis coordinate of the square we want to fill.
 * \param col x-axis coordinate of the square we want to fill.
 * \return "true" if the number can be put in the square, "false" otherwise.
**/
bool check_latin_squares_rule(int num, const int row, const int col,
grid_t *grid);

#endif /* _GRID_H_ */
