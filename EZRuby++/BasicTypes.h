#pragma once
constexpr bool LOG_OUTPUT = false;

namespace EzRuby {
	enum Color {
		Red = 0, Blue = 1, White = 2, Green = 3, Yellow = 4, Orange = 5
	};

	struct Rotation {
		Color faceColor;
		int towards; // either -, + or ++. For example: U', U or U2
	};

	struct CCLoopIteration {
		Color startColor;
		Color crossColor;
		Color nextColor;
		int index;
		bool& stop;
	};
	//bool operator==(const Color& color1, const Color& color2) {
	//	// Logique de comparaison personnalisée pour les couleurs
	//	// Par exemple, comparer les valeurs entières des enum
	//	return static_cast<int>(color1) == static_cast<int>(color2);
	//}
	//bool operator==(int color1, const Color& color2) {
	//	// Logique de comparaison personnalisée pour les couleurs
	//	// Par exemple, comparer les valeurs entières des enum
	//	return color1 == static_cast<int>(color2);
	//}
	//bool operator==(const Color& color1, int color2) {
	//	// Logique de comparaison personnalisée pour les couleurs
	//	// Par exemple, comparer les valeurs entières des enum
	//	return static_cast<int>(color1) == color2;
	//}
}