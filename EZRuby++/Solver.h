#pragma once
#include "Cube.h"
#include "BasicTypes.h"
#include <vector>

namespace EzRuby {
	class Solver {
	private:
		Cube _hCube;
		std::vector<MoveOrientation> _solution;


	public:
		Solver() = delete;
		Solver(const Solver& solver) = delete;
		Solver(const Cube& cubeToSolve) : _hCube(cubeToSolve) // by copy constructor
		{
		}

		std::vector<MoveOrientation> getCubeSolution();
	};
}