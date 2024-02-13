#pragma once
#include "BasicTypes.h"
#include <vector>

namespace EzRuby {
	class SolutionProvider {
	public:
		std::vector<Rotation> simplifyRotations(std::vector<Rotation> solution);
		std::string getSolutionPhrase(std::vector<Rotation> solution);
	};
}

