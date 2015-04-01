// File: Output.h
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.04.19

#ifndef LIAM_OUTPUT
#define LIAM_OUTPUT

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#include "Color.h"
#include "Cube.h"
#include "Vector.h"
#include "Quaternion.h"

#include "OptionWindow.h"

void Initialize_Window(int argc, char **argv, Cube *cube, Color *Color_Array[6]) ;

void Display(void) ;

int Make_Arrow_GL_List(void) ;

void Mouse(int Button, int State, int X_Coord, int Y_Coord) ;

void Passive_Motion(int X_Coord, int Y_Coord) ;

void Motion(int X_Coord, int Y_Coord) ;

void Animate_Rotation(void) ;

void Rotate_Slice(void) ;

void Rotate_View(void) ;

void Pulsate_Background_Color(void) ;

void Resize(int X_Size, int Y_Size) ;

void Set_Spin(int Value) ;

void Keyboard(unsigned char Key, int X_Coord, int Y_Coord) ;

void Close_Window(void) ;

Vector Get_Twist_Vector(int Side) ;

Vector Get_Twist_Side(Vector Origin, Direction direction) ;

Vector Find_Cube_Point(Vector Begin, Vector End) ;

Vector Find_Direction(Vector Initial, Vector Begin, Vector End) ;

Direction Get_Direction(Vector Begin, Vector End) ;

void Cube_Remake_View(void) ;

#endif // LIAM_OUTPUT
