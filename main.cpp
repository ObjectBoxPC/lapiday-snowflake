#include "matrix.h"
#include "snowflake.h"
#include "draw.h"
#include "constants.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstddef>
#include <SFML/Graphics.hpp>

using namespace lapiday;
using namespace std;

int main() {
	srand(time(NULL));
	vector<snowflake::line> lines;
	vector<snowflake::line>::size_type linecount;

	//Seed a single "spoke"
	lines.push_back(snowflake::line(snowflake::scale(BASE_LENGTH)));

	//Iterate
	//vector<snowflake::line> newlines;
	double distance; //Distance from parent line (0 to 1)
	double scale; //Scale factor (0 to 1)
	for(unsigned int i = 0; i < ITERATION_COUNT; i++) {
		linecount = lines.size();
		for(vector<snowflake::line>::size_type j = 0; j < linecount; j++) {
			if(!lines[j].completed) {
				for(unsigned int k = 0; k < PAIRS_PER_LINE; k++) {
					//Distance must be in the k-th part
					distance = ((static_cast<double>(rand()) / RAND_MAX) + k) / PAIRS_PER_LINE;
					scale = MIN_SCALE + (static_cast<double>(rand()) / RAND_MAX) * (MAX_SCALE - MIN_SCALE);
					lines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, distance) * snowflake::rotate(PI / 3) * snowflake::scale(scale)));
					lines.push_back(snowflake::line(lines[j].transformation * snowflake::translate(0, distance) * snowflake::rotate(-PI / 3) * snowflake::scale(scale)));
				}
				lines[j].completed = true;
			}
		}
	}

	//Create remaining "spokes"
	linecount = lines.size();
	for(vector<snowflake::line>::size_type i = 0; i < linecount; i++) {
		for(int j = 1; j < 6; j++) {
			lines.push_back(snowflake::line(snowflake::rotate(PI / 3 * j) * lines[i].transformation));
		}
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