#include <iostream>
#include "Viewer.h"

using namespace EzRuby;

int main() {
	Viewer viewer(nullptr);
	int result = viewer.showWindow();
	return result;
}