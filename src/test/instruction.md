# Instruction to compute unit tests

- use ```minunit``` or ```criterion``` library (use -lcriterion).
- respect the same path for the tested src file.
- complete Makefile to execute your test.

## Instruction how to use criterion

- command to run the tests files : ```$ make tests``` from root Makefile.
- to add new tests' files you need to put the names of the involved files into the ```OBJS_TEST_* =``` dependecies from main and test directories : example made with ```logger.c & test_logger.c```.

## Instruction how to install criterion

website : ```https://criterion.readthedocs.io/en/master/setup.html```;