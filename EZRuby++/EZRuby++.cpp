#include <iostream>
#include "Viewer.h"
#include "Cube.h"
#include "Solver.h"
#include "ezruby_exception.h"
#include <vector>
#include <map>
#include <sstream>

using namespace EzRuby;



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
	auto solution = solver.getCubeSolution();
	std::cout << "Cube solution:" << std::endl;
	std::cout << solver.getSolutionPhrase() << std::endl;

	// TODO Solution optimiser
	// Which mean:
	// SolutionProvider
	// input:cube
	// std::vector<Rotation> simplifyRotations(cube)
	// std::string getSolutionPhrase(std::vector<Rotation>)
	// getCubeSolution

	ViewerV1 viewer3(cube);
	viewer3.showWindow();
	return 0;
}