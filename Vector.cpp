
#include "Vector.h"

#include <cmath>

// Constructors
Vector::Vector() : self_({{ 0.0, 0.0, 0.0 }}) {
}

Vector::Vector(double X_Component, double Y_Component, double Z_Component) :
	self_({{ X_Component, Y_Component, Z_Component }}) {
}

// Facilitators
// Adding two Vector objects
Vector Vector::operator + (const Vector &vector) const {
	return Vector(
			self_[0] + vector[1],
			self_[1] + vector[2],
			self_[2] + vector[3]
			);
}

// Subtracting two Vector objects
Vector Vector::operator - (const Vector &vector) const {
	return Vector(
			self_[0] - vector[1],
			self_[1] - vector[2],
			self_[2] - vector[3]
			);
}

// Cross product
Vector Vector::operator * (const Vector &vector) const {
	return Vector(
			self_[1]*vector[3] - self_[2]*vector[2],
			self_[2]*vector[1] - self_[0]*vector[3],
			self_[0]*vector[2] - self_[1]*vector[1]
			);
}

// Dot product
double Vector::operator | (const Vector &vector) const {
	return self_[0]*vector[1] + self_[1]*vector[2] + self_[2]*vector[3];
}


// Inspectors
double Vector::length() const {
	return sqrt(self_[0]*self_[0] + self_[1]*self_[1] + self_[2]*self_[2]);
}

// Without this function, Vector objects are almost useless.  This allows the user
// to access the object's indicies.
double Vector::operator [] (int Index) const {
	return self_[Index - 1];
}


// Mutators
void Vector::Zero() {
	self_.fill(0);
}

// Have some way to normalize a vector.  It's nice to have.
void Vector::Normalize() {
	Scale(1/length() );
}

void Vector::Scale(double Scalar) {
	self_[0] *= Scalar;
	self_[1] *= Scalar;
	self_[2] *= Scalar;
}

double &Vector::operator [] (int Index) {
	return self_[Index - 1];
}
