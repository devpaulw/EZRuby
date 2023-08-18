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
		std::cout << static_cast<int>(crossColor)<< "TWI\n"; // TEMP
		EdgePosition edgePos = _hCube.getEdgePos(Color::White, crossColor);

		// move 1, aiming to bring the edge towards yellow face
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			if (edgePos.contains(Color::White)) {
				Color rotatingFace = edgePos.face1Color == Color::White ? edgePos.face2Color : edgePos.face1Color; // rotating the color that is not white
				_hCube.rotateFace(rotatingFace, 1); // OPTI to be done here to lower rotation amount, so it's not just 1 but 1 or -1 
			}

			// it's now on the cross side
			Color greatestColor = crossGreatestColor(edgePos.face1Color, edgePos.face2Color);
			int dir = greatestColor == edgePos.face1Color ? 1 : -1;
			_hCube.rotateFace(edgePos.face2Color, dir);
			_hCube.rotateFace(Color::Yellow, dir);
			_hCube.rotateFace(edgePos.face2Color, -dir); // OPTI again because it's not necessarily useful
		}

		// move 2, rotate until one of the edge square touches right face
		// OPTI: can be -1 instead of 3. But this can be done at the move sequences process 
		int rotCount = 0; // For checks
		edgePos = _hCube.getEdgePos(Color::White, crossColor);
		while (!edgePos.contains(crossColor)) {
			_hCube.rotateFace(Color::Yellow, 1);
			if (++rotCount >= CROSS_COLOR_COUNT) {
				throw EZRubyException("yellow face rotation count should not exceed 3");
			}
			edgePos = _hCube.getEdgePos(Color::White, crossColor);
		}

		// move 3, to place the side correctly on the white cross
		_hCube.rotateFace(crossColor, 2);

		crossColor = crossNextColor(crossColor);
	} while (crossColor != startColor);
}

void Solver::step2()
{
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