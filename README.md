# KenKen Solver & Generator

The goal of this project is to create efficients solver and generator for the logical game KenKen.


## Compilation

To compile the project, use the command:
```
make
```


## Documentation

If you want to generate the project documentation, use:
```
make doc
```

Then, execute the following line to display the documentation in your web
browser (for instance, Firefox):
```
firefox doc/html/index.html
```

## Execution

Launch the following command to execute the application:
```
./kenken
```

All the grids used to test the program are located in the resources/ folder.
A basic command line to test the program efficiency is:
```
time ./kenken resources/9x9_easiest.ken
```

The program should take up to 0,200s to solve this grid.

If you want to display the help, use this command:
```
./kenken --help
```


## Project Structure

```
kenken/
├── doc/
│   ├── cahier_besoins.pdf
│   ├── Doxyfile
│   ├── mainpage.dox
|   └── Makefile
├── include/
│   ├── bitmap.h
│   ├── errcode.h
│   ├── generator.h
│   ├── grid.h
│   ├── output.h
│   ├── parser.h
│   └── solver.h
├── Makefile
├── README.md
├── resources/
│   ├── 3x3_easiest.ken
│   ├── 4x4_hard.ken
│   ├── 9x9_easiest.ken
│   └── example_grid.ken
│   └── ...
├── src/
│   ├── bitmap.c
│   ├── bruteforce_solver.c
│   ├── errcode.c
│   ├── generator.c
│   ├── grid.c
│   ├── kenken.c
│   ├── logic_solver.c
│   ├── Makefile
│   ├── output.c
│   ├── parser.c
│   └── solver.c
└── test/
    ├── covtest.c
    ├── Makefile
    └── proftest.c

```


## Headers Summary

* bitmap.h:    contains useful functions to manage bitmaps.
* errcode.h:   front-end module containing functions used to manage error codes
               and display error messages.
* generator.h: contains the functions used to generate grids.
* grid.h:      contains grid structure & its initialization functions.
* output.h:    contains the functions used to write grids in files or standard
               output.
* parser.h:    contains the functions used to parse a grid file.
* solver.h:    contains the functions used to solve a grid.
