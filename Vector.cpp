
#include "Vector.h"

#include <cmath>

// Constructors
Vector::Vector(void) : _self({{ 0.0, 0.0, 0.0 }}) {
}

Vector::Vector(double X_Component, double Y_Component, double Z_Component) :
	_self({{ X_Component, Y_Component, Z_Component }}) {
}

// Facilitators
// Adding two Vector objects
Vector Vector::operator + (const Vector &vector) const {
	return Vector(
			_self[0] + vector[1],
			_self[1] + vector[2],
			_self[2] + vector[3]
			);
}

// Subtracting two Vector objects
Vector Vector::operator - (const Vector &vector) const {
	return Vector(
			_self[0] - vector[1],
			_self[1] - vector[2],
			_self[2] - vector[3]
			);
}

// Cross product
Vector Vector::operator * (const Vector &vector) const {
	return Vector(
			_self[1]*vector[3] - _self[2]*vector[2],
			_self[2]*vector[1] - _self[0]*vector[3],
			_self[0]*vector[2] - _self[1]*vector[1]
			);
}

// Dot product
double Vector::operator | (const Vector &vector) const {
	return _self[0]*vector[1] + _self[1]*vector[2] + _self[2]*vector[3];
}


// Inspectors
double Vector::Get_Length(void) const {
	return sqrt(_self[0]*_self[0] + _self[1]*_self[1] + _self[2]*_self[2]);
}

// Without this function, Vector objects are almost useless.  This allows the user
// to access the object's indicies.
double Vector::operator [] (int Index) const {
	return _self[Index - 1];
}


// Mutators
void Vector::Zero(void) {
	_self.fill(0);
}

// Have some way to normalize a vector.  It's nice to have.
void Vector::Normalize(void) {
	Scale(1/Get_Length() );
}

void Vector::Scale(double Scalar) {
	_self[0] *= Scalar;
	_self[1] *= Scalar;
	_self[2] *= Scalar;
}

double &Vector::operator [] (int Index) {
	return _self[Index - 1];
}
