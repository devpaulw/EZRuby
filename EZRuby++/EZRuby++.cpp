#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "Viewer.h"



enum MoveOrientation {
	U, D, R, L, F, B
};

struct Move {
	MoveOrientation orientation;
	int towards; // either -, + or ++. For example: U', U or U2
};

enum Color {
	Red, Green, Orange, Blue, Yellow, White
};

class EdgePosition {
public:
	Color face1Color;
	Color face2Color;

	bool contains(Color color) {
		return face1Color == color || face2Color == color;
	}

	EdgePosition(const Color& face1Color, const Color& face2Color)
		: face1Color(face1Color), face2Color(face2Color) {
	}

	EdgePosition() = default;
};

class Cube {
public:
	static const int FACE_SQ_COUNT = 9;
	static const int FACE_COUNT = 6;
	static const int SQ_COUNT = FACE_SQ_COUNT * FACE_COUNT - FACE_COUNT; // 48

private:
	Color _sqArr[SQ_COUNT];

	// Edge square neighbor
	int edgeSqNeighbor(int sqIndex) {
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
	Color indexBelongingFace(int index) {
		if (index >= 0 && index <= 7)
			return Color::Red;
		else if (index >= 8 && index <= 15)
			return Color::Blue;
		// not done!
		else {
			throw std::exception();
		}
	}

public:
	Cube() {
		{ for (size_t i = 0; i < SQ_COUNT; i++) _sqArr[i] = Color::Blue; } // Note: this is definitely temporary
	}
	Cube(const Cube& cube) = default; // For now

	EdgePosition getEdgePos(Color color1, Color color2) {
		// we first get the indices where this edge is located
		int sq1Index, sq2Index;
		bool noFailure = false;

		for (sq1Index = 1; sq1Index < SQ_COUNT; sq1Index += 2) {
			sq2Index = edgeSqNeighbor(sq1Index);
			if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
				noFailure = true;
				break;
			}
		}

		if (!noFailure)
			throw std::exception();

		// then, we determine on which face these indices belong
		Color face1Color = indexBelongingFace(sq1Index);
		Color face2Color = indexBelongingFace(sq2Index);
		EdgePosition ret(face1Color, face2Color);
		return ret;
	}

	void RotateFace(Color faceColor, int towards) {

	}
};

class Solver {
private:
	Cube _hCube;
	std::vector<MoveOrientation> _solution;


public:
	Solver(const Cube& cubeToSolve) : _hCube(cubeToSolve) // by copy constructor
	{
	}

	std::vector<MoveOrientation> getCubeSolution() {
		_solution.clear(); // to avoid issues when the method is called many times

		// first step
		// 1. locate white+red
		// 2. il faut faire une rotation jusqu'a ce que l'arete soit du coté jaune
		// 3. mettre l'arete dans le bon sens
		// 4. 2 rotation pour le remettre en place

		// cube.pos(white, red) => white, green [example]
		// si contains white => 2, si yellow => 0, sinon => 1
		// si cube.pos(white, red) retourne yellow, x => ok. Sinon : x+, right[x]+, yellow-
		// x++

		Color whiteColor = Color::White; // todo just use Color::White directly
		Color sideColor = Color::Red;
		auto edgePos = _hCube.getEdgePos(whiteColor, sideColor);

		const int sideColorCount = 4;

		// move 1
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			Color rotatingFace;
			int rot1Count;

			if (edgePos.contains(Color::White)) {
				rotatingFace = edgePos.face1Color == Color::White ? edgePos.face2Color : edgePos.face1Color;
				rot1Count = 2;
			}
			else { // on the side
				// choosing the left face to be rotated
				rot1Count = -1;
				std::map<Color, int> sideColorOrder = {
					{Color::Blue, 0}, {Color::Red, 1}, {Color::Green, 2}, {Color::Orange, 3}
				};

				int face1Rank = sideColorOrder[edgePos.face1Color],
					face2Rank = sideColorOrder[edgePos.face2Color];

				if (edgePos.contains(Color::Orange) && edgePos.contains(Color::Blue))
					rotatingFace = Color::Orange; // exceptional because orange is actually lower than blue
				else if (face1Rank < face2Rank)
					rotatingFace = edgePos.face1Color;
				else if (face2Rank < face1Rank)
					rotatingFace = edgePos.face2Color;
				else
					throw std::exception();
			} // WARN: Not sure it will work, review this piece of code.

			_hCube.RotateFace(rotatingFace, rot1Count); // TODO: Instead of just that, make a method like "RegisterRotation" that both rotates the face and save the move
		}

		// move 2
		// technique: rotate until one of the edge square touches right face
		while (!_hCube.getEdgePos(whiteColor, sideColor).contains(sideColor)) { // TODO: Just done rapidly but should evolve.
			_hCube.RotateFace(Color::Yellow, 1);
		}
	}
};

int main()
{
	Viewer viewer;
	int result = viewer.ShowWindow();
	return result;
}

