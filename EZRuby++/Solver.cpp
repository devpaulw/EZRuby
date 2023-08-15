#include "Solver.h"
#include <map>
#include <iostream>

using namespace EzRuby;

std::vector<MoveOrientation> EzRuby::Solver::getCubeSolution() {
	_solution.clear(); // to avoid issues when the method is called many times

	Color whiteColor = Color::White; // todo just use Color::White directly
	Color crossColor = Color::Red;
	EdgePosition edgePos = _hCube.getEdgePos(whiteColor, crossColor);

	const int sideColorCount = 4;
	for (size_t i = 0; i < 2; i++) // vely temp
	{ // move 1, aiming to bring the edge towards yellow face
		if (!edgePos.contains(Color::Yellow)) { // because if it's yellow we do nothing
			Color rotatingFace;
			int rot1Count;

			if (edgePos.contains(Color::White)) {
				rotatingFace = edgePos.face1Color == Color::White ? edgePos.face2Color : edgePos.face1Color; // rotating the color that is not white
				rot1Count = 2;
			}
			else { // on the side
				// choosing the rightmost face to be rotated
				rot1Count = -1;
				rotatingFace = _hCube.crossGreatestColor(edgePos.face1Color, edgePos.face2Color);
			} // WARN: Not sure it will work, review this piece of code.
			
			_hCube.rotateFace(rotatingFace, rot1Count); // TODO: Instead of just that, make a method like "RegisterRotation" that both rotates the face and save the move
		}
	}

	//{ // move 2
	//  // technique: rotate until one of the edge square touches right face
	//  // OPTI: can be -1 instead of 3. But this can be done at the move sequences process 
	//	int rot2Count = 0;
	//	EdgePosition edgePos = _hCube.getEdgePos(whiteColor, crossColor);
	//	while (!edgePos.contains(crossColor)) {
	//		_hCube.rotateFace(Color::Yellow, 1);
	//		if (++rot2Count >= sideColorCount) {
	//			throw std::exception("yellow face rotation count should not exceed 3");
	//		}
	//		edgePos = _hCube.getEdgePos(whiteColor, crossColor);
	//	}
	//}

	//{ // move 3 
	//	EdgePosition edgePos = _hCube.getEdgePos(whiteColor, crossColor);

	//	if (edgePos.face1Color != Color::Yellow) {
	//		_hCube.rotateFace(crossColor, 1);
	//		Color nextColor = _hCube.crossNextColor(crossColor);
	//		_hCube.rotateFace(nextColor, 1); // rotate right face (yellow up)
	//		_hCube.rotateFace(Color::Yellow, 1);
	//	}

	//	// final move
	//	_hCube.rotateFace(crossColor, 2);
	//}

	return std::vector<MoveOrientation>(); // temp
}