#ifndef LAPIDAY_DRAW_H
#define LAPIDAY_DRAW_H

#include "matrix.h"
#include "snowflake.h"
#include <SFML/Graphics.hpp>

namespace lapiday {
	/**
	 * Draw a line to the target.
	 * @param target Target to draw to
	 * @param line Line to draw
	 * @param height Height of the target
	 */
	void draw_line(sf::RenderTarget& target, const snowflake::line& line, int height);
}

#endif