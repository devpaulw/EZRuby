#include "Cube.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "ezruby_exception.h"
#include <array>
#include <functional>
#include <vector> // TEMP maybe temporary
#include <set>
#include "Viewer.h"
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
	std::copy(std::begin(sqArr), std::end(sqArr), std::begin(_sqArr));
}

// It returns where on which face color1 is located, same for color2.
// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 

ColorPair Cube::getEdgePosition(Color color1, Color color2) const {
	// we first get the indices where this edge is located
	int sq1Index, sq2Index;
	bool found = false;

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
	for (sq1Index = 1; sq1Index < SQ_COUNT && !found; sq1Index += nextIndexDistance(sq1Index)) {
		sq2Index = edgeSqNeighbor(sq1Index);
		if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
			found = true;
		}
	}
	if (!found)
		throw EZRubyException("get edge pos failure");

	// then, we determine on which face these indices belong
	Color face1Color = indexBelongingFace(sq1Index);
	Color face2Color = indexBelongingFace(sq2Index);
	ColorPair ret(face1Color, face2Color);
	return ret;
}

ColorTriplet Cube::findCornerPosition(Color color1, Color color2, Color color3) {
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

ColorTriplet Cube::getCornerAt(Color color1, Color color2, Color color3) {
	using FaceColors = std::array<Color, 3>;
	using Indices = std::array<int, 3>;

	std::map<FaceColors, Indices> cornerIndexMap = {
		{{Color::Red,Color::Blue,Color::White}, {5, 16, 10}},
		{{Color::Red,Color::White,Color::Green}, {7, 18, 24}},
	};
	
	FaceColors colors = { color1, color2, color3 };
	auto compareColors = [](Color c1, Color c2) {
		return static_cast<int>(c1) < static_cast<int>(c2);
	};
	std::sort(colors.begin(), colors.end(), compareColors);
	Indices indices = cornerIndexMap[colors];
	std::vector<int> cornerIndices;

	for (const Color& color : colors) {
		// find color in faceColors
		Color faceColor;
		int fcIndex = -1;
		do {
			++fcIndex;
			faceColor = colors[fcIndex];
		} while (faceColor != color);

		if (fcIndex > 2)
			throw EZRubyException("fxIndex should not exceed 2");

		int cubeIndex = indices[fcIndex];
		cornerIndices.push_back(cubeIndex);
	}

	Color cornerColor1 = indexBelongingFace(cornerIndices[0]);
	Color cornerColor2 = indexBelongingFace(cornerIndices[1]);
	Color cornerColor3 = indexBelongingFace(cornerIndices[2]);

	ColorTriplet ret(cornerColor1, cornerColor2, cornerColor3);
	return ret;
	// TODO This algo was made really fast, check it!!!

	//auto getIndices = [&](const Color& c1, const Color& c2, const Color& c3) -> indices {
	//	// Parcourir chaque entr�e dans la map axl.
	//	for (const auto& entry : axl) {
	//		const faceColors& key = entry.first;
	//		const indices& value = entry.second;

	//		// Cr�er une copie de key et value que nous pouvons modifier.
	//		faceColors keyCopy = key;
	//		indices valueCopy = value;

	//		// Rechercher chaque couleur dans la cl�.
	//		for (int i = 0; i < 3; ++i) {
	//			// Si nous trouvons une correspondance pour toutes les couleurs ...
	//			if ((keyCopy[i] == c1 && keyCopy[(i + 1) % 3] == c2 && keyCopy[(i + 2) % 3] == c3) ||
	//				(keyCopy[i] == c3 && keyCopy[(i + 1) % 3] == c2 && keyCopy[(i + 2) % 3] == c1)) {
	//				// ... alors retourner les indices dans le m�me ordre.
	//				return { valueCopy[i], valueCopy[(i + 1) % 3], valueCopy[(i + 2) % 3] };
	//			}
	//		}
	//	}

	//	// Si aucune correspondance n'a �t� trouv�e, retourner une valeur par d�faut.
	//	return { 0, 0, 0 };
	//};
}

void Cube::rotateFace(Color faceColor, int towards) {
	static bool nativeCall = true;
	if (nativeCall && LOG_OUTPUT)
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
		sideSquares = { 34, 33, 32, 26, 25, 24, 18, 17, 16, 10, 9, 8 };
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
		sideSquares = { 21, 22, 23, 29, 30, 31, 37, 38, 39, 13, 14, 15 };
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