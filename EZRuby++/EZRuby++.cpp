#include <iostream>
#include "Viewer.h"
#include "Cube.h"
#include "Solver.h"

using namespace EzRuby;

int main() {
	std::array<Color, Cube::SQ_COUNT> testCubeArr = {
		Color::Yellow, Color::Green, Color::Yellow, Color::Blue, Color::White, Color::Red, Color::Green, Color::Yellow,
		Color::Orange, Color::Blue, Color::Orange, Color::Orange, Color::Orange, Color::Red, Color::Green, Color::Blue,
		Color::Blue, Color::Yellow, Color::Red, Color::Green, Color::White, Color::White, Color::Blue, Color::White,
		Color::Green, Color::Yellow, Color::White, Color::Orange, Color::Orange, Color::Blue, Color::Red, Color::Green,
		Color::Green, Color::White, Color::Green, Color::Blue, Color::Yellow, Color::Orange, Color::Yellow, Color::Yellow,
		Color::Orange, Color::Red, Color::Red, Color::Red, Color::White, Color::Blue, Color::Red, Color::White
	};
	Cube testCube(testCubeArr);
	//while (true) {
	//	ViewerV1 viewer(testCube);
	//	int result = viewer.showWindow();
	//	testCube.rotateFace(Color::White, 2);
	//}
	Solver solver(testCube);
	solver.getCubeSolution();
	ViewerV1 viewer(testCube);
	int result = viewer.showWindow();
	return 0;
}