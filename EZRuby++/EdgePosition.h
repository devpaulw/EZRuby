#pragma once
#include "BasicTypes.h"

namespace EzRuby {
	class ColorPair { // TODO Naming changed, so adapt all the variables.
	public:
		Color first;
		Color second;

		bool contains(Color color) {
			return first == color || second == color;
		}

		ColorPair(const Color& face1Color, const Color& face2Color)
			: first(face1Color), second(face2Color) {
		}

		ColorPair() = default;
	};
}
