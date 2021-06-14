SOURCES = grid.c errcode.c bitmap.c parser.c logic_solver.c bruteforce_solver.c solver.c generator.c output.c

KENKEN_EXEC = kenken
KENKEN = $(addsuffix .c, $(KENKEN_EXEC))

HEADERS_DIR = ../include/
DEST_DIR = ../

CFLAGS = -std=c11 -O3 -Wall -g
LDFLAGS = -lm
OMPFLAGS = -fopenmp

all: $(SOURCES) $(KENKEN)
	$(CC) $(CFLAGS) $(OMPFLAGS) -I $(HEADERS_DIR) $(SOURCES) $(KENKEN) -o $(KENKEN_EXEC) $(LDFLAGS)
	mv kenken $(DEST_DIR)

clean:
	rm -f $(DEST_DIR)$(KENKEN_EXEC)
