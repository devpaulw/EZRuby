#pragma once
#include "BasicTypes.h"

namespace EzRuby {
	class CornerPosition {
	public:
		Color face1Color;
		Color face2Color;
		Color face3Color;

		bool contains(Color color) {
			return face1Color == color || face2Color == color || face3Color == color;
		}

		CornerPosition(const Color& face1Color, const Color& face2Color, const Color& face3Color)
			: face1Color(face1Color), face2Color(face2Color), face3Color(face3Color) {
		}

		CornerPosition() = default;
	};
}
