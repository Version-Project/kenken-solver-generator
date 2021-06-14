SRC_NAMES = grid bitmap parser logic_solver bruteforce_solver solver generator output
ERRCODE = ../src/errcode.c


SRC_DIR = ../src/
HEADERS_DIR = ../include/

SRC_FILES = $(addsuffix .c, $(SRC_NAMES))
SOURCES = $(addprefix $(SRC_DIR), $(SRC_FILES))

COVTEST_EXEC = covtest
COVTEST = $(addsuffix .c, $(COVTEST_EXEC))

PROFTEST_SOLVER_EXEC = proftest_solvers
PROFTEST_SOLVER_BRUTEFORCE_EXEC = proftest_solver_bruteforce
PROFTEST_SOLVER_OPT_BRUTEFORCE_EXEC = proftest_solver_opt_bruteforce
PROFTEST_SOLVER_BLOCKSIZE_SORT_EXEC = proftest_solver_blocksize_sort
PROFTEST_SOLVER_LOGIC_EXEC = proftest_solver_logic
PROFTEST_GENERATOR_EXEC = proftest_generator

PROFTEST_SOLVER = $(addsuffix .c, $(PROFTEST_SOLVER_EXEC))
PROFTEST_SOLVER_BRUTEFORCE = $(addsuffix .c, $(PROFTEST_SOLVER_BRUTEFORCE_EXEC))
PROFTEST_SOLVER_OPT_BRUTEFORCE = $(addsuffix .c, $(PROFTEST_SOLVER_OPT_BRUTEFORCE_EXEC))
PROFTEST_SOLVER_BLOCKSIZE_SORT = $(addsuffix .c, $(PROFTEST_SOLVER_BLOCKSIZE_SORT_EXEC))
PROFTEST_SOLVER_LOGIC = $(addsuffix .c, $(PROFTEST_SOLVER_LOGIC_EXEC))
PROFTEST_GENERATOR = $(addsuffix .c, $(PROFTEST_GENERATOR_EXEC))

BITMAP_TEST_EXEC = bitmap_test
BITMAP_TEST = $(addsuffix .c, $(BITMAP_TEST_EXEC))

COV = gcov
PROF = gprof
CFLAGS = -std=c11 -Wall -g
LDFLAGS = -lm
COVFLAGS = -fprofile-arcs -ftest-coverage

all:
	$(MAKE) check
	$(MAKE) profile

check: $(SOURCES) $(COVTEST)
	$(CC) $(COVFLAGS) -I $(HEADERS_DIR) $(SOURCES) $(ERRCODE) $(COVTEST) -o $(COVTEST_EXEC) $(LDFLAGS)
	./$(COVTEST_EXEC) > /dev/null
	$(COV) $(SRC_NAMES).gcno -am

profile: $(SOURCES) $(PROFTEST_SOLVER) $(PROFTEST_GENERATOR)
	$(CC) $(CFLAGS) -no-pie -pg -I $(HEADERS_DIR) $(SOURCES) $(ERRCODE) $(PROFTEST_GENERATOR) -o $(PROFTEST_GENERATOR_EXEC) $(LDFLAGS)
	$(CC) $(CFLAGS) -no-pie -pg -I $(HEADERS_DIR) $(SOURCES) $(ERRCODE) $(PROFTEST_SOLVER) -o $(PROFTEST_SOLVER_EXEC) $(LDFLAGS)
	$(CC) $(CFLAGS) -no-pie -pg -I $(HEADERS_DIR) $(SOURCES) $(ERRCODE) $(PROFTEST_SOLVER_LOGIC) -o $(PROFTEST_SOLVER_LOGIC_EXEC) $(LDFLAGS)
	./$(PROFTEST_SOLVER_EXEC) > /dev/null
	$(PROF) -b $(PROFTEST_SOLVER_EXEC) gmon.out > analysis_solvers.txt
	./$(PROFTEST_SOLVER_LOGIC_EXEC) > /dev/null
	$(PROF) -b $(PROFTEST_SOLVER_LOGIC_EXEC) gmon.out > analysis_solver_logic.txt
	./$(PROFTEST_GENERATOR_EXEC) > /dev/null
	$(PROF) -b $(PROFTEST_GENERATOR_EXEC) gmon.out > analysis_generator.txt

clean:
	rm -f $(COVTEST_EXEC) $(BITMAP_TEST_EXEC) $(PROFTEST_SOLVER_EXEC) $(PROFTEST_SOLVER_BRUTEFORCE_EXEC) $(PROFTEST_SOLVER_OPT_BRUTEFORCE_EXEC) $(PROFTEST_SOLVER_BLOCKSIZE_SORT_EXEC) $(PROFTEST_SOLVER_LOGIC_EXEC) $(PROFTEST_GENERATOR_EXEC)
	rm -f *.gcno *.gcda *.gcov truc.ken gmon.out *.txt foo.dot
