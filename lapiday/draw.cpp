#include "draw.h"
#include "constants.h"
#include <cmath>

//using std::acos;
using std::sqrt;
//using std::fabs;

namespace lapiday {
	void draw_line(sf::RenderTarget& target, const snowflake::line& line, int height) {
		matrix::matrix base(3, 2);
		base(0, 0) = 0;
		base(0, 1) = 0;
		base(1, 0) = 0;
		base(1, 1) = 1;
		base(2, 0) = 1;
		base(2, 1) = 1;
		matrix::matrix endpoints = line.transformation * base;
		/*
		//Offset-polygon rendering
		sf::ConvexShape temp(4);
		temp.setPoint(0, sf::Vector2f(endpoints(0, 0), height - endpoints(1, 0)));
		temp.setPoint(1, sf::Vector2f(endpoints(0, 1), height - endpoints(1, 1)));
		temp.setPoint(2, sf::Vector2f(endpoints(0, 0) + LINE_OFFSET, height - endpoints(1, 0) + LINE_OFFSET));
		temp.setPoint(3, sf::Vector2f(endpoints(0, 1) + LINE_OFFSET, height - endpoints(1, 1) + LINE_OFFSET));
		temp.setFillColor(FOREGROUND_COLOR);
		target.draw(temp);
		*/
		/*
		//Line-primitive rendering
		sf::Vertex temp[2];
		temp[0] = sf::Vertex(sf::Vector2f(endpoints(0, 0), height - endpoints(1, 0)), FOREGROUND_COLOR);
		temp[1] = sf::Vertex(sf::Vector2f(endpoints(0, 1), height - endpoints(1, 1)), FOREGROUND_COLOR);
		target.draw(temp, 2, sf::Lines);
		*/

		//Pentagon rendering
		//The origin is an extra point
		sf::ConvexShape temp(6);
		double x = endpoints(0, 1) - endpoints(0, 0);
		double y = endpoints(1, 1) - endpoints(1, 0);
		//Scale that yields a line segment as long as half the width
		double normscale = 0.5 * PENTAGON_WIDTH / sqrt(x * x + y * y);
		//double normscale = 1;
		matrix::matrix segment;
		segment = line.transformation * snowflake::rotate(PI / 2) * snowflake::scale(normscale) * base;
		temp.setPoint(0, sf::Vector2f(segment(0, 1), height - segment(1, 1)));
		temp.setPoint(1, sf::Vector2f(segment(0, 0), height - segment(1, 0)));
		segment = line.transformation * snowflake::translate(normscale, 0) * snowflake::scale(1 - normscale) * base;
		temp.setPoint(2, sf::Vector2f(segment(0, 0), height - segment(1, 0)));
		temp.setPoint(3, sf::Vector2f(segment(0, 1), height - segment(1, 1)));
		segment = line.transformation * snowflake::translate(0, 1) * snowflake::rotate(PI * 3 / 4) * snowflake::scale(ROOT_TWO * normscale) * base;
		temp.setPoint(4, sf::Vector2f(segment(0, 0), height - segment(1, 0)));
		temp.setPoint(5, sf::Vector2f(segment(0, 1), height - segment(1, 1)));
		temp.setFillColor(FOREGROUND_COLOR);
		target.draw(temp);

	}
}