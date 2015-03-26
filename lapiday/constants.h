#ifndef LAPIDAY_CONSTANTS_H
#define LAPIDAY_CONSTANTS_H

#include <SFML/Graphics.hpp>

namespace lapiday {
	const double PI = 3.141592653589793;
	const double THIRD = 1.0 / 3;
	const double ROOT_TWO = 1.414213562373095;
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;
	const double BASE_LENGTH = 240;
	const sf::Color BACKGROUND_COLOR = sf::Color::White;
	const sf::Color FOREGROUND_COLOR = sf::Color::Black;
	const unsigned int ITERATION_COUNT = 4;
	/**
	 * For line-offset rendering
	 */
	const double LINE_OFFSET = 4;
	/**
	 * For random snowflake
	 */
	const unsigned int PAIRS_PER_LINE = 2;
	const double MIN_SCALE = 1.0 / 4;
	const double MAX_SCALE = 1.0 / 3;
	/**
	 * For pentagon rendering
	 */
	const double PENTAGON_WIDTH = 10;
}
#define LAPIDAY_RENDER_TO_FILE

#endif