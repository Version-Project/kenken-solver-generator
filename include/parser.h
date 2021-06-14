#ifndef _PARSER_H_
#define _PARSER_H_

#include "grid.h"
#include <stdio.h>

/**
 * \file parser.h
 * \brief Contains the functions used to parse a grid file.
 *
 * There is in fact only one function which will be used in other places.
**/

/**
 * \fn errcode_t parse(grid_t *grid, FILE *fp, size_t *row, char *c)
 * \brief Parses the file passed as parameter and fills the grid.
 *
 * Parses the content of the input file, and saves the information it contains
 * in the grid passed as parameter.
 *
 * \param grid grid to fill.
 * \param fp file pointer which will browse the file.
 * \param row file row where the function stops if an error occurred.
 * \param c unadmitted character whose reading has triggered an error.
 * \return an exit status managed by interpret_parser_error_code().
**/
errcode_t parse(grid_t *grid, FILE *fp, size_t *row, char *c);

#endif /* _PARSER_H_ */
