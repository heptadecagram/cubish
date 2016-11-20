//
// Description: This file defines a window that provides user options to a
// Cube.  There are methods for changing color and size, randomizing and
// solving, and saving and loading.

#ifndef CUBISH_OPTIONWINDOW
#define CUBISH_OPTIONWINDOW

#include "Tile.h"

// Tracks whether the option window is open
extern bool Is_Option_Window_Open;
extern int Option_Window_ID;

void New_Window(std::array<Color_p, 6> Color_Array);

void Option_Display();

void Option_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Option_Passive_Motion(int X_Coord, int Y_Coord);

void Option_Reshape(int X_Size, int Y_Size);

int Make_Option_Cube_List(std::array<Color_p, 6> Color_Array);

int Make_Option_Exit_List();

int Make_Option_Solved_List();

int Make_Option_Random_List();

int Make_Option_Line_List();

int Make_Option_Load_List();

int Make_Option_Save_List();

void Make_All_Option_Lists();

#endif // CUBISH_OPTIONWINDOW
