#ifndef CUBISH_COLORPICK
#define CUBISH_COLORPICK

// Description: Creates a simple window that allows the user to pick a new
// color, which is stored at the address of the color passed to Color_Window().

#include "Tile.h"

// Tracks whether the color picker is open or not
extern bool Is_Color_Window_Open;
extern int Color_Window_ID;

void Color_Window(Color Old_Color);

void Color_Display();

void Color_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Color_Reshape(int X_Size, int Y_Size);

#endif
