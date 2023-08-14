#include "Cube.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "ezruby_exception.h"
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
	if (index >= 0 && index <= 7)
		return Color::Red;
	else if (index >= 8 && index <= 15)
		return Color::Blue;
	// not done!
	else {
		throw std::exception();
	}
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

	auto nextIndex = [](int x) {
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
	for (sq1Index = 1; sq1Index < SQ_COUNT; sq1Index += nextIndex(sq1Index)) {
		sq2Index = edgeSqNeighbor(sq1Index);
		if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
			noFailure = true;
			break;
		}
	}

	if (!noFailure)
		throw EZRubyException();

	// then, we determine on which face these indices belong
	Color face1Color = Color::White;// indexBelongingFace(sq1Index);
	Color face2Color = Color::Yellow; //indexBelongingFace(sq2Index);
	EdgePosition ret(face1Color, face2Color);
	return ret;
}

void EzRuby::Cube::rotateFace(Color faceColor, int towards) {
	
}

// order: blue red green orange, right if top side is yellow, left if white
Color EzRuby::Cube::crossNextColor(Color color) const {
	return Color::Blue;
}

Color EzRuby::Cube::crossGreatestColor(Color color1, Color color2) const {
	return Color::Blue;
}
