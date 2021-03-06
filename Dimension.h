#ifndef CUBISH_DIMENSION
#define CUBISH_DIMENSION

// This file defines the use for a OpenGL window that companions the Cube project
// and allows for changes in size to the cube.  Dimension_Window() will create
// the window.  All other functions are standard GLUT callback functions.

#include <array>

#include "Tile.h"

// Tracks whether the dimension picker window is open
extern bool Is_Dimension_Window_Open;
extern int Dimension_Window_ID;

void Dimension_Window(int Dimension, std::array<Color, 6> Color_List);

void Dimension_Display();

void Dimension_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Dimension_Reshape(int X_Size, int Y_Size);

void Dimension_Passive_Motion(int X_Coord, int Y_Coord);

#endif
