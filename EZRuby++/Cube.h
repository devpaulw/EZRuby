#pragma once
#include "BasicTypes.h"
#include "EdgePosition.h"
#include "CornerPosition.h"
#include <array>
#include <vector>
#include <initializer_list>

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
		void cornerSqNeighbors(int sqIndex, int* neighbor1, int* neighbor2) const;

	public:
		Cube() = delete;
		Cube(std::array<Color, SQ_COUNT> sqArr);
		Cube(const Cube& cube);

		inline Color getColorAt(int index) const { return _sqArr[index]; }

		// It returns where on which face color1 is located, same for color2.
		// TODO: This point has to be explicit because it is important, make sure it's easy to figure out this point 
		ColorPair locateEdgePos(Color color1, Color color2) const;
		ColorTriplet locateCornerPos(Color color1, Color color2, Color color3) const;
		ColorTriplet getCornerAt(Color color1, Color color2, Color color3) const;
		// HTBD Make a method that returns a "face index" which is static_cast<int> of Color

		void performRotation(Color faceColor, int towards);
		void performRotationSequence(std::initializer_list<Rotation> rotations);
	};

}