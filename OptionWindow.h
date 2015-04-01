// File: OptionWindow.h
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.04.18
//
// Description: This file defines a window that provides user options to a 
// Cube.  There are methods for changing color and size, randomizing and 
// solving, and saving and loading.

#ifndef LIAM_OPTIONWINDOW
#define LIAM_OPTIONWINDOW

#include "Output.h"
#include "ColorPick.h"
#include "Dimension.h"

void New_Window(Color **Color_Array) ;

void Option_Display(void) ;

void Option_Mouse(int Button, int State, int X_Coord, int Y_Coord) ;

void Option_Passive_Motion(int X_Coord, int Y_Coord) ;

void Option_Reshape(int X_Size, int Y_Size) ;

int Make_Option_Cube_List(Color **Color_Array) ;

int Make_Option_Exit_List(void) ;

int Make_Option_Solved_List(void) ;

int Make_Option_Random_List(void) ;

int Make_Option_Line_List(void) ;

int Make_Option_Load_List(void) ;

int Make_Option_Save_List(void) ;

void Make_All_Option_Lists(void) ;

#endif // LIAM_OPTIONWINDOW
