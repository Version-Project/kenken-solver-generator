#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "grid.h"

#define DEFAULT_DIFFICULTY 5 /**< Difficulty level of the generated grids when
* the '-d' flag is not used. */
#define MAX_GEN_GRID_SIZE 1447 /**< Maximum size of a generated grid */
#define MAX_STABLE_GEN_GRID_SIZE 708 /**< Maximum size for a stable grid
* generation */

/**
 * \file generator.h
 * \brief Contains the function used to generate grids.
**/

/**
 * \fn errcode_t generate(grid_t* grid, int size, int difficulty, bool vrb)
 * \brief Generates grids with specified size and/or difficulty.
 * \param grid grid to be generated.
 * \param size size of the grid.
 * \param difficulty difficulty of the grid, 1 corresponding to the easiest
 * difficulty for a grid, and 10 to the hardest.
 * \param vrb corresponding to the verbose mode, which is enabled if vrb is
  "true".
**/
errcode_t generate(grid_t* grid, int size, int difficulty, bool vrb);

#endif /* _GENERATOR_H_ */
