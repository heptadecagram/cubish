// File: Dimension.h
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.04.19
//
// This file defines the use for a OpenGL window that companions the Cube project
// and allows for changes in size to the cube.  Dimension_Window() will create
// the window.  All other functions are standard GLUT callback functions.

#ifndef LIAM_DIMENSION
#define LIAM_DIMENSION

#include "OptionWindow.h"
#include "Face.h"

void Dimension_Window(int Dimension, Color **Color_List) ;

void Dimension_Display(void) ;

void Dimension_Mouse(int Button, int State, int X_Coord, int Y_Coord) ;

void Dimension_Reshape(int X_Size, int Y_Size) ;

void Dimension_Passive_Motion(int X_Coord, int Y_Coord) ;

#endif // LIAM_DIMENSION
