#include "parser.h"
#include "solver.h"
#include "generator.h"
#include "output.h"

#define FILE1 "../resources/example_grid.ken"
#define HARD_FILE "../resources/generated_grids/10x10_1.ken"

int main(void) {
  errcode_t status;
  grid_t grid;
  size_t row = 1;
  char unadmitted_char;
  FILE* fp = fopen(FILE1, "r");
  if (fp == NULL) {
    fprintf(stderr, "proftest: error: unable to open file '%s'\n", FILE1);
    exit(EXIT_FAILURE);
  }
  if ((status = parse(&grid, fp, &row, &unadmitted_char)))
    interpret_parser_error_code(status, row, unadmitted_char);

  int nb_set = 0, nb_unset = 0, nb_rm_possib = 0, nb_hypo = 0;

  for (int i = 0; i < 100; ++i) {
    solve_using_bruteforce(&grid, false, &nb_set, &nb_unset, false);
    reset_grid(&grid);
    solve_using_bruteforce(&grid, true, &nb_set, &nb_unset, false);
    reset_grid(&grid);
    solve_using_blocksize_sort(&grid, &nb_set, &nb_unset, false);
    reset_grid(&grid);
    solve_using_possibilities(&grid, &nb_rm_possib, &nb_hypo, &nb_set,
    &nb_unset, false);
    reset_grid(&grid);
  }

  fclose(fp);
  clear_grid(&grid);
  return EXIT_SUCCESS;
}
