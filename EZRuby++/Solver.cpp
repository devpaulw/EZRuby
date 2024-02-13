#include "Solver.h"
#include <map>
#include <iostream>
#include <sstream>
#include <array>
#include <optional>
#include "ezruby_exception.h"
#include "Viewer.h"

using namespace EzRuby;

void Solver::whiteCrossStep() {
	executeCCLoop([&](CCLoopIteration it) {
		ColorPair edgePos = _hCube.locateEdgePos(Color::White, it.crossColor);

		// move 1, aiming to bring the edge towards yellow face
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			if (edgePos.contains(Color::White)) {
				Color rotatingFace = edgePos.first == Color::White ? edgePos.second : edgePos.first; // rotating the color that is not white
				this->recordRotation(rotatingFace, 1); // OPTI to be done here to lower rotation amount, so it's not just 1 but 1 or -1 
			}

			// it's now on the cross side
			Color greatestColor = crossGreatestColor(edgePos.first, edgePos.second);
			int dir = greatestColor == edgePos.first ? 1 : -1;
			this->recordRotationSequence({
				{ edgePos.second, dir },
				{ Color::Yellow, dir },
				{ edgePos.second, -dir } });
			// OPTI again because it's not necessarily useful
		}

		// move 2, rotate until one of the edge square touches right face
		// OPTI: can be -1 instead of 3. But this can be done at the move sequences process 
		int rotCount = 0; // For checks
		edgePos = _hCube.locateEdgePos(Color::White, it.crossColor);
		while (!edgePos.contains(it.crossColor)) {
			this->recordRotation(Color::Yellow, 1);
			if (++rotCount >= CROSS_COLOR_COUNT) {
				throw EZRubyException("yellow face rotation count should not exceed 3");
			}
			edgePos = _hCube.locateEdgePos(Color::White, it.crossColor);
		}

		// move 3, to place the side correctly on the white cross
		this->recordRotation(it.crossColor, 2);
		});
}

void Solver::whiteCornersStep() {
	// first, if the corner doesn't touch the yellow face => place it right
	executeCCLoop([&](CCLoopIteration it) {
		// move 1 - remove corner from crown (especially when wrong one, or displaced)
		ColorTriplet currentCorner = _hCube.locateCornerPos(Color::White, it.crossColor, it.nextColor);
		if (currentCorner.contains(Color::White)) {
			// HTBD If ever we use this "extract cross colors from corner" way, do a method for that
			Color cc1 = (currentCorner.first == White) ? currentCorner.second : currentCorner.first;
			Color cc2 = (currentCorner.first == White || currentCorner.second == White) ? currentCorner.third : currentCorner.second;
			Color faceToRotate = crossGreatestColor(cc1, cc2);

			this->recordRotation(faceToRotate, -1);
			for (size_t i = 0; true; i++) {
				if (i >= CROSS_COLOR_COUNT)
					throw EZRubyException("Infinite loop");

				this->recordRotation(Color::Yellow, -1);
				auto guestCp = _hCube.getCornerAt(Color::Yellow, cc1, cc2);
				if (!guestCp.contains(Color::White))
					break;
			}
			this->recordRotation(faceToRotate, 1);
			//_hCube.rotateFace(Color::Yellow, 1);
		}

		// move 2 - place the corner below right emplacement
		currentCorner = _hCube.locateCornerPos(Color::White, it.crossColor, it.nextColor);
		while (!(currentCorner.contains(it.crossColor) && currentCorner.contains(it.nextColor))) {
			this->recordRotation(Color::Yellow, 1);
			currentCorner = _hCube.locateCornerPos(Color::White, it.crossColor, it.nextColor);
		}

		// move 3 - place the corner correctly in the white face
		if (currentCorner.first == Color::Yellow) {
			// white touches yellow
			this->recordRotationSequence({
				{ it.crossColor, 1 },
				{ Color::Yellow, 2 },
				{ it.crossColor, -1 },
				{ Color::Yellow, 2 },
				{ it.nextColor, -1 },
				{ Color::Yellow, 1 },
				{ it.nextColor, 1 } });
		}
		else if (currentCorner.first == it.crossColor) {
			// white touches crossColor (leftColor)
			this->recordRotationSequence({
				{ it.crossColor, 1 },
				{ Color::Yellow, 1 },
				{ it.crossColor, -1 } });
		}
		else if (currentCorner.first == it.nextColor) {
			// white touches nextColor (rightColor)
			this->recordRotationSequence({
				{ it.nextColor, -1 },
				{ Color::Yellow, -1 },
				{ it.nextColor, 1 } });
		}

		it.crossColor = it.nextColor; //TEMP to avoid 
		});
}

void Solver::middleLayerStep() {
	// HTBD: because this scheme is everywhere, do something to do some kind of lambda 
	// (give directly the crossColor and nextColor in function arguments)
	executeCCLoop([&](CCLoopIteration it) {
		// move 1 - unlock corner if trapped
		// OPTI no need to unlock if already well placed (to check)
		ColorPair edgePos = _hCube.locateEdgePos(it.crossColor, it.nextColor);
		if (!edgePos.contains(Color::Yellow)) {
			Color trapRightFace = crossGreatestColor(edgePos.first, edgePos.second),
				trapLeftFace = trapRightFace == edgePos.first ? edgePos.second : edgePos.first;

			this->recordRotationSequence({
				{ trapLeftFace, 1 },
				{ Color::Yellow, 1 },
				{ trapLeftFace, -1 },
				{ Color::Yellow, -1 },
				{ trapRightFace, -1 },
				{ Color::Yellow, -1 },
				{ trapRightFace, 1 } });
		}

		// move 2 - place the corner
		edgePos = _hCube.locateEdgePos(it.crossColor, it.nextColor);
		// side color is the one that is not in the yellow face
		Color edgeColorOnSide = edgePos.first == Color::Yellow ? it.nextColor : it.crossColor;
		while (!edgePos.contains(edgeColorOnSide)) {
			this->recordRotation(Color::Yellow, 1);
			edgePos = _hCube.locateEdgePos(it.crossColor, it.nextColor);
		}

		if (edgeColorOnSide == it.nextColor) { // edge is on the right of its desired location
			this->recordRotationSequence({
				{ Color::Yellow, 1 },
				{ it.crossColor, 1 },
				{ Color::Yellow, -1 },
				{ it.crossColor, -1 },
				{ Color::Yellow, -1 },
				{ it.nextColor, -1 },
				{ Color::Yellow, 1 },
				{ it.nextColor, 1 } });
		}
		else { // on the left
			this->recordRotationSequence({
				{ Color::Yellow, -1 },
				{ it.nextColor, -1 },
				{ Color::Yellow, 1 },
				{ it.nextColor, 1 },
				{ Color::Yellow, 1 },
				{ it.crossColor, 1 },
				{ Color::Yellow, -1 },
				{ it.crossColor, -1 } });
		}
		});
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
		this->recordRotation(Color::Yellow, -1);
		break;
	case 0b0011:
		patternType = PatternType::Notch;
		break;
	case 0b0110:
		patternType = PatternType::Notch;
		this->recordRotation(Color::Yellow, -1);
		break;
	case 0b1001:
		patternType = PatternType::Notch;
		this->recordRotation(Color::Yellow, 1);
		break;
	case 0b1100:
		patternType = PatternType::Notch;
		this->recordRotation(Color::Yellow, 2);
		break;
	default:
		throw EZRubyException("unknown yellow pattern");
	}

	// 3. Do the right move knowing the pattern type
	switch (patternType) {
	case PatternType::Point:
	case PatternType::Line:
		// FRUR'U'F'
		this->recordRotationSequence({
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
		this->recordRotationSequence({
			{ startRightColor, 1 },
			{ Color::Yellow, 1 },
			{ startLeftColor, 1 },
			{ Color::Yellow, -1 },
			{ startLeftColor, -1 },
			{ startRightColor, -1 } });
		break;
	}
}

void Solver::edgeCongruenceStep() {
	// OPTI: know specific moves associated with each cases
	// first: turn until one is ok
	std::vector<Color> congruentEdgeColors;
	executeCCLoop([&](CCLoopIteration it) {
		ColorPair edgePos = _hCube.locateEdgePos(it.crossColor, Color::Yellow);
		if (edgePos.first == it.crossColor) { // means the edge has integrity
			congruentEdgeColors.push_back(it.crossColor);
		}
		});

	switch (congruentEdgeColors.size()) {
	case 4: {
		return; // yellow cross congruent, stop process
	}
		  // following cases: yellow cross unlock or attempt to solve
	case 0: {
		// if no congruence, keep seeking
		this->recordRotation(Color::Yellow, 1);
		break;
	}
	case 1: {
		// if unicity edge congruence, do right chair (twice if necessary)
		Color unicityViewpoint = congruentEdgeColors.front();
		performChairMove(crossPreviousColor(unicityViewpoint), Color::Yellow, false);
		break;
	}
	case 2: {
		// unlock line or notch congruence cases
		Color cc1 = congruentEdgeColors.front(),
			cc2 = congruentEdgeColors.back();
		// check if edge integrity type is notch
		bool notchCongruence = crossPreviousColor(cc2) == cc1;
		if (notchCongruence) {
			// if notch edge congruence, there will necessarily be unicity somewhere => keep seeking
			this->recordRotation(Color::Yellow, 1);
		}
		else {
			// if line edge congruence, break it and do integrity again (do one chair anywhere)
			performChairMove(crossPreviousColor(cc1), Color::Yellow, false);
		}
		break;
	}
	default: {
		std::stringstream ss;
		ss << "there cannot be " << congruentEdgeColors.size() << " congruent edges";
		throw EZRubyException(ss.str().c_str());
	}
	}
	// continue process until 4 congruent edges
	edgeCongruenceStep();
}

void Solver::cornerPositioningStep() {
	auto performMove = [&](Color leftFace, Color rightFace) {
		this->recordRotationSequence({
			{ leftFace, -1 },
			{ Color::Yellow, 1 },
			{ rightFace, 1 },
			{ Color::Yellow, -1 },
			{ leftFace, 1 },
			{ Color::Yellow, 1 },
			{ rightFace, -1 },
			{ Color::Yellow, -1 } });
		};
	bool correctPreviousCorner = false;

	executeCCLoop([&](CCLoopIteration it) {
		ColorTriplet corner = _hCube.getCornerAt(Color::Yellow, it.crossColor, it.nextColor);
		if (corner.contains(it.crossColor) && corner.contains(it.nextColor)) {
			// means this one is correctly positioned
			if (correctPreviousCorner) {
				// if at least 2 are well positioned => all are correct => finish the process
				it.stop = true;
			}
			else {
				correctPreviousCorner = true;
			}
		}
		else if (correctPreviousCorner || it.nextColor == it.startColor) {
			// means this correct is not correct, but the previous was => apply move
			// or means it's the last iteration => nothing found, do the move anyway to unlock the situation
			// we do it on the last iteration because it will work, alternative to rnd face technique
			Color rightFace = crossPreviousColor(it.crossColor);
			performMove(it.nextColor, rightFace);
			it.stop = true;
			cornerPositioningStep(); // check again and repeat if necessary (move sometimes necessary twice)
		}
		});
}

void Solver::finalChairsStep() {
	// OPTI 
	// 1. side chair
	// 2. rotate diagonal invalid corners face move solution
	bool incorrectPreviousCorner = false;
	executeCCLoop([&](CCLoopIteration it) {
		ColorTriplet corner = _hCube.getCornerAt(Color::Yellow, it.crossColor, it.nextColor);
		// check corner matches corner position (rightly inserted)
		if (!(corner.first == Color::Yellow && corner.second == it.crossColor && corner.third == it.nextColor)
			|| incorrectPreviousCorner) { // if incorrect previous corner and correct current corner => diagonal case
			if (incorrectPreviousCorner) {
				this->performChairMove(it.crossColor, Color::Yellow, false);
				this->performChairMove(crossNextColor(it.nextColor), Color::Yellow, true);
				it.stop = true;
				finalChairsStep(); // continue until all 
			}
			else {
				incorrectPreviousCorner = true;
			}
		}
		});
}

void Solver::simplifyRotations() {
	std::vector<Rotation> simplifiedSolution;
	std::cout << "not simplified:\n" << this->getSolutionPhrase() << std::endl; // temp

	// step 1: add up the rotations
	Rotation lastIgnoredRotation = this->_solution.back();
	lastIgnoredRotation.faceColor = static_cast<Color>(
		(static_cast<int>(lastIgnoredRotation.faceColor) + 1)
		% Cube::FACE_COUNT);
	this->_solution.push_back(lastIgnoredRotation);

	int sameRotationColor = 0;
	for (size_t i = 1; i < this->_solution.size(); i++) {
		const Rotation& previousRotation = this->_solution[i - 1],
			rotation = this->_solution[i];
		
		sameRotationColor += previousRotation.towards;
		
		if (previousRotation.faceColor != rotation.faceColor) {
			Rotation newRotation{previousRotation.faceColor, sameRotationColor};
			simplifiedSolution.push_back(previousRotation);
			sameRotationColor = 0;
		}
	}

	this->_solution = simplifiedSolution;
}

std::vector<Rotation> Solver::getCubeSolution() {
	if (!this->_solved) {
		this->whiteCrossStep();
		this->whiteCornersStep();
		this->middleLayerStep();
		this->yellowCrossStep();
		this->edgeCongruenceStep();
		this->cornerPositioningStep();
		this->finalChairsStep();
		_solved = true; // temp
		this->simplifyRotations();
	}

	_solved = true;
	return _solution;
}

std::string Solver::getSolutionPhrase() {
	if (!_solved) {
		throw EZRubyException("cube should be solved first");
	}

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
	for (Rotation rotation : _solution) {
		char move = colorMoveMap[rotation.faceColor];
		solutionPhrase << move;

		if (rotation.towards == -1)
			solutionPhrase << '\'';
		else if (rotation.towards == 2)
			solutionPhrase << '2';
		else if (rotation.towards != 1)
			throw EZRubyException("towards cannot be different than +, - or ++");
	}

	return solutionPhrase.str();
}

void Solver::performChairMove(Color sideFace, Color upFace, bool isLeftSide) {
	if (sideFace == Color::White ||
		upFace == Color::White) {
		throw EZRubyException("useless chair move");
	}

	// RU2R'U'RU'R' for right
	// L'U2LUL'UL for left
	int sideTowards = isLeftSide ? -1 : 1;

	this->recordRotationSequence({
		{ sideFace, sideTowards },
		{ upFace, 2 },
		{ sideFace, -sideTowards },
		{ upFace, -sideTowards },
		{ sideFace, sideTowards },
		{ upFace, -sideTowards },
		{ sideFace, -sideTowards } });
}

void Solver::executeCCLoop(std::function<void(CCLoopIteration)> executeOp) {
	const Color startColor = Color::Red;
	Color crossColor = startColor;
	int index = 0;
	bool stop = false;

	do {
		Color nextColor = crossNextColor(crossColor);
		executeOp({ startColor, crossColor, nextColor, index, stop });
		crossColor = nextColor;
		index++;
	} while (crossColor != startColor && !stop);
}

void Solver::recordRotation(Color faceColor, int towards) {
	_hCube.performRotation(faceColor, towards);
	_solution.push_back({ faceColor, towards });
}

void Solver::recordRotationSequence(std::initializer_list<Rotation> rotations) {
	_hCube.performRotationSequence(rotations);
	_solution.insert(_solution.end(), rotations.begin(), rotations.end());
}

Color Solver::crossDistantColor(Color color, int distance) {
	// order: right if top side is white, left if yellow
	Color crossColors[] = { Color::Red, Color::Blue, Color::Orange, Color::Green };

	int colorIndex;
	for (colorIndex = 0;
		crossColors[colorIndex] != color;
		++colorIndex) {
		if (colorIndex >= CROSS_COLOR_COUNT) {
			throw EZRubyException("input color should be a cross color");
		}
	}

	int nextColorIndex = (colorIndex + distance) % CROSS_COLOR_COUNT;
	return crossColors[nextColorIndex];
}
Color Solver::crossNextColor(Color color) { return crossDistantColor(color, 1); }
Color Solver::crossPreviousColor(Color color) { return crossDistantColor(color, -1); }

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
		throw EZRubyException();
}