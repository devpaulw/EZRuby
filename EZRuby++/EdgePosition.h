#pragma once
#include "BasicTypes.h"

namespace EzRuby {
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
}
