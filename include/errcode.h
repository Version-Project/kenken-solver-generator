#ifndef _ERRCODE_H_
#define _ERRCODE_H_

#include <stdlib.h>

#define BUFFER_SIZE 8192 /**< Maximum size of any string buffer. */

/**
 * \file errcode.h
 * \brief Contains all the front-end functions used to print error messages.
**/

/**
 * \enum errcode_t
 * \brief Enumeration of all the different error codes a function can return.
**/
typedef enum {
  SUCCESS = 0, /**< success code. */
  ERRCODE_OUT_OF_BOUNDS = 1, /**< out-of-bounds value or index access. */
  ERRCODE_ALLOC = 2, /**< allocation error. */
  ERRCODE_EMPTY_FILE = 3, /**< an empty file is given to the parser. */
  ERRCODE_GRID_SIZE = 4, /**< grid size is not uniform. */
  ERRCODE_NO_BLOCKS = 5, /**< the grid has no block num. */
  ERRCODE_UNADMIT_CHAR = 6, /**< cunadmitted character reading. */
  ERRCODE_CONVERSION_FAIL = 7, /**< string-to-int conversion failure. */
  ERRCODE_UNBOUND_VAL = 8, /**< parser detects a value less than or equal to
  * zero. */
  ERRCODE_MISSING_VAL = 9 /**< abnormal value is read.*/
} errcode_t;

/**
 * \fn void handle_alloc_error()
 * \brief Prints an error message for memory allocation failure and exits the
 * program.
**/
void handle_alloc_error();

/**
 * \fn void interpret_parser_error_code(const errcode_t status,
 * const size_t row, const char c)
 * \brief Takes an error code from the parser, displays the appropriate error
 * message and exits the program.
 *
 * Some errors only occurs during the file parsing, that is why there is a
 * function which interprets only these errors.
 * \param status error code to interpret.
 * \param row input file row where the error occurred (during parsing phase).
 * \param c character which caused the error (during parsing phase).
**/
void interpret_parser_error_code(const errcode_t status, const size_t row,
const char c);

/**
 * \fn void interpret_error_code(const errcode_t status)
 * \brief Takes an error code, displays the appropriate error message and
 * exits the program.
 * \param status error code to interpret.
 */
void interpret_error_code(const errcode_t status);

#endif /* _ERRCODE_H_ */
