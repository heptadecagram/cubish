//
// This file defines the use for a OpenGL window that companions the Cube project
// and allows for changes in size to the cube.  Dimension_Window() will create
// the window.  All other functions are standard GLUT callback functions.

#ifndef CUBISH_DIMENSION
#define CUBISH_DIMENSION

#include <array>

#include "Tile.h"

void Dimension_Window(int Dimension, std::array<Color_p, 6> Color_List);

void Dimension_Display();

void Dimension_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Dimension_Reshape(int X_Size, int Y_Size);

void Dimension_Passive_Motion(int X_Coord, int Y_Coord);

#endif // CUBISH_DIMENSION
