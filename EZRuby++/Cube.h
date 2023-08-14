#pragma once
#include "BasicTypes.h"
#include "EdgePosition.h"
#include <array>

namespace EzRuby {
	class Cube {
	public:
		static const int FACE_SQ_COUNT = 8; // we don't take the middle square into account because it's static
		static const int FACE_COUNT = 6;
		static const int SQ_COUNT = FACE_SQ_COUNT * FACE_COUNT; // 48

	private:
		Color _sqArr[SQ_COUNT];

		// Edge square neighbor
		int edgeSqNeighbor(int sqIndex) const;
		Color indexBelongingFace(int index) const;

	public:
		Cube();
		Cube(std::array<Color, SQ_COUNT> sqArr);
		Cube(const Cube& cube) = default; // For now

		inline Color getColorAt(int index) const { return _sqArr[index]; }
		// It returns where on which face color1 is located, same for color2.
		// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 
		EdgePosition getEdgePos(Color color1, Color color2) const;
		void rotateFace(Color faceColor, int towards);
		// order: blue red green orange, right if top side is yellow, left if white
		Color crossNextColor(Color color) const;
		Color crossGreatestColor(Color color1, Color color2) const;
	};

}