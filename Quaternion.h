//
// Description: Quaternions are vectors with a rotation angle.  This file declars
// a class Quaternion of such mathematical constructs.  These objects are very
// useful for graphical applications.  For more information on using quaternions
// in programming, consult Shoemake's work.  The algorithms in this class were
// derived from and expand upon the examples in the appendix of Edward Angel's
// _Interactive_Computer_Graphics_.
//
// The main purpose of this class is to create viewing volume matrices.

#ifndef CUBISH_QUATERNION
#define CUBISH_QUATERNION

#include "Vector.h"

class Quaternion: public Vector {
public:
	// Constructors
	Quaternion();
	Quaternion(double X_Component, double Y_Component, double Z_Component, double Phi);
	Quaternion &operator = (const Vector &vector);

	// Facilitators
	void Normalize();
	void Trackball(double Old_X_Coord, double Old_Y_Coord, double New_X_Coord, double New_Y_Coord);
	void Build(Vector Axis, double Phi);
	void Build_Rotation_Matrix(double Matrix[4][4]);
	Quaternion operator * (const Quaternion &quaternion) const;
	double operator | (const Quaternion &quaternion);
	Quaternion operator + (const Quaternion &quaternion);

	// Inspectors
	double operator [] (int Index) const;

	// Mutators
	double &operator [] (int Index);
	void Set_Radius(double Radius);

	// Class Variables
	static double RADIUS;

protected:
	// Member Variables
	double _phi;

private:
	double Sphere_Projection(double X_Coord, double Y_Coord);
};

#endif // CUBISH_QUATERNION
