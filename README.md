# cs3520-proj

This is the final project for Joshua (duck\_master), Samuel (ay136416), and Andrew (turingproblem) for CS3520: Introduction to C++. It aims to implement the game of chess.

## File structure

General structure:
 * src: for source (.cpp) files, everything here will get compiled/linked (src/main.cpp provided)
 * test: for test source code (using CxxText, expected to be in extern/cxxtest-4.4)
 * include: for header (.hpp) files, looked in for #include
 * res: for resources (e.g. images)
 * build: where created intermediate (e.g. object) files go
 * bin: where created executables go
 * doc: where generated documentation (Doxygen) goes

## Making

The Makefile is set up to build all `.cpp` files in the `src` folder and create executables in `bin/main`. To just build the release executable, run `make exe`. 
Note that `make -j` runs about 2.5x faster than `make` alone (you can verify this by running `time make 1>/dev/null` and `time make -j 1>/dev/null`). Similarly, `make -j bin/test` runs about 2.5x faster than `make bin/test`, and this is useful for debugging.

Configurations:
 * rel: for speed
 * deb: for debugging
 * cov: for code coverage (with gcov)
 * prf: for performance analysis (with gprof)

## SDL (to come later)

Will Usher's SDL 2.0 Tutorial can be found here: https://www.willusher.io/pages/sdl2/.
