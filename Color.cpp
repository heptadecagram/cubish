
#include "Color.h"

#include "ui.h"

#include <cmath>

// Inspectors
double Color::red() const {
	return red_;
}

double Color::green() const {
	return green_;
}

double Color::blue() const {
	return blue_;
}

bool Color::operator==(const Color& other) const {
	return std::abs(red_ - other.red_) < 0.01 &&
	std::abs(green_ - other.green_) < 0.01 &&
	std::abs(blue_ - other.blue_) < 0.01;
}


// Mutators
void Color::Set(double Red, double Green, double Blue) {
	red_ = Red;
	green_ = Green;
	blue_ = Blue;
}
