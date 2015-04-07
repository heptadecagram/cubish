//
// This file defines a Color class which is made of a color-triplet (RGB) of floating-point
// values, to be used with OpenGL.  By making the Color a class, one can create pointers
// to objects of this class, allowing them to be used more easily in object-oriented code,
// as well as be used for other, derived classes.  The constructor may take either no
// arguments, or three floating-point values.  The Change_To() function calls the OpenGL
// function glColor3f() with the current values of that color, which causes the current
// drawing color to switch to that of the Color calling the function.

#ifndef LIAM_COLOR
#define LIAM_COLOR

#include <GLUT/glut.h>

class Color {
public:
	// Constructors
	Color(void) ;
	Color(float Red, float Green, float Blue) ;

	// Destructor
	~Color(void) ;

	// Facilitators
	void Change_To(void) ;

	// Inspectors
	float Get_Red(void) const ;
	float Get_Green(void) const ;
	float Get_Blue(void) const ;

	// Mutators
	void Set(float Red, float Green, float Blue) ;

protected:
	float M_Red ;
	float M_Green ;
	float M_Blue ;
} ;

#endif // LIAM_COLOR
