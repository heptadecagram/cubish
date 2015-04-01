// File: ColorPick.h
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.04.19
//
// Description: Creates a simple window that allows the user to pick a new 
// color, which is stored at the address of the color passed to Color_Window().

#ifndef LIAM_COLORPICK
#define LIAM_COLORPICK

#include "OptionWindow.h"

void Color_Window(Color *Old_Color) ;

void Color_Display(void) ;

void Color_Mouse(int Button, int State, int X_Coord, int Y_Coord) ;

void Color_Reshape(int X_Size, int Y_Size) ;

#endif // LIAM_COLORPICK
