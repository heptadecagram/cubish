//
// Description: This file implements a standard Vector() class.  Vector objects
// can be added or subtracted ( + or - ), normalized, scaled, negated, and
// produce cross products ( * ) or dot products ( | ).

#ifndef CUBISH_VECTOR
#define CUBISH_VECTOR

#include <array>

class Vector {
public:
	// Constructors
	Vector();
	Vector(double X_Component, double Y_Component, double Z_Component);

	// Facilitators
	Vector operator + (const Vector &vector) const;
	Vector operator - (const Vector &vector) const;
	Vector operator * (const Vector &vector) const;
	double operator | (const Vector &vector) const;


	// Inspectors
	double length() const;
	double operator [] (int Index) const;


	// Mutators
	void Zero();
	void Normalize();
	void Scale(double Scalar);
	double &operator [] (int Index);
	void operator - ();

protected:
	std::array<double, 3> self_;
};

#endif // CUBISH_VECTOR

