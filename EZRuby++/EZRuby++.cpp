#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "Viewer.h"



enum Move {
	U, D, R, L, F, B
};

enum Color {
	Red, Green, Orange, Blue, Yellow, White
};

class Cube {
public:
	static const int FACE_SQ_COUNT = 9;
	static const int FACE_COUNT = 6;
	static const int SQ_COUNT = FACE_SQ_COUNT * FACE_COUNT - FACE_COUNT; // 48

private:
	Color _sqArr[SQ_COUNT];

	int sideNeighbor(int sqIndex) {
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
	Color indexFaceBelonging(int index) {
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

	std::tuple<Color, Color> get2dPos(Color color1, Color color2) {
		// we first get the indices where this edge is located
		int sq1Index, sq2Index;
		bool noFailure = false;

		for (sq1Index = 1; sq1Index < SQ_COUNT; sq1Index += 2) {
			sq2Index = sideNeighbor(sq1Index);
			if (_sqArr[sq1Index] == color1 && _sqArr[sq2Index] == color2) {
				noFailure = true;
				break;
			}
		}

		if (!noFailure)
			throw std::exception();

		// then, we determine on which face these indices belong
		Color face1Color = indexFaceBelonging(sq1Index);
		Color face2Color = indexFaceBelonging(sq2Index);
		auto retTuple = std::make_tuple(face1Color, face2Color);
		return retTuple;
	}
};

class Solver {
private:
	Cube _hCube;
	std::vector<Move> _solution;


public:
	Solver(const Cube& cubeToSolve) : _hCube(cubeToSolve) // by copy constructor
	{
	}

	std::vector<Move> getCubeSolution() {
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
	}
};

int main()
{
	Viewer viewer;
	int result = viewer.ShowWindow();
	return result;
}

