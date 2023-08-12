#pragma once
#include "BasicTypes.h"
#include "EdgePosition.h"

namespace EzRuby {
	class Cube {
	public:
		static const int FACE_SQ_COUNT = 8; // we don't take the middle square into account because it's static
		static const int FACE_COUNT = 6;
		static const int SQ_COUNT = FACE_SQ_COUNT * FACE_COUNT; // 48

	private:
		Color _sqArr[SQ_COUNT];

		// Edge square neighbor
		int edgeSqNeighbor(int sqIndex);
		Color indexBelongingFace(int index);

	public:
		Cube();
		Cube(char strRepr[SQ_COUNT]);
		Cube(const Cube& cube) = default; // For now

		// It returns where on which face color1 is located, same for color2.
		// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 
		EdgePosition getEdgePos(Color color1, Color color2);
		void rotateFace(Color faceColor, int towards);
		// order: blue red green orange, right if top side is yellow, left if white
		Color crossNextColor(Color color);
		Color crossGreaterColor(Color color1, Color color2);
	};

}