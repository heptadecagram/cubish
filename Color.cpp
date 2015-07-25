
#include "Color.h"

#include "ui.h"

// Constructors
Color::Color() {
	_red=0;
	_green=0;
	_blue=0;
}

Color::Color(double Red, double Green, double Blue) {
	_red=Red;
	_green=Green;
	_blue=Blue;
}


// Facilitators
void Color::Change_To() {
	glColor3d(_red, _green, _blue);
}


// Inspectors
double Color::Get_Red() const {
	return _red;
}

double Color::Get_Green() const {
	return _green;
}

double Color::Get_Blue() const {
	return _blue;
}

bool Color::operator==(const Color& other) const {
	return _red == other._red &&
	_green == other._green &&
	_blue == other._blue;
}


// Mutators
void Color::Set(double Red, double Green, double Blue) {
	_red=Red;
	_green=Green;
	_blue=Blue;
}
