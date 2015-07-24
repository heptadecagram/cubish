
#include "Color.h"

#include "ui.h"

// Constructors
Color::Color() {
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
void Color::Change_To() {
	glColor3d(M_Red, M_Green, M_Blue);
}


// Inspectors
double Color::Get_Red() const {
	return M_Red;
}

double Color::Get_Green() const {
	return M_Green;
}

double Color::Get_Blue() const {
	return M_Blue;
}

bool Color::operator==(const Color& other) const {
	return M_Red == other.M_Red &&
	M_Green == other.M_Green &&
	M_Blue == other.M_Blue;
}


// Mutators
void Color::Set(double Red, double Green, double Blue) {
	M_Red=Red;
	M_Green=Green;
	M_Blue=Blue;
}
