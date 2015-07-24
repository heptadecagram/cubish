//
// Description: This file defines a window that provides user options to a
// Cube.  There are methods for changing color and size, randomizing and
// solving, and saving and loading.

#ifndef LIAM_OPTIONWINDOW
#define LIAM_OPTIONWINDOW

#include "Tile.h"

void New_Window(Color_p *Color_Array);

void Option_Display();

void Option_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Option_Passive_Motion(int X_Coord, int Y_Coord);

void Option_Reshape(int X_Size, int Y_Size);

int Make_Option_Cube_List(Color_p *Color_Array);

int Make_Option_Exit_List();

int Make_Option_Solved_List();

int Make_Option_Random_List();

int Make_Option_Line_List();

int Make_Option_Load_List();

int Make_Option_Save_List();

void Make_All_Option_Lists();

#endif // LIAM_OPTIONWINDOW
