#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "generator.h"
#include "output.h"
#include "parser.h"
#include "solver.h"

/**
 * \file kenken.c
 * \brief Contains the main() function, used to interpret and manage execution
 * options.
 *
 * All the program features (solver, generator, parser...) are called
 * in the main() function contained in this file.
**/

#define LAST_UPDATE "Apr. 04 2019"
#define SOLVER_VERSION "3.0.1"
#define GENERATOR_VERSION "2.0.0"

#define DIVIDING_TIME 1000000

static void display_help() {
 printf(
   "Usage: kenken [-a|-A ALGO|-o FILE|-n|-v|-V|-h] FILE...\n"
   "       kenken -g[SIZE] [-u|-d LVL|-o FILE|-n|-v|-V|-h]\n"
   "Solve or generate kenken grids of variable size\n\n"
   " -g[N],--generate[=N]      generate a grid of size NxN (default:6x6)\n"
   " -a,--all                  search for all possible solutions\n"
   " -u,--unique               generate a grid with unique solution\n"
   " -d LVL,--difficulty LVL   set the difficulty of the grid to LVL (1..10)\n"
   " -A ALGO,--algorithm ALGO  solve a grid using the ALGO algorithm\n"
   " -o FILE,--output FILE     write result to FILE\n"
   " -n,--no-display           do not print result in the standard output\n"
   " -v,--verbose              verbose output\n"
   " -V,--version              display version and exit\n"
   " -h,--help                 display this help and exit\n\n"
 );
}

static void display_version(char *date, char *solver_v, char *generator_v) {
 printf(
   "kenken (last update: %s)\n"
   "  Solver version   : %s\n"
   "  Generator version: %s\n",
   date, solver_v, generator_v
 );
}

static void display_algorithms_list() {
 printf(
   "If you do not use the -- 'algorithm' option to solve, the fastest one will"
   " be used by default.\n\nThe following algorithm flags can be used:\n"
   "  0,OPT_BRUTEFORCE  optimized backtracking bruteforce algorithm\n"
   "  1,BRUTEFORCE      classic backtracking bruteforce algorithm\n"
   "  2,BLOCKSIZE_SORT  backtracking algorithm sorting blocks by size\n"
   "  3,LOGIC           backtracking algorithm using logic\n\n"
 );
}

/* Returns the algo_t corresponding to the argument passed with the
 * '--algorithm' option. */
static algo_t get_algorithm(char *flag) {
 if (! strcmp(flag, "OPT_BRUTEFORCE") || ! strcmp(flag, "0"))
   return OPT_BRUTEFORCE;
 if (! strcmp(flag, "BRUTEFORCE") || ! strcmp(flag, "1"))
   return BRUTEFORCE;
 if (! strcmp(flag, "BLOCKSIZE_SORT") || ! strcmp(flag, "2"))
   return BLOCKSIZE_SORT;
 if (! strcmp(flag, "LOGIC") || ! strcmp(flag, "3"))
   return LOGIC;
 fprintf(stderr, "kenken: error: invalid algorithm flag '%s'\n", flag);
 display_algorithms_list();
 exit(EXIT_FAILURE);
}

/* Converts a string to an integer by checking all potential errors. */
static int convert_word_to_int_frontend(char* word) {
  int result = strtol(word, NULL, 10);
  if (errno == ERANGE || errno == EINVAL) {
    fprintf(stderr, "kenken: error: problem occured during conversion"
    " of the word \"%s\" to an integer\n", word);
    exit(EXIT_FAILURE);
  }
  return result;
}

/* Solves a grid passed as parameter with a specified algorithm. */
static bool choose_solver_algorithm(grid_t *grid, algo_t algorithm, bool vrb) {

  /* Checks if the grid is already solved. */
  location_t empty_square = find_empty_square_in_grid(grid);
  if (empty_square.x == -1 && empty_square.y == -1)
    return true;

  /* Variables used for verbose mode. */
  int nb_set = 0, nb_unset = 0, nb_rm_possib = 0, nb_hypo = 0;
  struct timeval start, end;
  if (vrb) gettimeofday(&start, NULL);

  bool is_solved = false;

  switch (algorithm) {
    case LOGIC:
      if (grid->size > sizeof(bitmap_t) * CHAR_BIT) {
        fprintf(stderr, "kenken: error: logic solver can not accept grids of"
        " size greater than %lu\n", sizeof(bitmap_t) * CHAR_BIT);
        exit(EXIT_FAILURE);
      }
      if (vrb) printf("kenken: verbose: launch logic solver\n");
      is_solved = solve_using_possibilities(grid, &nb_rm_possib, &nb_hypo,
      &nb_set, &nb_unset, vrb);
      break;

    case BLOCKSIZE_SORT:
      if (vrb) printf("kenken: verbose: launch block-size sort solver\n");
      is_solved = solve_using_blocksize_sort(grid, &nb_set, &nb_unset, vrb);
      break;

    case OPT_BRUTEFORCE:
      if (vrb) printf("kenken: verbose: launch optimized bruteforce solver\n");
      is_solved = solve_using_bruteforce(grid, true, &nb_set, &nb_unset, vrb);
      break;

    case BRUTEFORCE:
      if (vrb) printf("kenken: verbose: launch classic bruteforce solver\n");
      is_solved = solve_using_bruteforce(grid, false, &nb_set, &nb_unset, vrb);
      break;

    default:
      break;
  }

  if (vrb) {
     gettimeofday(&end, NULL);
     printf("\nkenken: verbose: grid solved in %f sec.\n\n",
     difftime(end.tv_sec, start.tv_sec) + (double)(end.tv_usec - start.tv_usec)
     / DIVIDING_TIME);

     printf("Statistics:\n  %d value(s) set\n  %d value(s) unset\n",
     nb_set, nb_unset);

     if (algorithm == LOGIC) {
       printf("  %d possibility(ies) removed before calling the backtracking"
       " function\n  %d hypothesis made during the backtracking resolution"
       "\n\n", nb_rm_possib, nb_hypo);
     }
     else printf ("\n");
  }
  return is_solved;
}

/**
 * \fn int main(int argc, char** argv)
 * \brief Manages the operands using the getopt.h library.
 *
 * In a first time, it reads all the options with the getopt_long() function,
 * then it interprets those options and launch the appropriate features.
 * \param argc number of program arguments.
 * \param argv list of program arguments.
 * \return an exit status.
**/
int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "kenken: missing operand\n"
    "Try './kenken --help' for more information.\n");
    exit(EXIT_FAILURE);
  }

  errcode_t status;
  size_t size = DEFAULT_SIZE;
  algo_t algorithm = DEFAULT_ALGORITHM;
  FILE *output_fp = stdout;
  char *solved_file = calloc(BUFFER_SIZE, sizeof(char));
  if (solved_file == NULL) handle_alloc_error();

  bool generator_mode = false, unique_mode = false, difficulty_mode = false,
  verbose_mode = false, all_mode = false, display_mode = true;

  int val, option_index, difficulty_level = DEFAULT_DIFFICULTY;
  opterr = 0;
  static struct option long_options[] = {
    { "generate",   optional_argument, NULL, 'g' },
    { "all",        no_argument,       NULL, 'a' },
    { "algorithm",  required_argument, NULL, 'A' },
    { "unique",     no_argument,       NULL, 'u' },
    { "difficulty", required_argument, NULL, 'd' },
    { "output",     required_argument, NULL, 'o' },
    { "no-display", no_argument,       NULL, 'n' },
    { "verbose",    no_argument,       NULL, 'v' },
    { "version",    no_argument,       NULL, 'V' },
    { "help",       no_argument,       NULL, 'h' },
    { NULL,         0,                 NULL,  0  }
  };

  /* Get all the options and put the appropriate modes at "true". */
  while ((val = getopt_long(argc, argv, "g::aA:ud:o:nvVh",
   long_options, &option_index)) != -1) {
    option_index = 0;

    switch (val) {
      case 'g':
        generator_mode = true;
        if (optarg != 0) {
          size = convert_word_to_int_frontend(optarg);
        }
        if (size < 2) {
          size = DEFAULT_SIZE;
        }
        if (size >= MAX_GEN_GRID_SIZE) {
          fprintf(stderr, "kenken: error: grid size must be smaller than %d\n",
          MAX_GEN_GRID_SIZE);
          exit(EXIT_FAILURE);
        }
        if (size >= MAX_STABLE_GEN_GRID_SIZE) {
          fprintf(stderr, "kenken: warning: generate a grid with a size"
          " greater than %d could bring some memory problems\n",
          MAX_STABLE_GEN_GRID_SIZE);
        }
        break;

      case 'a':
        all_mode = true;
        break;

      case 'A':
        algorithm = get_algorithm(optarg);
        break;

      case 'u':
        unique_mode = true;
        break;

      case 'd':
        difficulty_mode = true;
        if (optarg != 0) {
          difficulty_level = convert_word_to_int_frontend(optarg);
        }
        if ((difficulty_level > 10) || (difficulty_level < 1)) {
          fprintf(stderr, "kenken: error:"
          " invalid difficulty level '%s'\n"
          "Difficulty level of the generated grid must be between 1 and 10.\n",
          optarg);
          exit(EXIT_FAILURE);
        }
        break;

      case 'n':
        display_mode = false;
        break;

      case 'o':
        output_fp = fopen(optarg, "w");
        if (output_fp == NULL) {
          fprintf(stderr, "kenken: error:"
            " program failed to open file \"%s\"\n", optarg);
          exit(EXIT_FAILURE);
        }
        break;

      case 'v':
        verbose_mode = true;
        break;

      case 'V':
        display_version(LAST_UPDATE, SOLVER_VERSION, GENERATOR_VERSION);
        free(solved_file);
        return EXIT_SUCCESS;

      case 'h':
        display_help();
        printf("\n");
        display_algorithms_list();
        free(solved_file);
        return EXIT_SUCCESS;

      /* In case an option required a parameter which is not given... */
      case '?':
        switch (optopt) {
          case 'A':
            fprintf(stderr, "kenken: error: name of the algorithm to use is"
            " missing\nYou have to specify an algorithm flag.\n");
            display_algorithms_list();
            exit(EXIT_FAILURE);

          case 'd':
            fprintf(stderr, "kenken: error: difficulty level is not"
            " specified\nDifficulty level of the generated grid must be"
            " between 1 and 10.\n");
            exit(EXIT_FAILURE);

          case 'o':
            fprintf(stderr, "kenken: error: missing file operand\n"
            "-- 'output' option requires a file operand where output the"
            " generated or solved grid.\nTry './kenken --help' for more"
            " information.\n");
            exit(EXIT_FAILURE);

          default:
            fprintf(stderr, "kenken: error: invalid operand\n"
            "Try './kenken --help' for more information.\n");
            exit(EXIT_FAILURE);
        }

      default:
        fprintf(stderr, "kenken: error: invalid operand\n"
        "Try './kenken --help' for more information.\n");
        exit(EXIT_FAILURE);
    }
  }

  /* Time to interpret! At first, we have to handle the options error cases. */
  if (! generator_mode) {
    if (unique_mode) {
      fprintf(stderr, "kenken: error: invalid operand -- 'unique'\n"
      "This operand should be used with -- 'generate'.\n"
      "Try './kenken --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
    if (difficulty_mode) {
      fprintf(stderr, "kenken: error: invalid operand -- 'difficulty'\n"
      "This operand should be used with -- 'generate'.\n"
      "Try './kenken --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
    if (optind >= argc) {
      fprintf(stderr, "kenken: error: missing file operand\n"
      "Please specify a file to solve.\n"
      "Try './kenken --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < strlen(argv[optind]); ++i) {
      solved_file[i] = argv[optind][i];
    }
  }
  else {
    if (all_mode) {
      fprintf(stderr, "kenken: error: invalid operand -- 'all'\n"
      "This operand should not be used with -- 'generate'.\n"
      "Try './kenken --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
  }

  grid_t grid;

  /* Now we can treat all the generator mode options. */
  if (generator_mode) {
    if (unique_mode) {
      printf("kenken: for now, the program can not generate a grid with an"
      " unique solution\n");
      status = generate(&grid, size, difficulty_level, verbose_mode);
    }
    else {
      status = generate(&grid, size, difficulty_level, verbose_mode);
    }

    if (status) interpret_error_code(status);
    if (output_fp != stdout)
      print_generated_grid(output_fp, &grid);
    else if (display_mode)
      print_generated_grid(stdout, &grid);

    clear_grid(&grid);
  }

  /* In this case, we are in a "solver mode", and we need to parse a file. */
  else {
    FILE *fp = fopen(solved_file, "r");
    if (fp == NULL) {
      fprintf(stderr, "kenken: error: unable to open file '%s'\n",
      solved_file);
      free(solved_file);
      exit(EXIT_FAILURE);
    }

    size_t row = 1;
    char unadmitted_char;
    if ((status = parse(&grid, fp, &row, &unadmitted_char))) {
      free(solved_file);
      fclose(fp);
      if (output_fp != stdout) fclose(output_fp);
      interpret_parser_error_code(status, row, unadmitted_char);
    }
    bool is_solved = false;

    if (all_mode) {
      printf("kenken: for now, the program can not search for all the possible"
      " solutions of a grid\n");
      is_solved = choose_solver_algorithm(&grid, algorithm, verbose_mode);
    }
    else {
      is_solved = choose_solver_algorithm(&grid, algorithm, verbose_mode);
    }

    if (output_fp != stdout)
      print_solution(output_fp, is_solved, &grid);
    else if (display_mode)
      print_solution(output_fp, is_solved, &grid);

    free(solved_file);
    clear_grid(&grid);
    fclose(fp);
    if (output_fp != stdout) fclose(output_fp);
  }

  return EXIT_SUCCESS;
}
