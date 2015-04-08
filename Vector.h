//
// Description: This file implements a standard Vector() class.  Vector objects
// can be added or subtracted ( + or - ), normalized, scaled, negated, and
// produce cross products ( * ) or dot products ( | ).

#ifndef LIAM_VECTOR
#define LIAM_VECTOR

#include <array>

class Vector {
public:
	// Constructors
	Vector(void) ;
	Vector(float X_Component, float Y_Component, float Z_Component) ;

	// Facilitators
	Vector operator + (const Vector &vector) const;
	Vector operator - (const Vector &vector) const;
	Vector operator * (const Vector &vector) const;
	float operator | (const Vector &vector) const;


	// Inspectors
	float Get_Length(void) const ;
	int Get_Size(void) const ;
	float operator [] (int Index) const ;


	// Mutators
	void Zero(void) ;
	void Normalize(void) ;
	void Scale(float Scalar) ;
	float &operator [] (int Index) ;
	void operator - (void) ;

protected:
	std::array<float, 3> _self;
} ;

#endif // LIAM_VECTOR

