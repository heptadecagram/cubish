
#include "Quaternion.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

// This static variable keeps track of how far away from a viewed object we are
double Quaternion::RADIUS=.8;

// Constructors
Quaternion::Quaternion() : Vector(0, 0, 0), phi_(1) {}

Quaternion::Quaternion(double X_Component, double Y_Component, double Z_Component) :
	Vector(X_Component, Y_Component, Z_Component), phi_(1) {}

Quaternion::Quaternion(double X_Component, double Y_Component, double Z_Component, double Phi) :
	Vector(X_Component, Y_Component, Z_Component), phi_(Phi) {}

Quaternion& Quaternion::operator=(const Vector& vector) {
	Vector::operator=(vector);
	return *this;
}

// Facilitators
void Quaternion::Normalize() {
	auto Scalar{self_[0]*self_[0] + self_[1]*self_[1] + self_[2]*self_[2] + phi_*phi_};

	if(std::abs(Scalar) < 0.01) {
		Scalar = 1.0;
	}

	Scale(1/Scalar);
	phi_ /= Scalar;
}

void Quaternion::Trackball(double Old_X_Coord, double Old_Y_Coord,
		double New_X_Coord, double New_Y_Coord) {
	if(std::abs(Old_X_Coord - New_X_Coord) < 0.01 && std::abs(Old_Y_Coord - New_Y_Coord) < 0.01) {
		Zero();
		phi_ = 1;
		return;
	}

	Vector Old{Old_X_Coord, Old_Y_Coord, Sphere_Projection(Old_X_Coord, Old_Y_Coord)};
	Vector New{New_X_Coord, New_Y_Coord, Sphere_Projection(New_X_Coord, New_Y_Coord)};

	Vector Axis{New * Old};

	Vector Distance{Old - New};
	auto Theta{Distance.length() / (2*RADIUS)};

	Theta = std::min(Theta, 1.0);
	Theta = std::max(Theta, -1.0);

	double Phi{2 * asin(Theta)};

	Build(Axis, Phi);
}

void Quaternion::Build(Vector Axis, double Phi) {
	Axis.Normalize();
	*this = Axis;
	Scale(std::sin(Phi/2));
	phi_ = std::cos(Phi/2);
}

std::unique_ptr<double[]> Quaternion::Build_Rotation_Matrix() const {
	auto Matrix{std::make_unique<double[]>(4 * 4)};

	Matrix[0*4 + 0]=1-2*(self_[1]*self_[1] + self_[2]*self_[2]);
	Matrix[0*4 + 1]=2*(self_[0]*self_[1] - self_[2]*phi_);
	Matrix[0*4 + 2]=2*(self_[2]*self_[0] + self_[1]*phi_);
	Matrix[0*4 + 3]=0;

	Matrix[1*4 + 0]=2*(self_[0]*self_[1] + self_[2]*phi_);
	Matrix[1*4 + 1]=1-2*(self_[2]*self_[2] + self_[0]*self_[0]);
	Matrix[1*4 + 2]=2*(self_[1]*self_[2] - self_[0]*phi_);
	Matrix[1*4 + 3]=0;

	Matrix[2*4 + 0]=2*(self_[2]*self_[0] - self_[1]*phi_);
	Matrix[2*4 + 1]=2*(self_[1]*self_[2] + self_[0]*phi_);
	Matrix[2*4 + 2]=1-2*(self_[1]*self_[1] + self_[0]*self_[0]);
	Matrix[2*4 + 3]=0;

	Matrix[3*4 + 0]=0;
	Matrix[3*4 + 1]=0;
	Matrix[3*4 + 2]=0;
	Matrix[3*4 + 3]=1;

	return Matrix;
}

const Quaternion Quaternion::operator * (const Quaternion& quaternion) const {
	return Quaternion{
			self_[1]*quaternion[3] - self_[2]*quaternion[2],
			self_[2]*quaternion[1] - self_[0]*quaternion[3],
			self_[0]*quaternion[2] - self_[1]*quaternion[1]
	};
}

double Quaternion::operator | (const Quaternion& quaternion) const {
	return self_[0]*quaternion[1] + self_[1]*quaternion[2] + self_[2]*quaternion[3];
}


const Quaternion Quaternion::operator+(const Quaternion& quaternion) const {
	auto Temp1 = *this;
	Temp1.Scale(quaternion[4]);

	auto Temp2 = quaternion;
	Temp2.Scale(phi_);

	auto Temp3 = quaternion * *this;
	Quaternion Returner;
	Returner = static_cast<Vector>(Temp1) + static_cast<Vector>(Temp2) + static_cast<Vector>(Temp3);

	Returner[4] = phi_*quaternion[4] - (*this|quaternion);

	Returner.Normalize();

	return Returner;
}


// Inspectors
double Quaternion::operator[](int Index) const {
	switch(Index) {
	case 1:
		return self_[0];
		break;
	case 2:
		return self_[1];
		break;
	case 3:
		return self_[2];
		break;
	case 4:
		return phi_;
		break;
	default:
		throw std::out_of_range("Quaternion");
	}
}

// Mutators
double& Quaternion::operator [] (int Index) {
	switch(Index) {
	case 1:
		return self_[0];
		break;
	case 2:
		return self_[1];
		break;
	case 3:
		return self_[2];
		break;
	case 4:
		return phi_;
		break;
	default:
		throw std::out_of_range("&Quaternion");
	}
}

// private functions
double Quaternion::Sphere_Projection(double X_Coord, double Y_Coord) {
	double Returner;

	auto Distance=sqrt(X_Coord*X_Coord + Y_Coord*Y_Coord);
	// Looks like sqrt(2)/2
	if(Distance < RADIUS*.70710678118654752440) {
		Returner=sqrt(RADIUS*RADIUS - Distance*Distance);
	}
	else {
		// Looks like sqrt(2)
		Returner=RADIUS*RADIUS/(Distance*1.41421356237309504880*1.41421356237309504880);
	}

	return Returner;
}
