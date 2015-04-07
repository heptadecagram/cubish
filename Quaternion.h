//
// Description: Quaternions are vectors with a rotation angle.  This file declars
// a class Quaternion of such mathematical constructs.  These objects are very
// useful for graphical applications.  For more information on using quaternions
// in programming, consult Shoemake's work.  The algorithms in this class were
// derived from and expand upon the examples in the appendix of Edward Angel's
// _Interactive_Computer_Graphics_.
//
// The main purpose of this class is to create viewing volume matrices.

#ifndef LIAM_QUATERNION
#define LIAM_QUATERNION

#include <math.h>

#include "Vector.h"

class Quaternion: public Vector {
public:
	// Constructors
	Quaternion(void) ;
	Quaternion(float X_Component, float Y_Component, float Z_Component, float Phi) ;
	Quaternion &operator = (const Vector &vector) ;
	Quaternion &operator = (const Quaternion &quaternion) ;

	// Destructor
	~Quaternion(void) ;

	// Facilitators
	void Normalize(void) ;
	void Trackball(float Old_X_Coord, float Old_Y_Coord, float New_X_Coord, float New_Y_Coord) ;
	void Build(Vector Axis, float Phi) ;
	void Build_Rotation_Matrix(float Matrix[4][4]) ;
	Quaternion operator * (const Quaternion &quaternion) const ;
	float operator | (const Quaternion &quaternion) ;
	Quaternion operator + (const Quaternion &quaternion) ;

	// Inspectors
	float operator [] (int Index) const ;

	// Mutators
	float &operator [] (int Index) ;
	void Set_Radius(float Radius) ;

	// Member Variables
	static float M_Radius ;

protected:
	// Member Variables
	float M_Phi ;
	//float M_Radius ;

private:
	float Sphere_Projection(float X_Coord, float Y_Coord) ;
} ;

#endif // LIAM_QUATERNION
