#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "grid.h"
#include <stdio.h>

#define VRB_SET(vrb, nb_set, val, x, y) {\
  if (vrb) {\
    printf("kenken: verbose: set value %d at position (%d,%d)\n", val, x, y);\
    ++(*nb_set);\
  }\
} /**< Function used to display a verbose message when a value is set, and
* increase the number of values set during the program execution. */

#define VRB_UNSET(vrb, nb_unset, val, x, y) {\
  if (vrb) {\
    printf("kenken: verbose:"\
    " unset value %d at position (%d,%d)\n", val, x, y);\
    ++(*nb_unset);\
  }\
} /**< Does the same thing than the VRB_SET function, but for the unset values.
* */

#define VRB_SUBSET(vrb) {\
  if (vrb) printf("kenken: verbose: apply subset rules\n");\
} /**< Function which displays a verbose message for the subset rules
* application. */

/**
 * \file output.h
 * \brief Contains all the front-end functions used to write grids or messages
 * in verbose mode.
 *
 * The solved or generated grids can be write in an output file specified in
 * the command line or in stdout by default.
**/

/**
 * \fn void print_board(grid_t* grid)
 * \brief Used for debug. Prints grid on the standard output.
 * \param grid grid to be printed.
**/
void print_board(grid_t *grid);

/**
 * \fn void print_block_board(grid_t* grid)
 * \brief Used for debug. Prints the grid blocks on the standard output.
 * \param grid grid to be printed.
**/
void print_block_board(grid_t *grid);

/**
 * \fn void print_solution(FILE *output_fp, bool is_solved, const grid_t *grid)
 * \brief Saves the solver result in a specified output file.
 * \param output_fp file pointer where the solution will be saved or written.
 * It could be a specified file or the standard output.
 * \param is_solved 0 if the grid is unsolved, 1 else.
 * \param grid grid to be printed.
**/
void print_solution(FILE *output_fp, bool is_solved, const grid_t *grid);

/**
 * \fn void print_generated_grid(FILE *output_fp, const grid_t* grid)
 * \brief Saves the generated grid and its constraints
 * in a specified output file.
 * \param output_fp ile pointer where the generated grid will be saved/written.
 * It could be a specified file or the standard output.
 * \param grid grid to be printed.
**/
void print_generated_grid(FILE *output_fp, const grid_t* grid);

#endif /* _OUTPUT_H_ */
