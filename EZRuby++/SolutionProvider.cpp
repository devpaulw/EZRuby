#include "SolutionProvider.h"
#include "ezruby_exception.h"
#include <map>
#include <sstream>
#include <iostream>

using namespace EzRuby;

std::vector<Rotation> SolutionProvider::simplifyRotations(std::vector<Rotation> solution) {
	std::vector<Rotation> simplifiedSolution;

	// step 1: add up the rotations 
	for (const Rotation& rotation : solution) {

	}
}

std::string SolutionProvider::getSolutionPhrase(std::vector<Rotation> solution) {
	// string solution mapper
	static std::map<Color, char> colorMoveMap = {
		{Color::Red, 'F'}, // front
		{Color::Blue, 'R'}, // right
		{Color::Orange, 'B'}, // back
		{Color::Green, 'L'}, // left
		{Color::White, 'U'}, // up
		{Color::Yellow, 'D'}, // down
	};

	std::stringstream solutionPhrase;
	for (Rotation rotation : solution) {
		char move = colorMoveMap[rotation.faceColor];
		solutionPhrase << move;

		if (rotation.towards == -1)
			solutionPhrase << '\'';
		else if (rotation.towards == 2)
			solutionPhrase << '2';
		else if (rotation.towards != 1)
			throw EZRubyException("towards cannot be different than +, - or ++");
	}

	std::cout << "Cube solution:" << std::endl;
	std::cout << solutionPhrase.str() << std::endl;

	return solutionPhrase.str();
}