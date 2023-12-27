# Snowflake

This program generates snowflakes using a fractal model. Snowflakes can be rendered to screen or an image file, and parameters can be varied.

More information is at <https://objectboxpc.name/pi-day-2015.html>.

This program is free and open-source under the 3-clause BSD license (refer to `LICENSE.txt` for details).

## Compiling

The program requires [SFML](http://sfml-dev.org/), version 2.0 or later. You will have to link against the *Graphics*, *Window*, and *System* modules (in that order).

Suggested build line (for GCC/MinGW):

	g++ -Wall -Wextra -ansi -pedantic -iquote./lapiday lapiday/* main.cpp -lsfml-graphics -lsfml-window -lsfml-system

The `main.cpp` file generates randomized snowflakes. It can be replaced with `nonrandom.cpp` for nonrandom snowflakes, or `matrixdemo.cpp` for a demonstration of the matrix functionality.