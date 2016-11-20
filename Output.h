
#ifndef CUBISH_OUTPUT
#define CUBISH_OUTPUT

#include <array>

#include "Color.h"
#include "Cube.h"
#include "Vector.h"

extern Cube Current_Cube;
extern int Window_ID;
extern int Cube_List;

void Initialize_Window(int argc, char **argv, Cube& cube, std::array<Color_p, 6> colors);

void Display();

int Make_Arrow_GL_List();

void Mouse(int Button, int State, int X_Coord, int Y_Coord);

void Passive_Motion(int X_Coord, int Y_Coord);

void Motion(int X_Coord, int Y_Coord);

void Animate_Rotation();

void Rotate_Slice();

void Rotate_View();

void Pulsate_Background_Color();

void Resize(int X_Size, int Y_Size);

void Set_Spin(int Value);

void Keyboard(unsigned char Key, int X_Coord, int Y_Coord);

void Close_Window();

Vector Get_Twist_Vector(int Side);

std::array<double, 3> Get_Twist_Side(Vector Origin, Direction direction);

Vector Find_Cube_Point(Vector Begin, Vector End);

Vector Find_Direction(Vector Initial, Vector Begin, Vector End);

Direction Get_Direction(Vector Begin, Vector End);

void Cube_Remake_View();

#endif // CUBISH_OUTPUT
