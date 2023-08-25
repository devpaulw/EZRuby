#include <iostream>
#include "Viewer.h"
#include "Cube.h"
#include "Solver.h"

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
	//while (true) {
	//	ViewerV1 viewer(testCube);
	//	int result = viewer.showWindow();
	//	testCube.rotateFace(Color::White, 2);
	//}

	//ColorTriplet triplet1 = cube.getCornerAt(Color::Red, Color::Blue, Color::White);
	//ColorTriplet triplet2 = cube.getCornerAt(Color::Red, Color::White, Color::Green);
	//ColorTriplet triplet3 = cube.getCornerAt(Color::Red, Color::White, Color::Blue);
	//ColorTriplet triplet4 = cube.getCornerAt(Color::Yellow, Color::White, Color::Blue);

	Solver solver(cube);
	solver.getCubeSolution();

	ViewerV1 viewer3(cube);
	viewer3.showWindow();

	//testCube.rotateFace(static_cast<Color>(4), 1);
	//ViewerV1 viewer3(testCube);
	//viewer3.showWindow();

	//testCube.rotateFace(static_cast<Color>(0), 2);
	//ViewerV1 viewer4(testCube);
	//viewer4.showWindow();
	return 0;
}