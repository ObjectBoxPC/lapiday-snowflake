#include "matrix.h"
#include "snowflake.h"
#include "draw.h"
#include "constants.h"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace lapiday;
using namespace std;

int main() {
	vector<snowflake::line> lines;
	vector<snowflake::line>::size_type linecount;

	//Setup six "spokes"
	for(int i = 0; i < 6; i++) {
		lines.push_back(snowflake::line(snowflake::rotate(PI / 3 * i) * snowflake::scale(BASE_LENGTH)));
	}

	//Iterate
	vector<snowflake::line> newlines;
	for(unsigned int i = 0; i < ITERATION_COUNT; i++) {
		linecount = lines.size();
		for(vector<snowflake::line>::size_type j = 0; j < linecount; j++) {
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::scale(THIRD)));
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, THIRD) * snowflake::scale(THIRD)));
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, 2 * THIRD) * snowflake::scale(THIRD)));
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, THIRD) * snowflake::rotate(PI / 3) * snowflake::scale(THIRD)));
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, THIRD) * snowflake::rotate(-PI / 3) * snowflake::scale(THIRD)));
			newlines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, 2 * THIRD) * snowflake::rotate(PI / 3) * snowflake::scale(THIRD)));
			newlines.push_back(lines[j].transformation * snowflake::translate(0, 2 * THIRD) * snowflake::rotate(-PI / 3) * snowflake::scale(THIRD));
		}
		lines = newlines;
		newlines.clear();
	}

	//Translate all points
	linecount = lines.size();
	for(vector<snowflake::line>::size_type i = 0; i < linecount; i++) {
		lines[i].transformation = snowflake::translate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2) * lines[i].transformation;
	}

	//Draw
#ifndef LAPIDAY_RENDER_TO_FILE
	sf::RenderWindow target;
	target.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snowflake Test");
#else
	sf::RenderTexture target;
	if(!target.create(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		cout << "Could not create texture!" << endl;
		return 1;
	}
	target.setSmooth(true);
#endif

	target.clear(BACKGROUND_COLOR);
	linecount = lines.size();
	for(vector<snowflake::line>::size_type i = 0; i < linecount; i++) {
		draw_line(target, lines[i], WINDOW_HEIGHT);
	}
	target.display();

#ifndef LAPIDAY_RENDER_TO_FILE
	//Keep open
	while(target.isOpen()) {
		sf::Event e;
		while(target.pollEvent(e)) {
			if(e.type == sf::Event::Closed) {
				target.close();
			}
		}
	}
#else
	target.getTexture().copyToImage().saveToFile("snowflake.png");
#endif

	return 0;
}