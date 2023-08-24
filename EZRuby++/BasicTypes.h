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
}