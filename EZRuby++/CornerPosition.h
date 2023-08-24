#pragma once
#include "BasicTypes.h"

namespace EzRuby {
	class ColorTriplet {
	public:
		Color first;
		Color second;
		Color third;

		bool contains(Color color) {
			return first == color || second == color || third == color;
		}

		ColorTriplet(const Color& face1Color, const Color& face2Color, const Color& face3Color)
			: first(face1Color), second(face2Color), third(face3Color) {
		}

		ColorTriplet() = default;
	};
}

