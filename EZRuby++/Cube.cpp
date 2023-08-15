#include "Cube.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "ezruby_exception.h"
#include <array>
using namespace EzRuby;

int EzRuby::Cube::edgeSqNeighbor(int sqIndex) const {
	std::map<int, int> neighborMap = {
	{1, 35}, {3, 9}, {4, 25}, {6, 17},
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

	Color faceColorOrder[] = {Color::Red, Color::Blue, Color::White, Color::Green, Color::Yellow, Color::Orange};
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
		throw EZRubyException();

	// then, we determine on which face these indices belong
	Color face1Color = indexBelongingFace(sq1Index);
	Color face2Color = indexBelongingFace(sq2Index);
	EdgePosition ret(face1Color, face2Color);
	return ret;
}

void EzRuby::Cube::rotateFace(Color faceColor, int towards) {
	std::cout << "Rotating " << static_cast<int>(faceColor) << " by " << towards << std::endl;
}

// order: blue red green orange, right if top side is yellow, left if white
Color EzRuby::Cube::crossNextColor(Color color) const {
	return Color::Blue;
}

Color EzRuby::Cube::crossGreatestColor(Color color1, Color color2) const {
	std::map<Color, int> sideColorOrder = {
	{ Color::Blue, 0 },{ Color::Red, 1 },{ Color::Green, 2 },{ Color::Orange, 3 }};

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
