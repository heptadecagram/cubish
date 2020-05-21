
#include "Color.h"

#include "ui.h"

#include <cmath>

// Constructors
Color::Color() {
	red_=0;
	green_=0;
	blue_=0;
}

Color::Color(double Red, double Green, double Blue) {
	red_=Red;
	green_=Green;
	blue_=Blue;
}


// Facilitators
void Color::Change_To() {
	glColor3d(red_, green_, blue_);
}


// Inspectors
double Color::Get_Red() const {
	return red_;
}

double Color::Get_Green() const {
	return green_;
}

double Color::Get_Blue() const {
	return blue_;
}

bool Color::operator==(const Color& other) const {
	return fabs(red_ - other.red_) < 0.01 &&
	fabs(green_ - other.green_) < 0.01 &&
	fabs(blue_ - other.blue_) < 0.01;
}


// Mutators
void Color::Set(double Red, double Green, double Blue) {
	red_=Red;
	green_=Green;
	blue_=Blue;
}
