
#include "Quaternion.h"

#include <cmath>
#include <stdexcept>

// This static variable keeps track of how far away from a viewed object we are
float Quaternion::M_Radius=.8;

// Constructors
Quaternion::Quaternion(void) : Vector(0, 0, 0), _phi(1) {
}

Quaternion::Quaternion(float X_Component, float Y_Component, float Z_Component, float Phi) :
	Vector(X_Component, Y_Component, Z_Component), _phi(Phi) {
}

Quaternion &Quaternion::operator = (const Vector &vector) {
	Vector::operator=(vector);
	return *this;
}

// Facilitators
void Quaternion::Normalize(void) {
	float Scalar=_self[0]*_self[0] + _self[1]*_self[1] + _self[2]*_self[2] + _phi*_phi;
	if(Scalar==0)
		Scalar=1.0;
	Scale(1/Scalar);
	_phi/=Scalar;
}

void Quaternion::Trackball(float Old_X_Coord, float Old_Y_Coord,
						   float New_X_Coord, float New_Y_Coord) {
	if(Old_X_Coord==New_X_Coord && Old_Y_Coord==New_Y_Coord) {
        Zero();
        _phi=1;
        return;
	}

	Vector Old(Old_X_Coord, Old_Y_Coord, Sphere_Projection(Old_X_Coord, Old_Y_Coord) );
	Vector New(New_X_Coord, New_Y_Coord, Sphere_Projection(New_X_Coord, New_Y_Coord) );

	Vector Axis=New*Old;

	Vector Distance=Old-New;
	float Theta=Distance.Get_Length()/(2*M_Radius);

	if(Theta>1)
		Theta=1;
	if(Theta<-1)
		Theta=-1;
	float Phi=2*(float)asin(Theta);

	Build(Axis, Phi);
}

void Quaternion::Build(Vector Axis, float Phi) {
	Axis.Normalize();
	*this=Axis;
	Scale((float)sin(Phi/2) );
	_phi=(float)cos(Phi/2);
}

void Quaternion::Build_Rotation_Matrix(float Matrix[4][4]) {
	Matrix[0][0]=1-2*(_self[1]*_self[1] + _self[2]*_self[2]);
	Matrix[0][1]=2*(_self[0]*_self[1] - _self[2]*_phi);
	Matrix[0][2]=2*(_self[2]*_self[0] + _self[1]*_phi);
	Matrix[0][3]=0;

	Matrix[1][0]=2*(_self[0]*_self[1] + _self[2]*_phi);
	Matrix[1][1]=1-2*(_self[2]*_self[2] + _self[0]*_self[0]);
	Matrix[1][2]=2*(_self[1]*_self[2] - _self[0]*_phi);
	Matrix[1][3]=0;

	Matrix[2][0]=2*(_self[2]*_self[0] - _self[1]*_phi);
	Matrix[2][1]=2*(_self[1]*_self[2] + _self[0]*_phi);
	Matrix[2][2]=1-2*(_self[1]*_self[1] + _self[0]*_self[0]);
	Matrix[2][3]=0;

	Matrix[3][0]=0;
	Matrix[3][1]=0;
	Matrix[3][2]=0;
	Matrix[3][3]=1;
}

Quaternion Quaternion::operator * (const Quaternion &quaternion) const {
	Quaternion Temp;
	Temp[1]=_self[1]*quaternion[3] - _self[2]*quaternion[2];
	Temp[2]=_self[2]*quaternion[1] - _self[0]*quaternion[3];
	Temp[3]=_self[0]*quaternion[2] - _self[1]*quaternion[1];
	return Temp;
}

float Quaternion::operator | (const Quaternion &quaternion) {
	return _self[0]*quaternion[1] + _self[1]*quaternion[2] +
		_self[2]*quaternion[3];
}


Quaternion Quaternion::operator + (const Quaternion &quaternion) {
	Quaternion Temp1, Temp2, Temp3, Returner;

	Temp1=*this;
	Temp1.Scale(quaternion[4]);

	Temp2=quaternion;
	Temp2.Scale(_phi);

	Temp3=quaternion * *this;
	Returner = (Vector)Temp1 + (Vector)Temp2 + (Vector)Temp3;

	Returner[4]=_phi*quaternion[4] - (*this|quaternion);

	Returner.Normalize();

	return Returner;
}


// Inspectors
float Quaternion::operator [] (int Index) const {
	switch(Index) {
	case 1:
		return _self[0];
		break;
	case 2:
		return _self[1];
		break;
	case 3:
		return _self[2];
		break;
	case 4:
		return _phi;
		break;
	default:
		throw std::out_of_range("Quaternion");
	}
}

// Mutators
float &Quaternion::operator [] (int Index) {
	switch(Index) {
	case 1:
		return _self[0];
		break;
	case 2:
		return _self[1];
		break;
	case 3:
		return _self[2];
		break;
	case 4:
		return _phi;
		break;
	default:
		throw std::out_of_range("&Quaternion");
	}
}

void Quaternion::Set_Radius(float Radius) {
	M_Radius=Radius;
}

// private functions
float Quaternion::Sphere_Projection(float X_Coord, float Y_Coord) {
    double Returner;

	double Distance=sqrt(X_Coord*X_Coord + Y_Coord*Y_Coord);
    if(Distance<M_Radius*.70710678118654752440)
		Returner=sqrt(M_Radius*M_Radius - Distance*Distance);
	else
		Returner=M_Radius*M_Radius/(Distance*1.41421356237309504880*1.41421356237309504880);

	return (float)Returner;
}
