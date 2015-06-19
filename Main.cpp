
#include <cstdlib>

#include "ui.h"

#include "Color.h"
#include "Cube.h"
#include "Output.h"

// Description: This file contains the main() function for the program
// _Liam's_Cubish_.  It creates a new Cube object and six Color objects,
// stored in an array.
int main(int Argument_Count, char** Arguments) {

	// Declare a default cube size
	auto Width=3, Height=3, Depth=3;

	// Create the color list and default colors
	Color_p Side_Colors[6];
	Side_Colors[0]= std::make_shared<Color>(1, 0, 1);
	Side_Colors[1]= std::make_shared<Color>(1, 1, 1);
	Side_Colors[2]= std::make_shared<Color>(0, 0, 1);
	Side_Colors[3]= std::make_shared<Color>(1, 0, 0);
	Side_Colors[4]= std::make_shared<Color>(0, 1, 0);
	Side_Colors[5]= std::make_shared<Color>(1, 1, 0);

	// Behold the Cube
	auto Alpha=Cube(Width, Height, Depth, Side_Colors[0], Side_Colors[1], Side_Colors[2],
			Side_Colors[3], Side_Colors[4], Side_Colors[5]);

	Initialize_Window(Argument_Count, Arguments, Alpha, Side_Colors);

	glutMainLoop();

	return EXIT_SUCCESS;
}
