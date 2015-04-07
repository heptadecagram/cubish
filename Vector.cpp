
#include <stdexcept>

#include "Vector.h"

// Constructors
Vector::Vector(void) {
	M_X_Component=0 ;
	M_Y_Component=0 ;
	M_Z_Component=0 ;
}

Vector::Vector(float X_Component, float Y_Component, float Z_Component) {
	M_X_Component=X_Component ;
	M_Y_Component=Y_Component ;
	M_Z_Component=Z_Component ;
}

Vector::Vector(Vector &vector) {
	M_X_Component=vector[1] ;
	M_Y_Component=vector[2] ;
	M_Z_Component=vector[3] ;
}

Vector &Vector::operator = (const Vector &vector) {
	M_X_Component=vector[1] ;
	M_Y_Component=vector[2] ;
	M_Z_Component=vector[3] ;

	return *this ;
}

// Destructor
Vector::~Vector(void) {
}

// Facilitators
// Adding two Vector objects
Vector Vector::operator + (Vector &vector) {
	Vector Temp ;
	Temp[1]=M_X_Component+vector[1] ;
	Temp[2]=M_Y_Component+vector[2] ;
	Temp[3]=M_Z_Component+vector[3] ;
	return Temp ;
}

// Subtracting two Vector objects
Vector Vector::operator - (Vector &vector) {
	Vector Temp ;
	Temp[1]=M_X_Component-vector[1] ;
	Temp[2]=M_Y_Component-vector[2] ;
	Temp[3]=M_Z_Component-vector[3] ;
	return Temp ;
}

// Cross product
Vector Vector::operator * (Vector &vector) {
	Vector Temp ;
	Temp[1]=M_Y_Component*vector[3] - M_Z_Component*vector[2] ;
	Temp[2]=M_Z_Component*vector[1] - M_X_Component*vector[3] ;
	Temp[3]=M_X_Component*vector[2] - M_Y_Component*vector[1] ;
	return Temp ;
}

// Dot product
float Vector::operator | (Vector &vector) {
	return M_X_Component*vector[1] + M_Y_Component*vector[2] + M_Z_Component*vector[3] ;
}


// Inspectors
float Vector::Get_Length(void) const {
	return (float)sqrt(M_X_Component*M_X_Component + M_Y_Component*M_Y_Component +
			M_Z_Component*M_Z_Component) ;
}

// Without this function, Vector objects are almost useless.  This allows the user
// to access the object's indicies.
float Vector::operator [] (int Index) const {
	switch(Index) {
	case 1:
		return M_X_Component ;
		break ;
	case 2:
		return M_Y_Component ;
		break ;
	case 3:
		return M_Z_Component ;
		break ;
	default:
		throw std::out_of_range("Vector");
	}
}


// Mutators
void Vector::Zero(void) {
	M_X_Component=0 ;
	M_Y_Component=0 ;
	M_Z_Component=0 ;
}

// Have some way to normalize a vector.  It's nice to have.
void Vector::Normalize(void) {
	Scale(1/Get_Length() ) ;
}

void Vector::Scale(float Scalar) {
	M_X_Component*=Scalar ;
	M_Y_Component*=Scalar ;
	M_Z_Component*=Scalar ;
}

float &Vector::operator [] (int Index) {
	switch(Index) {
	case 1:
		return M_X_Component ;
		break ;
	case 2:
		return M_Y_Component ;
		break ;
	case 3:
		return M_Z_Component ;
		break ;
	default:
		throw std::out_of_range("&Vector");
	}
}
