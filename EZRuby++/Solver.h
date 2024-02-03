#pragma once
#include "Cube.h"
#include "BasicTypes.h"
#include <vector>

namespace EzRuby {
	class Solver {
	private:
		static const int CROSS_COLOR_COUNT = 4;

		Cube& _hCube;
		std::vector<MoveOrientation> _solution;

		// order: red green orange blue, right if top side is white, left if yellow
		//static ColorPair extractCrossColors(Color color1, Color color2, Color color3);
		static Color crossNextColor(Color color);
		static Color crossGreatestColor(Color color1, Color color2);
		void step1();
		void step2();
		void step3();
		void step4();

	public:
		Solver() = delete;
		Solver(const Solver& solver) = delete;
		Solver(Cube& cubeToSolve) : _hCube(cubeToSolve) // by copy constructor
		{
		}

		std::vector<MoveOrientation> getCubeSolution();
	};
}