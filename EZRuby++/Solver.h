#pragma once
#include "Cube.h"
#include "BasicTypes.h"
#include <vector>
#include <functional>

namespace EzRuby {
	class Solver {
	private:
		static const int CROSS_COLOR_COUNT = 4;

		Cube& _hCube;
		std::vector<MoveOrientation> _solution;

		// order: red green orange blue, right if top side is white, left if yellow
		//static ColorPair extractCrossColors(Color color1, Color color2, Color color3);
		static Color crossNextColor(Color color);
		static Color crossPreviousColor(Color color);
		static Color crossGreatestColor(Color color1, Color color2); 

		void performChairMove(Color frontFace, bool left);
		void executeCCLoop(std::function<void(CCLoopIteration)> executeOp); // loop on cross colors

		void whiteCrossStep();
		void whiteCornersStep();
		void middleLayerStep();
		void yellowCrossStep();
		void edgeCongruenceStep();

	public:
		Solver() = delete;
		Solver(const Solver& solver) = delete;
		Solver(Cube& cubeToSolve) : _hCube(cubeToSolve) // by copy constructor
		{
		}

		std::vector<MoveOrientation> getCubeSolution();
	};
}