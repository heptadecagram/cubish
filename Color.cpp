
#include "Color.h"

#include <GLUT/glut.h>

// Constructors
Color::Color(void) {
	M_Red=0;
	M_Green=0;
	M_Blue=0;
}

Color::Color(double Red, double Green, double Blue) {
	M_Red=Red;
	M_Green=Green;
	M_Blue=Blue;
}


// Facilitators
void Color::Change_To(void) {
	glColor3d(M_Red, M_Green, M_Blue);
}


// Inspectors
double Color::Get_Red(void) const {
	return M_Red;
}

double Color::Get_Green(void) const {
	return M_Green;
}

double Color::Get_Blue(void) const {
	return M_Blue;
}


// Mutators
void Color::Set(double Red, double Green, double Blue) {
	M_Red=Red;
	M_Green=Green;
	M_Blue=Blue;
}
