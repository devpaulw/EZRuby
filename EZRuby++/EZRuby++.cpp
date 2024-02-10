#include <iostream>
#include "Viewer.h"
#include "Cube.h"
#include "Solver.h"
#include "ezruby_exception.h"
#include <map>
#include <sstream>

using namespace EzRuby;

void printSolutionPhrase(const Cube& cube) {
	// string solution mapper
	std::vector<Rotation> solution = cube.getHistory();
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
}

int main() {
	std::array<Color, Cube::SQ_COUNT> testCubeArr = {
		Color::Yellow, Color::Green, Color::Red, Color::White, Color::Blue, Color::Yellow, Color::Green, Color::Yellow,
		Color::Orange, Color::Blue, Color::Orange, Color::Orange, Color::Orange, Color::Red, Color::Green, Color::Blue,
		Color::Blue, Color::Yellow, Color::Red, Color::Green, Color::White, Color::White, Color::Blue, Color::White,
		Color::Green, Color::Yellow, Color::White, Color::Orange, Color::Orange, Color::Blue, Color::Red, Color::Green,
		Color::Green, Color::White, Color::Green, Color::Blue, Color::Yellow, Color::Orange, Color::Yellow, Color::Yellow,
		Color::Orange, Color::Red, Color::Red, Color::Red, Color::White, Color::Blue, Color::Red, Color::White
	};
	Cube cube(testCubeArr);

	Solver solver(cube);
	solver.solveCube();

	printSolutionPhrase(cube);
	// TODO Solution optimiser
	// Which mean:
	// SolutionProvider
	// input:cube
	// std::vector<Rotation> simplifyRotations(cube)
	// std::string getSolutionPhrase(std::vector<Rotation>)

	ViewerV1 viewer3(cube);
	viewer3.showWindow();
	return 0;
}