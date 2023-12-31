#include "Solver.h"
#include <map>
#include <iostream>
#include <functional>
#include "ezruby_exception.h"
#include "Viewer.h"

using namespace EzRuby;

void Solver::step1() {
	const Color startColor = Color::Red;
	Color crossColor = startColor;
	do {
		if (LOG_OUTPUT) std::cout << static_cast<int>(crossColor)<< "TWI\n"; // TEMP
		ColorPair edgePos = _hCube.locateEdgePos(Color::White, crossColor);

		// move 1, aiming to bring the edge towards yellow face
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			if (edgePos.contains(Color::White)) {
				Color rotatingFace = edgePos.first == Color::White ? edgePos.second : edgePos.first; // rotating the color that is not white
				_hCube.rotateFace(rotatingFace, 1); // OPTI to be done here to lower rotation amount, so it's not just 1 but 1 or -1 
			}

			// it's now on the cross side
			Color greatestColor = crossGreatestColor(edgePos.first, edgePos.second);
			int dir = greatestColor == edgePos.first ? 1 : -1;
			_hCube.rotateFace(edgePos.second, dir);
			_hCube.rotateFace(Color::Yellow, dir);
			_hCube.rotateFace(edgePos.second, -dir); // OPTI again because it's not necessarily useful
		}

		// move 2, rotate until one of the edge square touches right face
		// OPTI: can be -1 instead of 3. But this can be done at the move sequences process 
		int rotCount = 0; // For checks
		edgePos = _hCube.locateEdgePos(Color::White, crossColor);
		while (!edgePos.contains(crossColor)) {
			_hCube.rotateFace(Color::Yellow, 1);
			if (++rotCount >= CROSS_COLOR_COUNT) {
				throw EZRubyException("yellow face rotation count should not exceed 3");
			}
			edgePos = _hCube.locateEdgePos(Color::White, crossColor);
		}

		// move 3, to place the side correctly on the white cross
		_hCube.rotateFace(crossColor, 2);

		crossColor = crossNextColor(crossColor);
	} while (crossColor != startColor);
}

void Solver::step2() {
	// let's go for this step
	// first, if the corner doesn't touch the yellow face => place it right
	const Color startColor = Color::Red;
	Color crossColor = startColor;

	do {
		Color nextColor = crossNextColor(crossColor);

		// move 1
		ColorTriplet currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		if (currentCorner.contains(Color::White)) {
			// HTBD If ever we use this "extract cross colors from corner" way, do a method for that
			Color cc1 = (currentCorner.first == White) ? currentCorner.second : currentCorner.first;
			Color cc2 = (currentCorner.first == White || currentCorner.second == White) ? currentCorner.third : currentCorner.second;
			Color faceToRotate = crossGreatestColor(cc1, cc2);

			_hCube.rotateFace(faceToRotate, -1);
			for (size_t i = 0; true; i++) {
				if (i >= 4)
					throw EZRubyException("Infinite loop");
				
				_hCube.rotateFace(Color::Yellow, -1);
				auto guestCp = _hCube.getCornerAt(Color::Yellow, cc1, cc2);
				if (!guestCp.contains(Color::White))
					break;
			}
			_hCube.rotateFace(faceToRotate, 1);
			//_hCube.rotateFace(Color::Yellow, 1);
		}

		// move 2
		currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		while (!(currentCorner.contains(crossColor) && currentCorner.contains(nextColor))) {
			_hCube.rotateFace(Color::Yellow, 1);
			currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		}

		// move 3
		// TODO 3 cases to handle, either:
		// - white touches yellow
		// - white touches crossColor (leftColor)
		// - white touches nextColor (rightColor)
		// then just do a few moves, and it's all right.

		//crossColor = nextColor; //TEMP to avoid 
	} while (crossColor  != startColor);
}

std::vector<MoveOrientation> Solver::getCubeSolution() {
	_solution.clear(); // to avoid issues when the method is called many times

	step1();
	step2();

	return std::vector<MoveOrientation>(); // temp
}

// order: red green orange blue, right if top side is white, left if yellow
Color Solver::crossNextColor(Color color) {
	std::map<Color, Color> nextColors = {
		{Color::Red, Color::Blue},
		{Color::Blue, Color::Orange},
		{Color::Orange, Color::Green},
		{Color::Green, Color::Red},
	};

	return nextColors[color];
}

Color Solver::crossGreatestColor(Color color1, Color color2) {
	std::map<Color, int> crossColorOrder = {
	{ Color::Blue, 0 },{ Color::Red, 1 },{ Color::Green, 2 },{ Color::Orange, 3 } };

	int face1Rank = crossColorOrder[color1],
		face2Rank = crossColorOrder[color2];

	if ((color1 == Color::Orange && color2 == Color::Blue)
		|| (color1 == Color::Blue && color2 == Color::Orange))
		return Color::Orange; // exceptional because orange is actually lower than blue
	else if (face1Rank < face2Rank)
		return color1;
	else if (face2Rank < face1Rank)
		return color2;
	else
		throw std::exception();
}