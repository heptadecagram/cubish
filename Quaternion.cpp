
#include "Quaternion.h"

// This static variable keeps track of how far away from a viewed object we are
float Quaternion::M_Radius=.8 ;

// Constructors
Quaternion::Quaternion(void) {
	M_X_Component=0 ;
	M_Y_Component=0 ;
	M_Z_Component=0 ;
	M_Phi=1 ;
}

Quaternion::Quaternion(float X_Component, float Y_Component, float Z_Component, float Phi) {
	M_X_Component=X_Component ;
	M_Y_Component=Y_Component ;
	M_Z_Component=Z_Component ;
	M_Phi=Phi ;
}

Quaternion &Quaternion::operator = (const Vector &vector) {
	M_X_Component=vector[1] ;
	M_Y_Component=vector[2] ;
	M_Z_Component=vector[3] ;
	return *this ;
}

Quaternion &Quaternion::operator = (const Quaternion &quaternion) {
	M_X_Component=quaternion[1] ;
	M_Y_Component=quaternion[2] ;
	M_Z_Component=quaternion[3] ;
	M_Phi=quaternion[4] ;
	return *this ;
}

// Destructor
Quaternion::~Quaternion(void) {
}

// Facilitators
void Quaternion::Normalize(void) {
	float Scalar=M_X_Component*M_X_Component + M_Y_Component*M_Y_Component +
		M_Z_Component*M_Z_Component + M_Phi*M_Phi ;
	if(Scalar==0)
		Scalar=1.0 ;
	Scale(1/Scalar) ;
	M_Phi/=Scalar ;
}

void Quaternion::Trackball(float Old_X_Coord, float Old_Y_Coord,
						   float New_X_Coord, float New_Y_Coord) {
	if(Old_X_Coord==New_X_Coord && Old_Y_Coord==New_Y_Coord) {
        Zero() ;
        M_Phi=1 ;
        return ;
	}

	Vector Old(Old_X_Coord, Old_Y_Coord, Sphere_Projection(Old_X_Coord, Old_Y_Coord) ) ;
	Vector New(New_X_Coord, New_Y_Coord, Sphere_Projection(New_X_Coord, New_Y_Coord) ) ;

	Vector Axis=New*Old ;

	Vector Distance=Old-New ;
	float Theta=Distance.Get_Length()/(2*M_Radius) ;

	if(Theta>1)
		Theta=1 ;
	if(Theta<-1)
		Theta=-1 ;
	float Phi=2*(float)asin(Theta) ;

	Build(Axis, Phi) ;
}

void Quaternion::Build(Vector Axis, float Phi) {
	Axis.Normalize() ;
	*this=Axis ;
	Scale((float)sin(Phi/2) ) ;
	M_Phi=(float)cos(Phi/2) ;
}

void Quaternion::Build_Rotation_Matrix(float Matrix[4][4]) {
	Matrix[0][0]=1-2*(M_Y_Component*M_Y_Component + M_Z_Component*M_Z_Component) ;
	Matrix[0][1]=2*(M_X_Component*M_Y_Component - M_Z_Component*M_Phi) ;
	Matrix[0][2]=2*(M_Z_Component*M_X_Component + M_Y_Component*M_Phi) ;
	Matrix[0][3]=0 ;

	Matrix[1][0]=2*(M_X_Component*M_Y_Component + M_Z_Component*M_Phi) ;
	Matrix[1][1]=1-2*(M_Z_Component*M_Z_Component + M_X_Component*M_X_Component) ;
	Matrix[1][2]=2*(M_Y_Component*M_Z_Component - M_X_Component*M_Phi) ;
	Matrix[1][3]=0 ;

	Matrix[2][0]=2*(M_Z_Component*M_X_Component - M_Y_Component*M_Phi) ;
	Matrix[2][1]=2*(M_Y_Component*M_Z_Component + M_X_Component*M_Phi) ;
	Matrix[2][2]=1-2*(M_Y_Component*M_Y_Component + M_X_Component*M_X_Component) ;
	Matrix[2][3]=0 ;

	Matrix[3][0]=0 ;
	Matrix[3][1]=0 ;
	Matrix[3][2]=0 ;
	Matrix[3][3]=1 ;
}

Quaternion Quaternion::operator * (const Quaternion &quaternion) const {
	Quaternion Temp ;
	Temp[1]=M_Y_Component*quaternion[3] - M_Z_Component*quaternion[2] ;
	Temp[2]=M_Z_Component*quaternion[1] - M_X_Component*quaternion[3] ;
	Temp[3]=M_X_Component*quaternion[2] - M_Y_Component*quaternion[1] ;
	return Temp ;
}

float Quaternion::operator | (const Quaternion &quaternion) {
	return M_X_Component*quaternion[1] + M_Y_Component*quaternion[2] +
		M_Z_Component*quaternion[3] ;
}


Quaternion Quaternion::operator + (const Quaternion &quaternion) {
	Quaternion Temp1, Temp2, Temp3, Returner ;

	Temp1=*this ;
	Temp1.Scale(quaternion[4]) ;

	Temp2=quaternion ;
	Temp2.Scale(M_Phi) ;

	Temp3=quaternion * *this ;
	Returner=(Vector)Temp1+(Vector)Temp2+(Vector)Temp3 ;

	Returner[4]=M_Phi*quaternion[4] - (*this|quaternion) ;

	Returner.Normalize() ;

	return Returner ;
}


// Inspectors
float Quaternion::operator [] (int Index) const {
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
	case 4:
		return M_Phi ;
		break ;
	default:
		Die("Improper Quaternion Index: %d", Index) ;
		break ;
	}
}

// Mutators
float &Quaternion::operator [] (int Index) {
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
	case 4:
		return M_Phi ;
		break ;
	default:
		Die("Improper &Quaternion Index: %d", Index) ;
		break ;
	}
}

void Quaternion::Set_Radius(float Radius) {
	M_Radius=Radius ;
}

// private functions
float Quaternion::Sphere_Projection(float X_Coord, float Y_Coord) {
    double Returner ;

	double Distance=sqrt(X_Coord*X_Coord + Y_Coord*Y_Coord) ;
    if(Distance<M_Radius*.70710678118654752440)
		Returner=sqrt(M_Radius*M_Radius - Distance*Distance) ;
	else
		Returner=M_Radius*M_Radius/(Distance*1.41421356237309504880*1.41421356237309504880) ;

	return (float)Returner ;
}
