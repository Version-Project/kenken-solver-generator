#include "parser.h"
#include "solver.h"
#include "generator.h"
#include "output.h"

#define FILE1 "../resources/example_grid_cov.ken"
#define HARD_FILE "../resources/20x20_level5.ken"
#define OUT_FILE "../resources/solutions/outcovtest.sol"

#define GEN_SIZE 20
#define NB_GENERATIONS 50

int main() {
  errcode_t status;
  grid_t grid0, grid1, grid2, grid3, grid4;
  size_t row = 1;
  char unadmitted_char;
  FILE *output_fp = stdout;
  output_fp = fopen(OUT_FILE, "w");
  if (output_fp == NULL) {
    fprintf(stderr, "kenken: error:"
      " program failed to open file \"%s\"\n", OUT_FILE);
    exit(EXIT_FAILURE);
  }

  FILE* fp = fopen(FILE1, "r");
  if (fp == NULL) {
    fprintf(stderr, "covtest: error: unable to open file '%s'\n", FILE1);
    exit(EXIT_FAILURE);
  }
  if ((status = parse(&grid0, fp, &row, &unadmitted_char)))
    interpret_parser_error_code(status, row, unadmitted_char);


  create_and_copy_grid(&grid0, &grid1);
  create_and_copy_grid(&grid0, &grid2);
  create_and_copy_grid(&grid0, &grid3);

  int nb_set = 0, nb_unset = 0, nb_rm_possib = 0, nb_hypo = 0;

  solve_using_bruteforce(&grid0, true, &nb_set, &nb_unset, true);
  solve_using_bruteforce(&grid1, false, &nb_set, &nb_unset, true);
  solve_using_blocksize_sort(&grid2, &nb_set, &nb_unset, true);
  solve_using_possibilities(&grid3, &nb_rm_possib, &nb_hypo, &nb_set,
  &nb_unset, true);
  reset_grid(&grid3);

  for (int i = 0 ; i < NB_GENERATIONS ; ++i){
    generate(&grid4, GEN_SIZE, 5, true);
  }

  print_board(&grid1);
  print_block_board(&grid1);
  print_solution(stdout, 0, &grid1);
  print_solution(output_fp, 1, &grid1);
  print_generated_grid(stdout, &grid4);


  fclose(fp);
  clear_grid(&grid0);
  clear_grid(&grid1);
  clear_grid(&grid2);
  clear_grid(&grid3);
  clear_grid(&grid4);
  return EXIT_SUCCESS;
}
