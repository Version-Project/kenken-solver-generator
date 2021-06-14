#include "parser.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

/* Checks if the current-reading character is an admitted one,
 * or a character to ignore. If it is not, an error occurs. */
static errcode_t check_admitted_char(char c) {
  if (c != '#' && c != '+' && c != '-' && (c < '/' || c > ':') && c != 'x'
  && c != ' ' && c != '\t' && c != '\n') {
    return ERRCODE_UNADMIT_CHAR;
  }
  return SUCCESS;
}

/* Converts a string to a long, and checks if there has been
 * no failure during the conversion. */
static errcode_t convert_word_to_value(char* word, long *val_long, int *val_int) {
  long val = strtol(word, NULL, 10);
  if (errno == ERANGE) return ERRCODE_UNBOUND_VAL;
  if (errno == EINVAL) return ERRCODE_CONVERSION_FAIL;
  if (val <= 0) return ERRCODE_UNBOUND_VAL;
  if (val_long == NULL) *val_int = val;
  else if (val_int == NULL) *val_long = val;
  return SUCCESS;
}

/* Saves the constraint (goal and operator) get from the read word. */
static errcode_t save_constraint(grid_t *grid, char *word,
int *block_num, char opr) {
  word[strlen(word) - 1] = '\0';
  errcode_t status;
  if ((status =
  convert_word_to_value(word, &grid->blocks[*block_num - 1].goal, NULL)))
    return status;
  grid->blocks[*block_num - 1].opr = opr;
  return SUCCESS;
}

/* Interprets a word from the constrainy part of the input file, and saves the
 * information related to it in the grid_t parameter. */
static errcode_t treat_constraint_word(grid_t *grid, char *word,
int *block_num) {
  size_t length = strlen(word);
  errcode_t status;
  if (word[length - 1] == ':') {
    word[length - 1] = '\0';
    if ((status = convert_word_to_value(word, NULL, block_num)))
      return status;
  }
  else if (word[length - 1] == '+')
    status = save_constraint(grid, word, block_num, '+');
  else if (word[length - 1] == '-')
    status = save_constraint(grid, word, block_num, '-');
  else if (word[length - 1] == 'x')
    status = save_constraint(grid, word, block_num, 'x');
  else if (word[length - 1] == '/')
    status = save_constraint(grid, word, block_num, '/');
  else
    return ERRCODE_UNADMIT_CHAR;

  return status;
}

/* Checks if the read word contains a '#', and returns its position. */
static int check_commentary(char *word) {
  size_t i = 0;
  for (; i < strlen(word); ++i) {
    if (word[i] == '#') {
      return i;
    }
  }
  return i;
}

/* Gets the word read before the commentary character. */
static void get_word_before_commentary(char *in, char *out,
int commentary_position) {
  for (size_t i = 0; i < commentary_position; ++i) {
    out[i] = in[i];
  }
}

/* In function of the actual state (reading room or not), treat the line passed
  as parameter. It use the function strtok to parse each word of the line. */
static errcode_t treat_line(int first_line, char *line, bool is_reading_room,
int row, grid_t *grid, int *count) {
  int word_count = 0, nb_word = 1, commentary_position = 0, block_num = 0;
  errcode_t status;
  char *word ;
  char *prev_word;

  word = strtok(line, " \n");

  while (word != NULL) {
    ++word_count;

    /* For each character of a word, we check if we can recognize it. */
    for (size_t i = 0; i < strlen(word); ++i)
      if ((status = check_admitted_char(word[i])))
        return status;

    /* A special case, when there is a '#' in a word and a value
      before it. Like "12#". */
    if ((commentary_position = check_commentary(word)) != strlen(word)) {
      prev_word = calloc(commentary_position, sizeof(char));
      if (prev_word == NULL) return ERRCODE_ALLOC;

      get_word_before_commentary(word, prev_word, commentary_position);
      if (is_reading_room) {
        if ((status = convert_word_to_value(prev_word, NULL,
        &grid->block_board[INDEX((row - first_line), (nb_word - 1),
        grid->size)]))) {
          return status;
        }
      }
      else {
        if ((status = treat_constraint_word(grid, prev_word, &block_num)))
          return status;
      }

      free(prev_word);
      free(word);
      *count = word_count;
      return SUCCESS;
    }

    /* In normal cases, for the first part of the file we just convert the value
      of the word to long, and put it into the grid. */
    if (is_reading_room) {
      if ((status = convert_word_to_value(word, NULL,
      &grid->block_board[INDEX((row - first_line), (nb_word - 1),
      grid->size)]))) {
        return status;
      }
    }
    /* In case we read the constraints of the file. */
    else {
      if ((status = treat_constraint_word(grid, word, &block_num)))
        return status;
    }

    /* Read next token. */
    word = strtok(NULL, " \n");
    ++nb_word;
  }

  free(word);
  *count = word_count;
  return SUCCESS;
}

/* In this function, we check if a word start by '#' or if it is empty. If yes,
  we skip the line and do it again until we find a treatable word. */
static void skip_useless_lines(int *first_line, FILE *fp, size_t *row,
char *line) {
  int nb_char_to_back = 0;
  while (fgets(line, BUFFER_SIZE, fp) != NULL) {
    nb_char_to_back = strlen(line);
    if (line[0] != '#' && line[0] != '\n'){
      fseek(fp, -nb_char_to_back, SEEK_CUR);
      break;
    }
    ++(*first_line);
    ++(*row);
  }
}

static int get_grid_size(char *line, FILE *fp) {
  size_t size = 0;
  char *word;

  word = strtok(line, " \n");
  while (word != NULL) {
    ++size;
    if (check_commentary(word) != strlen(word)) {
      break;
    }
    word = strtok(NULL, " \n");
  }

  free(word);
  return size;
}


static int get_nb_blocks(grid_t *grid) {
  int actual_max = 0;
  int size = grid->size;
  for (size_t i = 0; i < size * size; ++i) {
    if (actual_max < grid->block_board[i])
      actual_max = grid->block_board[i];
  }
  return actual_max;
}

/* We check if all constraints have been found */
static errcode_t check_getted_values(grid_t *grid) {
  bool *array = calloc(grid->nb_blocks, sizeof(bool));
  if (array == NULL) return ERRCODE_ALLOC;
  int size = grid->size;
  for (size_t i = 0; i < size * size; ++i)
    array[grid->block_board[i] - 1] = true;

  for (size_t i = 0; i < grid->nb_blocks; ++i) {
    if (! array[i]) {
      free(array);
      return ERRCODE_MISSING_VAL;
    }
  }

  free(array);
  return SUCCESS;
}

errcode_t parse(grid_t *grid, FILE *fp, size_t *row, char *c) {
  int size = 0, nb_blocks = 0, skipped_lines = 1;
  errcode_t status;
  bool is_reading_room = true;
  char *line = calloc(BUFFER_SIZE, sizeof(char));
  if (line == NULL) return ERRCODE_ALLOC;

  /* We start by skipping useless lines before we start to really read the file.
  */
  skip_useless_lines(&skipped_lines, fp, row, line);
  size = get_grid_size(line, fp);
  if (size == -1) return ERRCODE_EMPTY_FILE;

  if ((status = initialize_grid(grid, size))) return status;

  int count = 0;
  /* The file must contain same amount a rows than the size of the grid, so we
    skip all useless lines, then we treat a line before going to the next one.
    We do that size times to get the entire grid. */
  for (size_t i = 0; i < size; ++i) {
    skip_useless_lines(&skipped_lines, fp, row, line);
    if (fgets(line, BUFFER_SIZE, fp) == NULL) continue;
    if ((status = treat_line(skipped_lines, line, is_reading_room, *row,
      grid, &count)))
      return status;
    if (is_reading_room && size != count) return ERRCODE_GRID_SIZE;
    ++(*row);
  }

  is_reading_room = false;

  nb_blocks = get_nb_blocks(grid);
  if (nb_blocks == 0) return ERRCODE_NO_BLOCKS;
  if ((status = initialize_blocks_array(grid, nb_blocks))) return status;
  if((status = check_getted_values(grid))) return status;

  /* We proceed same as we do to get the grid, but this time for the
    constraints. */
  for (size_t i = 0; i < nb_blocks; ++i) {
    skip_useless_lines(&skipped_lines, fp, row, line);
    if (fgets(line, BUFFER_SIZE, fp) == NULL) continue;
    if ((status = treat_line(skipped_lines, line, is_reading_room, *row,
    grid, &count))) {
      return status;
    }
    ++(*row);
  }
  free(line);

  return fill_squares_array(grid);
}
