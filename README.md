# MXM

Benchmarking dynamic compilation with different compilers

## Setup

At the top of the [main Makefile](./Makefile) there are a couple of variables
which must be set to appropriate values. The most important one being the
variable `CC`.

If you need to regenerate `compile_commands.json` run `bear -- make all`. Should
the file be empty after this you must most likely change the variable `CC` in
the [main Makefile](./Makefile).

## Running

Run `make benchmark`
