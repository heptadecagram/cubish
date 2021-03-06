
#include <array>

#include "ui.h"

#include "Color.h"
#include "Cube.h"
#include "Output.h"

// Description: This file contains the main() function for the program
// _Liam's_Cubish_.  It creates a new Cube object and six Color objects,
// stored in an array.
int main(int argc, char* argv[]) {

	// Create the color list and default colors
	std::array<Color, 6> colors{
		{
			Color{1, 0, 1},
			Color{1, 1, 1},
			Color{0, 0, 1},
			Color{1, 0, 0},
			Color{0, 1, 0},
			Color{1, 1, 0}
		}
	};

	// Behold the Cube
	// Declare a default cube size of 3x3x3
	auto Alpha=Cube(3, 3, 3, colors);

	Initialize_Window(argc, argv, Alpha, colors);

	glutMainLoop();
}
