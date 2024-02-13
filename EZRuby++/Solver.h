#pragma once
#include "Cube.h"
#include "BasicTypes.h"
#include <vector>
#include <functional>
#include <string>

namespace EzRuby {
	class Solver {
	private:
		static const int CROSS_COLOR_COUNT = 4;

		Cube _hCube;
		std::vector<Rotation> _solution;
		bool _solved = false;

		static Color crossDistantColor(Color color, int distance);
		static Color crossNextColor(Color color);
		static Color crossPreviousColor(Color color);
		static Color crossGreatestColor(Color color1, Color color2);

		void recordRotation(Color faceColor, int towards);
		void recordRotationSequence(std::initializer_list<Rotation> rotations);
		/// <summary>
		/// Perform chair move
		/// </summary>
		/// <param name="sideFace">the side where the chair is</param>
		/// <param name="upFace">the side where the man walks</param>
		/// <param name="isLeftSide">side is considered right if false, left if true</param>
		void performChairMove(Color frontFace, Color upFace, bool left);

		/// <summary>
		/// Execute loop iterating on all cross colors
		/// </summary>
		/// <param name="executeOp">the operation to be executed at each iteration</param>
		void executeCCLoop(std::function<void(CCLoopIteration)> executeOp); // loop on cross colors

		void whiteCrossStep();
		void whiteCornersStep();
		void middleLayerStep();
		void yellowCrossStep();
		void edgeCongruenceStep();
		void cornerPositioningStep();
		void finalChairsStep();
		void simplifyRotations();

	public:
		Solver() = delete;
		Solver(const Solver& solver) = delete;
		Solver(Cube& cubeToSolve) : _hCube(cubeToSolve)
		{
		}

		std::vector<Rotation> getCubeSolution();
		std::string getSolutionPhrase();
	};
}