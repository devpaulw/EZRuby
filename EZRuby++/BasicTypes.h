#pragma once

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
		Red, Green, Orange, Blue, Yellow, White
	};
}