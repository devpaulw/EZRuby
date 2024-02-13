#include "Cube.h"
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "ezruby_exception.h"
#include <array>
#include <functional>
#include <queue>
#include "Viewer.h"
using namespace EzRuby;

int EzRuby::Cube::edgeSqNeighbor(int sqIndex) const {
	static std::unordered_map<int, int> neighborMap = {
		{1, 33}, {3, 9}, {4, 25}, {6, 17},
		{9, 3}, {11, 36}, {12, 19}, {14, 43},
		{17, 6}, {19, 12}, {20, 27}, {22, 41},
		{25, 4}, {27, 20}, {28, 35}, {30, 44},
		{33, 1}, {35, 28}, {36, 11}, {38, 46},
		{41, 22}, {43, 14}, {44, 30}, {46, 38}
	};

	if (neighborMap.find(sqIndex) == neighborMap.end()) {
		throw EZRubyException("This square index is not from an edge");
	}

	return neighborMap[sqIndex];
}

void Cube::cornerSqNeighbors(int sqIndex, int* neighbor1, int* neighbor2) const {
	// less optimised than edgeSqNeighbor but simpler data type
	const size_t cornerCount = 8,
		cornerSqCount = 3;

	int cornerIndices[cornerCount][cornerSqCount] = {
		{16, 5, 10}, {18, 7, 24}, {21, 15, 40}, {23, 29, 42},
		{32, 2, 26}, {34, 0, 8}, {37, 31, 47}, {39, 13, 45}
	};

	bool found = false;
	for (int i = 0; i < cornerCount && !found; i++) {
		for (int j = 0; j < cornerSqCount && !found; j++) {
			if (cornerIndices[i][j] == sqIndex) {
				*neighbor1 = cornerIndices[i][(j + 1) % 3];
				*neighbor2 = cornerIndices[i][(j + 2) % 3];
				found = true;
			}
		}
	}

	if (!found) {
		throw EZRubyException("This square index is not from a corner");
	}
}

Color EzRuby::Cube::indexBelongingFace(int index) const {
	if (index < 0 || index > 48)
		throw EZRubyException();

	Color faceColorOrder[] = { Color::Red, Color::Blue, Color::White, Color::Green, Color::Yellow, Color::Orange };
	const int indexOnFace = index / FACE_SQ_COUNT;
	Color ret = faceColorOrder[indexOnFace];
	return ret;
}

EzRuby::Cube::Cube(std::array<Color, SQ_COUNT> sqArr) {
	std::copy(std::begin(sqArr), std::end(sqArr), _sqArr);
}

EzRuby::Cube::Cube(const Cube& cube) {
	std::copy(std::begin(cube._sqArr), std::end(cube._sqArr), _sqArr);
}

// It returns where on which face color1 is located, same for color2.
// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 

ColorPair Cube::locateEdgePos(Color color1, Color color2) const {
	// we first get the indices where this edge is located
	int sq1Index, sq2Index;

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
	for (sq1Index = 1; true; sq1Index += nextIndexDistance(sq1Index)) {
		sq2Index = edgeSqNeighbor(sq1Index);
		if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
			break; // found
		}
	}
	if (sq1Index >= SQ_COUNT)
		throw EZRubyException("get edge pos failure");

	// then, we determine on which face these indices belong
	Color face1Color = indexBelongingFace(sq1Index);
	Color face2Color = indexBelongingFace(sq2Index);
	ColorPair ret(face1Color, face2Color);
	return ret;
}

ColorTriplet Cube::locateCornerPos(Color color1, Color color2, Color color3) const {
	// almost same principle than get edgePos
	int sq1Index, sq2Index, sq3Index;

	auto nextIndexDistance = [](int x) {
		int indexOnFace = x % FACE_SQ_COUNT;
		switch (indexOnFace) {
		case 0: return 2;
		case 2: return 3;
		case 5: return 2;
		case 7: return 1;
		default:
			throw EZRubyException("The square index is not from a corner");
		}
		};

	for (sq1Index = 0; sq1Index < SQ_COUNT; sq1Index += nextIndexDistance(sq1Index)) {
		cornerSqNeighbors(sq1Index, &sq2Index, &sq3Index);
		if (_sqArr[sq1Index] == color1) {
			if (_sqArr[sq2Index] == color3 && _sqArr[sq3Index] == color2) {
				std::swap(sq2Index, sq3Index); // to keep the correct return order
			}
			else if (!(_sqArr[sq2Index] == color2 && _sqArr[sq3Index] == color3)) {
				// this is if the second condition is not met, then the search continues
				continue;
			}
			// at this pointer the corner has been found
			break;
		}
	}

	bool found = sq1Index < SQ_COUNT; // because if it is not true it means the for loop didn't end by break instruction
	if (!found)
		throw EZRubyException("get corner pos failure");

	// then, we determine on which face these indices belong
	Color face1Color = indexBelongingFace(sq1Index);
	Color face2Color = indexBelongingFace(sq2Index);
	Color face3Color = indexBelongingFace(sq3Index);
	ColorTriplet ret(face1Color, face2Color, face3Color);
	return ret;
}

ColorTriplet Cube::getCornerAt(Color color1, Color color2, Color color3) const {
	const size_t cornerSqCount = 3;
	using FaceColors = std::array<Color, cornerSqCount>;
	using CornerIndices = std::array<int, cornerSqCount>;

	static std::map<FaceColors, CornerIndices> cornerIndexMap = { // face colors have to be sorted!!
		// white
		{{Color::Red,Color::Blue,Color::White}, {5, 10, 16}},
		{{Color::Red,Color::White,Color::Green}, {7, 18, 24}},
		{{Color::White,Color::Green,Color::Orange}, {23, 29, 42}},
		{{Color::Blue,Color::White,Color::Orange}, {15, 21, 40}},
		// yellow
		{{Color::Red,Color::Green,Color::Yellow}, {2, 26, 32}},
		{{Color::Red,Color::Blue,Color::Yellow}, {0, 8, 34}},
		{{Color::Blue,Color::Yellow,Color::Orange}, {13, 39, 45}},
		{{Color::Green,Color::Yellow,Color::Orange}, {31, 37, 47}},
	};

	FaceColors unsortedColors = { color1, color2, color3 };
	FaceColors sortedColors = { color1, color2, color3 };
	auto compareColors = [](const Color& c1, const Color& c2) {
		return static_cast<int>(c1) < static_cast<int>(c2);
		};
	std::sort(sortedColors.begin(), sortedColors.end(), compareColors);
	if (cornerIndexMap.find(sortedColors) == cornerIndexMap.end())
		throw EZRubyException("getCornerAt: this position doesn't exist");
	CornerIndices indices = cornerIndexMap[sortedColors];
	Color cornerColors[cornerSqCount]{}; // the indices of the corner we are going to return

	for (size_t i = 0; i < cornerSqCount; i++) {
		const Color& unsortedColor = unsortedColors[i];
		Color sortedColor;
		int fcIndex = -1;
		do {
			++fcIndex;
			if (fcIndex >= cornerSqCount)
				throw EZRubyException("fxIndex should not exceed 2");
			sortedColor = sortedColors[fcIndex];
		} while (unsortedColor != sortedColor);

		int cubeIndex = indices[fcIndex];
		cornerColors[i] = getColorAt(cubeIndex);
	}

	ColorTriplet ret(cornerColors[0], cornerColors[1], cornerColors[2]);
	return ret;
}

void Cube::performRotation(Color faceColor, int towards) {
	static bool nativeCall = true;
	if (nativeCall && LOG_OUTPUT)
		std::cout << "Rotating " << static_cast<int>(faceColor) << " by " << towards << std::endl; // TEMP

	// this method is quite wierd but it's working
	if (towards == 0)
		return; // No need to do anything in this case 
	else if (towards == 2) {
		nativeCall = false;
		performRotation(faceColor, 1);
		performRotation(faceColor, 1);
		nativeCall = true;
		return;
	}
	else if (towards != -1 && towards != 1)
		throw EZRubyException("Rotation count should be between -1 and 2 (-, + or ++)");
	// At this stage after the above checks, towards can be either -1 or 1

	const size_t sideSqCount = 12;
	std::array<int, sideSqCount> sideSquares;
	int faceIndex = static_cast<int>(faceColor);
	std::array<int, FACE_SQ_COUNT> frontSquaresGap = { 0, 1, 1, 2, 3, -1, -1, -2 };

	static std::unordered_map<Color, std::array<int, sideSqCount>> colorMap = {
		{Color::Red, {34, 33, 32, 26, 25, 24, 18, 17, 16, 10, 9, 8}},
		{Color::Blue, {0, 3, 5, 16, 19, 21, 40, 43, 45, 39, 36, 34}},
		{Color::White, {5, 6, 7, 24, 27, 29, 42, 41, 40, 15, 12, 10}},
		{Color::Green, {7, 4, 2, 32, 35, 37, 47, 44, 42, 23, 20, 18}},
		{Color::Yellow, {2, 1, 0, 8, 11, 13, 45, 46, 47, 31, 28, 26}},
		{Color::Orange, {21, 22, 23, 29, 30, 31, 37, 38, 39, 13, 14, 15}}
	};

	if (colorMap.find(faceColor) != colorMap.end())
		sideSquares = colorMap[faceColor];
	else
		throw EZRubyException("Wrong color");

	auto stepFS = [&](size_t iterCount, int gap, std::function<int(int)> sqArrIndexLambda) {// front and sides
		Color* newPlacement = new Color[iterCount]; // new colors placement
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

void Cube::performRotationSequence(std::initializer_list<Rotation> rotations) {
	for (const Rotation& rotation : rotations) {
		this->performRotation(rotation.faceColor, rotation.towards);
	}
}