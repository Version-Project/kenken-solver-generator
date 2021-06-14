#include "parser.h"
#include "solver.h"
#include "generator.h"
#include "output.h"

#define OUT_FILE "../resources/solutions/outproftest.sol"

#define GEN_SIZE 200
#define NB_GENERATIONS 100

int main(void) {
  grid_t grid;
  FILE *output_fp = stdout;
  output_fp = fopen(OUT_FILE, "w");
  if (output_fp == NULL) {
    fprintf(stderr, "proftest: error:"
      " program failed to open file \"%s\"\n", OUT_FILE);
    exit(EXIT_FAILURE);
  }

  for (int i = 0 ; i < NB_GENERATIONS ; ++i){
    generate(&grid, GEN_SIZE, 5, false);
  }

  fclose(output_fp);
  return EXIT_SUCCESS;
}
