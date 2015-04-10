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
	Vector(void);
	Vector(double X_Component, double Y_Component, double Z_Component);

	// Facilitators
	Vector operator + (const Vector &vector) const;
	Vector operator - (const Vector &vector) const;
	Vector operator * (const Vector &vector) const;
	double operator | (const Vector &vector) const;


	// Inspectors
	double Get_Length(void) const;
	int Get_Size(void) const;
	double operator [] (int Index) const;


	// Mutators
	void Zero(void);
	void Normalize(void);
	void Scale(double Scalar);
	double &operator [] (int Index);
	void operator - (void);

protected:
	std::array<double, 3> _self;
};

#endif // LIAM_VECTOR

