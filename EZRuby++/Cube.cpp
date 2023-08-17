#include "Cube.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "ezruby_exception.h"
#include <array>
#include <functional>
using namespace EzRuby;

int EzRuby::Cube::edgeSqNeighbor(int sqIndex) const {
	std::map<int, int> neighborMap = {
	{1, 33}, {3, 9}, {4, 25}, {6, 17},
	{9, 3}, {11, 36}, {12, 19}, {14, 43},
	{17, 6}, {19, 12}, {20, 27}, {22, 41},
	{25, 4}, {27, 20}, {28, 35}, {30, 44},
	{33, 1}, {35, 28}, {36, 11}, {38, 46},
	{41, 22}, {43, 14}, {44, 30}, {46, 38}
	};

	return neighborMap[sqIndex];
}

Color EzRuby::Cube::indexBelongingFace(int index) const {
	if (index < 0 || index > 48)
		throw EZRubyException();

	Color faceColorOrder[] = { Color::Red, Color::Blue, Color::White, Color::Green, Color::Yellow, Color::Orange };
	const int indexOnFace = index / FACE_SQ_COUNT;
	Color ret = faceColorOrder[indexOnFace];
	return ret;

}

inline EzRuby::Cube::Cube() {
	{ for (size_t i = 0; i < SQ_COUNT; i++) _sqArr[i] = Color::Blue; } // Note: this is definitely temporary
}

EzRuby::Cube::Cube(std::array<Color, SQ_COUNT> sqArr) {
	std::copy(std::begin(sqArr), std::end(sqArr), std::begin(_sqArr));
}

// It returns where on which face color1 is located, same for color2.
// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 

EdgePosition EzRuby::Cube::getEdgePos(Color color1, Color color2) const {
	// we first get the indices where this edge is located
	int sq1Index, sq2Index;
	bool noFailure = false;

	auto nextIndexDistance = [](int x) {
		int indexOnFace = x % FACE_SQ_COUNT;
		switch (indexOnFace) {
		case 3:
			return 1;
		case 6:
			return 3;
		case 4:
		case 1:
			return 2;
		default:
			throw EZRubyException("The square index is not from an edge");
		}
	};
	for (sq1Index = 1; sq1Index < SQ_COUNT; sq1Index += nextIndexDistance(sq1Index)) {
		sq2Index = edgeSqNeighbor(sq1Index);
		if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
			noFailure = true;
			break;
		}
	}
	if (!noFailure)
		throw EZRubyException("get edge pos failure");

	// then, we determine on which face these indices belong
	Color face1Color = indexBelongingFace(sq1Index);
	Color face2Color = indexBelongingFace(sq2Index);
	EdgePosition ret(face1Color, face2Color);
	return ret;
}

void Cube::rotateFace(Color faceColor, int towards) {
	static bool nativeCall = true; // TEMP
	if (nativeCall)
		std::cout << "Rotating " << static_cast<int>(faceColor) << " by " << towards << std::endl; // TEMP

	// this method is quite wierd but it's working
	if (towards == 0)
		return; // No need to do anything in this case 
	else if (towards == 2) {
		nativeCall = false;
		rotateFace(faceColor, 1);
		rotateFace(faceColor, 1);
		nativeCall = true;
		return;
	}
	else if (towards != -1 && towards != 1)
		throw EZRubyException("Rotation count should be between -1 and 2 (-, + or ++)");
	// At this stage after the above checks, towards can be either -1 or 1

	const size_t sideSqCount = 12;
	std::array<int, sideSqCount> sideSquares;
	int faceIndex;
	switch (faceColor) {
	case Color::Red:
		faceIndex = 0;
		sideSquares = { 45, 46, 47, 26, 25, 24, 18, 17, 16, 10, 9, 8 };
		break;
	case Color::Blue:
		faceIndex = 1;
		sideSquares = { 0, 3, 5, 16, 19, 21, 40, 43, 45, 39, 36, 34 };
		break;
	case Color::White:
		faceIndex = 2;
		sideSquares = { 5, 6, 7, 24, 27, 29, 42, 41, 40, 15, 12, 10 };
		break;
	case Color::Green:
		faceIndex = 3;
		sideSquares = { 7, 4, 2, 32, 35, 37, 47, 44, 42, 23, 20, 18 };
		break;
	case Color::Yellow:
		faceIndex = 4;
		sideSquares = { 2, 1, 0, 8, 11, 13, 45, 46, 47, 31, 28, 26 };
		break;
	case Color::Orange:
		faceIndex = 5;
		sideSquares = { 21, 22, 23, 29, 30, 31, 2, 1, 0, 13, 14, 15 };
		break;
	default:
		throw EZRubyException("Wrong color");
	} // TODO Check if nothing is wrong

	std::array<int, FACE_SQ_COUNT> frontSquaresGap = { 0, 1, 1, 2, 3, -1, -1, -2 };
	
	auto stepFS = [&](size_t iterCount, int gap, std::function<int(int)> sqArrIndexLambda) {// front and sides
		Color *newPlacement = new Color[iterCount]; // new colors placement

		for (size_t i = 0; i < iterCount; i++) {
			int sqArrIndex = sqArrIndexLambda(i);
			int npIndex = (i + gap + iterCount) % iterCount;
			newPlacement[npIndex] = _sqArr[sqArrIndex];
		}
		
		// concrete new color attribution
		for (size_t i = 0; i < iterCount; i++) {
			int sqArrIndex = sqArrIndexLambda(i);
			Color colorAttrib = newPlacement[i];
			_sqArr[sqArrIndex] = colorAttrib;
		}

		delete[] newPlacement;
	};

	// step 1 - sides
	int gap = 3 * towards;
	auto step1SqArrIndex = [&](int i) { return sideSquares[i]; };
	stepFS(sideSqCount, gap, step1SqArrIndex);

	// step 2 - front
	gap = 2 * towards;
	auto step2SqArrIndex = [&](int i) {
		int sqIndex = faceIndex * FACE_SQ_COUNT;
		for (size_t j = 0; j <= i; j++) 
			sqIndex += frontSquaresGap[j];
		return sqIndex;
	};
	stepFS(FACE_SQ_COUNT, gap, step2SqArrIndex);
}

// order: blue red green orange, right if top side is yellow, left if white
Color EzRuby::Cube::crossNextColor(Color color) const {
	return Color::Blue; // TODO: Question: is it really useful?
}

Color EzRuby::Cube::crossGreatestColor(Color color1, Color color2) const {
	std::map<Color, int> sideColorOrder = {
	{ Color::Blue, 0 },{ Color::Red, 1 },{ Color::Green, 2 },{ Color::Orange, 3 } };

	int face1Rank = sideColorOrder[color1],
		face2Rank = sideColorOrder[color2];

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
