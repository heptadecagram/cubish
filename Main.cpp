// File: Main.cpp
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.04.19
//
// Description: This file contains the main() function for the program
// _Liam's_Cubish_.  It creates a new Cube object and six Color objects,
// stored in an array.

#include <stdio.h>
#include <string.h>

#include "Color.h"
#include "Cube.h"
#include "Output.h"

int main(int Argument_Count, char** Arguments) {

	// Declare a default cube size
	int Width=3, Height=3, Depth=3 ;

	// Create the color list and default colors
	Color *Side_Colors[6] ;
	Side_Colors[0]=new Color(1, 0, 1) ;
	Side_Colors[1]=new Color(1, 1, 1) ;
	Side_Colors[2]=new Color(0, 0, 1) ;
	Side_Colors[3]=new Color(1, 0, 0) ;
	Side_Colors[4]=new Color(0, 1, 0) ;
	Side_Colors[5]=new Color(1, 1, 0) ;

	// Behold the Cube
	Cube *Alpha=new Cube(Width, Height, Depth, Side_Colors[0], Side_Colors[1], Side_Colors[2],
			Side_Colors[3], Side_Colors[4], Side_Colors[5]) ;

	Initialize_Window(Argument_Count, Arguments, Alpha, Side_Colors) ;

	glutMainLoop() ;

	return EXIT_SUCCESS ;
}
