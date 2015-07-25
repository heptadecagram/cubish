//
// Description: Creates a simple window that allows the user to pick a new
// color, which is stored at the address of the color passed to Color_Window().

#ifndef CUBISH_COLORPICK
#define CUBISH_COLORPICK

#include "Tile.h"

void Color_Window(Color_p Old_Color);

void Color_Display();

void Color_Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Color_Reshape(int X_Size, int Y_Size);

#endif // CUBISH_COLORPICK
