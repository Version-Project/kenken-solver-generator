#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "bitmap.h"
#include "grid.h"

#define DEFAULT_ALGORITHM 0 /**< Most efficient algorithm, used by default.*/

/**
 * \file solver.h
 * \brief Contains the functions used to solve a grid, and the algo_t
 * enumeration.
 *
 * There are four different algorithms.
**/

/**
 * \enum algo_t
 * \brief Enumeration of all the different algorithm flags.
**/
typedef enum {
  OPT_BRUTEFORCE = 0, /**< flag corresponding to the backtracking bruteforce
  * algorithm, which has been optimized. */
  BRUTEFORCE = 1, /**< flag corresponding to the simplest backtracking
  * bruteforce algorithm. */
  BLOCKSIZE_SORT = 2, /**< flag corresponding to the algorithm sorting blocks
  * by their size. */
  LOGIC = 3 /**< flag corresponding to the algorithm using logic. */
} algo_t;

/**
 * \fn bool check_grid(grid_t *grid, bool optimized, int val,
 * location_t val_loc, int block_num)
 * \brief Checks if the value we want to set respects its block goal, depending
 * on the block operator.
 *
 * User can choose if he wants to check all the constraints by setting the
 * "optimized" parameter to "true".
 * \param grid grid to solve.
 * \param optimized if "true", use all the optimized conditions.
 * \param val value to set in the grid.
 * \param val_loc location where the value have to be set.
 * \param block_num number of the block.
 * \return "true" if the value can be set, "false" otherwise.
**/
bool check_grid(grid_t *grid, bool optimized, int val, location_t val_loc,
int block_num);

/**
 * \fn bool solve_using_possibilities(grid_t *grid, int *nb_rm_possib,
 * int *nb_hypo, int *nb_set, int *nb_unset, bool vrb)
 * \brief Solves a grid passed as parameter using a backtracking algorithm
 * which sort squares by their number of possibilities.
 *
 * \param grid grid to solve.
 * \param nb_rm_possib (verbose counter) number of initial removed
 * possibilites.
 * \param nb_hypo (verbose counter) number of times the solver make an
 * hypothesis.
 * \param nb_set (verbose counter) number of times the program set a value.
 * \param nb_unset (verbose counter) number of times the program unset a value.
 * \param vrb corresponding to the verbose mode, which is enabled if vrb is
 * "true".
 * \return "true" if the grid has been solved, "false" otherwise.
**/
bool solve_using_possibilities(grid_t *grid, int *nb_rm_possib, int *nb_hypo,
int *nb_set, int *nb_unset, bool vrb);

/**
 * \fn bool solve_using_blocksize_sort(grid_t *grid, int *nb_set,
 * int *nb_unset, bool vrb)
 * \brief Solves a grid passed as parameter using a backtracking algorithm
 * which sort blocks by their size.
 *
 * Based on the algorithm described in the description of
 * solve_using_bruteforce().
 * \param grid grid to solve.
 * \param nb_set (verbose counter) number of times the program set a value.
 * \param nb_unset (verbose counter) number of times the program unset a value.
 * \param vrb corresponding to the verbose mode, which is enabled if vrb is
 * "true".
 * \return "true" if the grid has been solved, "false" otherwise.
**/
bool solve_using_blocksize_sort(grid_t *grid, int *nb_set, int *nb_unset,
bool vrb);

/**
 * \fn bool solve_using_bruteforce(grid_t *grid, bool optimized, int *nb_set,
 * int *nb_unset, bool vrb)
 * \brief Solves a grid passed as parameter using a 'brute-force' backtracking
 * algorithm.
 *
 * Even if it is called 'brute-force', this algorithm checks in any case if
 * the value it wants to put respects the latin squares rule.
 * Based on the algorithm described in this article :
 * <a href="http://www.labri.fr/perso/fleury/courses/pdp/Puzzle_Games/KenKen/
 * KenKen_Puzzle_Solver_using_Backtracking_Algorithm.pdf">
 * KenKen Puzzle Solver using Backtracking Algorithm</a>
 *
 * \param grid grid to solve.
 * \param optimized allow the algorithm to use optimized conditions to skip
 * directly some values that a classic brute-force would try to put.
 * \param nb_set (verbose counter) number of value set in the board.
 * \param nb_unset (verbose counter) number of times the program unset a value
 * in the board.
 * \param vrb corresponding to the verbose mode, which is enabled if vrb is
 * "true".
 * \return "true" if the grid has been solved, "false" otherwise.
**/
bool solve_using_bruteforce(grid_t *grid, bool optimized, int *nb_set,
int *nb_unset, bool vrb);

#endif /* _SOLVER_H_ */
