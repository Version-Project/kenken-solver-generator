#include "errcode.h"

#include <stdio.h>

static void print_error(char *message) {
  fprintf(stderr, "kenken: error: %s\n", message);
}

static void print_error_row(char *message, int row) {
  char error_string[BUFFER_SIZE];
  sprintf(error_string, "line %d: %s", row, message);
  print_error(error_string);
}

void handle_alloc_error() {
  print_error("memory allocation of a pointer failed");
  exit(EXIT_FAILURE);
}

void interpret_parser_error_code(const errcode_t status, const size_t row,
const char c) {
  char *error_string = "";
  switch (status) {
    case ERRCODE_OUT_OF_BOUNDS:
      print_error("index out of bounds");
      break;

    case ERRCODE_ALLOC:
      handle_alloc_error();
      break;

    case ERRCODE_EMPTY_FILE:
      print_error("file does not contain any grid");
      break;

    case ERRCODE_GRID_SIZE:
      print_error_row("grid size is not uniform", row);
      break;

    case ERRCODE_NO_BLOCKS:
      print_error("the grid has no block number");
      break;

    case ERRCODE_UNADMIT_CHAR:
      if (c >= 32) {
        sprintf(error_string, "'%c' is an unadmitted character", c);
        print_error_row(error_string, row);
      }
      else
        print_error_row("unadmitted and unprintable character detected", row);
      break;

    case ERRCODE_CONVERSION_FAIL:
      print_error("problem occurred during string-to-int conversion");
      break;

    case ERRCODE_UNBOUND_VAL:
      print_error("negative or null value encountered during file parsing");
      break;

    case ERRCODE_MISSING_VAL:
      print_error("abnormal value detected in the grid during file parsing");
      break;

    default:
      print_error("unrecognized parser error");
      break;
  }
  exit(EXIT_FAILURE);
}

void interpret_error_code(const errcode_t status) {
  switch (status) {
    case ERRCODE_OUT_OF_BOUNDS:
      print_error("value or index out of bounds");
      break;

    case ERRCODE_ALLOC:
      handle_alloc_error();
      break;

    default:
      print_error("unrecognized error");
      break;
  }
  exit(EXIT_FAILURE);
}
