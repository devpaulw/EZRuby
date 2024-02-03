#include "Solver.h"
#include <map>
#include <iostream>
#include <functional>
#include "ezruby_exception.h"
#include "Viewer.h"

using namespace EzRuby;

void Solver::whiteCrossStep() {
	const Color startColor = Color::Red;
	Color crossColor = startColor;
	do {
		if (LOG_OUTPUT) std::cout << static_cast<int>(crossColor) << "TWI\n"; // TEMP
		ColorPair edgePos = _hCube.locateEdgePos(Color::White, crossColor);

		// move 1, aiming to bring the edge towards yellow face
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			if (edgePos.contains(Color::White)) {
				Color rotatingFace = edgePos.first == Color::White ? edgePos.second : edgePos.first; // rotating the color that is not white
				_hCube.performRotation(rotatingFace, 1); // OPTI to be done here to lower rotation amount, so it's not just 1 but 1 or -1 
			}

			// it's now on the cross side
			Color greatestColor = crossGreatestColor(edgePos.first, edgePos.second);
			int dir = greatestColor == edgePos.first ? 1 : -1;
			_hCube.performRotationSequence({
				{ edgePos.second, dir },
				{ Color::Yellow, dir },
				{ edgePos.second, -dir } });
			// OPTI again because it's not necessarily useful
		}

		// move 2, rotate until one of the edge square touches right face
		// OPTI: can be -1 instead of 3. But this can be done at the move sequences process 
		int rotCount = 0; // For checks
		edgePos = _hCube.locateEdgePos(Color::White, crossColor);
		while (!edgePos.contains(crossColor)) {
			_hCube.performRotation(Color::Yellow, 1);
			if (++rotCount >= CROSS_COLOR_COUNT) {
				throw EZRubyException("yellow face rotation count should not exceed 3");
			}
			edgePos = _hCube.locateEdgePos(Color::White, crossColor);
		}

		// move 3, to place the side correctly on the white cross
		_hCube.performRotation(crossColor, 2);

		crossColor = crossNextColor(crossColor);
	} while (crossColor != startColor);
}

void Solver::whiteCornersStep() {
	// first, if the corner doesn't touch the yellow face => place it right
	const Color startColor = Color::Red;
	Color crossColor = startColor;

	do {
		Color nextColor = crossNextColor(crossColor);

		// move 1 - remove corner from crown (especially when wrong one, or displaced)
		ColorTriplet currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		if (currentCorner.contains(Color::White)) {
			// HTBD If ever we use this "extract cross colors from corner" way, do a method for that
			Color cc1 = (currentCorner.first == White) ? currentCorner.second : currentCorner.first;
			Color cc2 = (currentCorner.first == White || currentCorner.second == White) ? currentCorner.third : currentCorner.second;
			Color faceToRotate = crossGreatestColor(cc1, cc2);

			_hCube.performRotation(faceToRotate, -1);
			for (size_t i = 0; true; i++) {
				if (i >= 4)
					throw EZRubyException("Infinite loop");

				_hCube.performRotation(Color::Yellow, -1);
				auto guestCp = _hCube.getCornerAt(Color::Yellow, cc1, cc2);
				if (!guestCp.contains(Color::White))
					break;
			}
			_hCube.performRotation(faceToRotate, 1);
			//_hCube.rotateFace(Color::Yellow, 1);
		}

		// move 2 - place the corner below right emplacement
		currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		while (!(currentCorner.contains(crossColor) && currentCorner.contains(nextColor))) {
			_hCube.performRotation(Color::Yellow, 1);
			currentCorner = _hCube.locateCornerPos(Color::White, crossColor, nextColor);
		}

		// move 3 - place the corner correctly in the white face
		if (currentCorner.first == Color::Yellow) {
			// white touches yellow
			_hCube.performRotationSequence({
				{ crossColor, 1 },
				{ Color::Yellow, 2 },
				{ crossColor, -1 },
				{ Color::Yellow, -2 },
				{ nextColor, -1 },
				{ Color::Yellow, 1 },
				{ nextColor, 1 } });
		}
		else if (currentCorner.first == crossColor) {
			// white touches crossColor (leftColor)
			_hCube.performRotationSequence({
				{ crossColor, 1 },
				{ Color::Yellow, 1 },
				{ crossColor, -1 } });
		}
		else if (currentCorner.first == nextColor) {
			// white touches nextColor (rightColor)
			_hCube.performRotationSequence({
				{ nextColor, -1 },
				{ Color::Yellow, -1 },
				{ nextColor, 1 } });
		}

		crossColor = nextColor; //TEMP to avoid 
	} while (crossColor != startColor);
}

void Solver::middleLayerStep() {
	const Color startColor = Color::Red;
	Color crossColor = startColor;
	// HTBD: because this scheme is everywhere, do something to do some kind of lambda 
	// (give directly the crossColor and nextColor in function arguments)

	do {
		Color nextColor = crossNextColor(crossColor);

		// move 1 - unlock corner if trapped
		// OPTI no need to unlock if already well placed (to check)
		ColorPair edgePos = _hCube.locateEdgePos(crossColor, nextColor);
		if (!edgePos.contains(Color::Yellow)) {
			Color trapRightFace = crossGreatestColor(edgePos.first, edgePos.second),
				trapLeftFace = trapRightFace == edgePos.first ? edgePos.second : edgePos.first;

			_hCube.performRotationSequence({
				{ trapLeftFace, 1 },
				{ Color::Yellow, 1 },
				{ trapLeftFace, -1 },
				{ Color::Yellow, -1 },
				{ trapRightFace, -1 },
				{ Color::Yellow, -1 },
				{ trapRightFace, 1 } });
		}

		// move 2 - place the corner
		edgePos = _hCube.locateEdgePos(crossColor, nextColor);
		// side color is the one that is not in the yellow face
		Color edgeColorOnSide = edgePos.first == Color::Yellow ? nextColor : crossColor;
		while (!edgePos.contains(edgeColorOnSide)) {
			_hCube.performRotation(Color::Yellow, 1);
			edgePos = _hCube.locateEdgePos(crossColor, nextColor);
		}

		if (edgeColorOnSide == nextColor) { // edge is on the right of its desired location
			_hCube.performRotationSequence({
				{ Color::Yellow, 1 },
				{ crossColor, 1 },
				{ Color::Yellow, -1 },
				{ crossColor, -1 },
				{ Color::Yellow, -1 },
				{ nextColor, -1 },
				{ Color::Yellow, 1 },
				{ nextColor, 1 } });
		}
		else { // on the left
			_hCube.performRotationSequence({
				{ Color::Yellow, -1 },
				{ nextColor, -1 },
				{ Color::Yellow, 1 },
				{ nextColor, 1 },
				{ Color::Yellow, 1 },
				{ crossColor, 1 },
				{ Color::Yellow, -1 },
				{ crossColor, -1 } });
		}

		crossColor = nextColor;
	} while (crossColor != startColor);
}

void Solver::yellowCrossStep() {
	// 1. Identify yellow face pattern
	const Color startLeftColor = Color::Green;
	const Color startRightColor = crossNextColor(startLeftColor);
	Color crossColor = startRightColor;
	int yellowPattern = 0b0;

	for (int i = 0b1000; true; i /= 0b10) {
		ColorPair edgePos = _hCube.locateEdgePos(Color::Yellow, crossColor);
		if (edgePos.first == Color::Yellow)
			yellowPattern += i;

		if ((crossColor = crossNextColor(crossColor)) == startRightColor)
			break;
	}

	// 2. Identify pattern type
	enum PatternType { Notch, Line, Point };
	PatternType patternType{};

	switch (yellowPattern) {
	case 0b0000:
		patternType = PatternType::Point;
		break;
	case 0b0101:
		patternType = PatternType::Line;
		break;
	case 0b1010:
		patternType = PatternType::Line;
		_hCube.performRotation(Color::Yellow, -1);
		break;
	case 0b0011:
		patternType = PatternType::Notch;
		break;
	case 0b0110:
		patternType = PatternType::Notch;
		_hCube.performRotation(Color::Yellow, -1);
		break;
	case 0b1001:
		patternType = PatternType::Notch;
		_hCube.performRotation(Color::Yellow, 1);
		break;
	case 0b1100:
		patternType = PatternType::Notch;
		_hCube.performRotation(Color::Yellow, 2);
		break;
	default:
		throw std::exception("unknown yellow pattern");
	}

	// 3. Do the right move knowing the pattern type
	switch (patternType) {
	case PatternType::Point:
	case PatternType::Line:
		// FRUR'U'F'
		_hCube.performRotationSequence({
			{ startRightColor, 1 },
			{ startLeftColor, 1 },
			{ Color::Yellow, 1 },
			{ startLeftColor, -1 },
			{ Color::Yellow, -1 },
			{ startRightColor, -1 } });

		if (patternType == PatternType::Point) {
			yellowCrossStep();
		}
		break;
	case PatternType::Notch:
		// FURU'R'F'
		_hCube.performRotationSequence({
			{ startRightColor, 1 },
			{ Color::Yellow, 1 },
			{ startLeftColor, 1 },
			{ Color::Yellow, -1 },
			{ startLeftColor, -1 },
			{ startRightColor, -1 } });
		break;
	}
}

void Solver::edgeIntegrityStep() {

}

std::vector<MoveOrientation> Solver::getCubeSolution() {
	_solution.clear(); // to avoid issues when the method is called many times

	whiteCrossStep();
	whiteCornersStep();
	middleLayerStep();
	yellowCrossStep();

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