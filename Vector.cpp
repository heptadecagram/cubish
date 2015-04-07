
#include <stdexcept>

#include "Vector.h"

// Constructors
Vector::Vector(void) {
	_self = {{ 0.0, 0.0, 0.0 }};
}

Vector::Vector(float X_Component, float Y_Component, float Z_Component) {
	_self = {{ X_Component, Y_Component, Z_Component }};
}

// Facilitators
// Adding two Vector objects
Vector Vector::operator + (Vector &vector) {
	Vector Temp ;
	Temp[1]=_self[0]+vector[1] ;
	Temp[2]=_self[1]+vector[2] ;
	Temp[3]=_self[2]+vector[3] ;
	return Temp ;
}

// Subtracting two Vector objects
Vector Vector::operator - (Vector &vector) {
	Vector Temp ;
	Temp[1]=_self[0]-vector[1] ;
	Temp[2]=_self[1]-vector[2] ;
	Temp[3]=_self[2]-vector[3] ;
	return Temp ;
}

// Cross product
Vector Vector::operator * (Vector &vector) {
	Vector Temp ;
	Temp[1]=_self[1]*vector[3] - _self[2]*vector[2] ;
	Temp[2]=_self[2]*vector[1] - _self[0]*vector[3] ;
	Temp[3]=_self[0]*vector[2] - _self[1]*vector[1] ;
	return Temp ;
}

// Dot product
float Vector::operator | (Vector &vector) {
	return _self[0]*vector[1] + _self[1]*vector[2] + _self[2]*vector[3] ;
}


// Inspectors
float Vector::Get_Length(void) const {
	return (float)sqrt(_self[0]*_self[0] + _self[1]*_self[1] + _self[2]*_self[2]) ;
}

// Without this function, Vector objects are almost useless.  This allows the user
// to access the object's indicies.
float Vector::operator [] (int Index) const {
	return _self[Index - 1];
}


// Mutators
void Vector::Zero(void) {
	_self.fill(0);
}

// Have some way to normalize a vector.  It's nice to have.
void Vector::Normalize(void) {
	Scale(1/Get_Length() ) ;
}

void Vector::Scale(float Scalar) {
	_self[0]*=Scalar ;
	_self[1]*=Scalar ;
	_self[2]*=Scalar ;
}

float &Vector::operator [] (int Index) {
	return _self[Index - 1];
}
