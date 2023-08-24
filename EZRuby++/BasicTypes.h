#pragma once
constexpr bool LOG_OUTPUT = false;

namespace EzRuby {
	// TODO: Is it really a good filename?
	enum MoveOrientation {
		U, D, R, L, F, B
	};

	struct Move {
		MoveOrientation orientation;
		int towards; // either -, + or ++. For example: U', U or U2
	};

	enum Color {
		Red, Blue, White, Green, Yellow, Orange
	};
	//bool operator==(const Color& color1, const Color& color2) {
	//	// Logique de comparaison personnalis�e pour les couleurs
	//	// Par exemple, comparer les valeurs enti�res des enum
	//	return static_cast<int>(color1) == static_cast<int>(color2);
	//}
	//bool operator==(int color1, const Color& color2) {
	//	// Logique de comparaison personnalis�e pour les couleurs
	//	// Par exemple, comparer les valeurs enti�res des enum
	//	return color1 == static_cast<int>(color2);
	//}
	//bool operator==(const Color& color1, int color2) {
	//	// Logique de comparaison personnalis�e pour les couleurs
	//	// Par exemple, comparer les valeurs enti�res des enum
	//	return static_cast<int>(color1) == color2;
	//}
}