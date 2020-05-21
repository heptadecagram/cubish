//
// This file defines a Color class which is made of a color-triplet (RGB) of floating-point
// values, to be used with OpenGL.  By making the Color a class, one can create pointers
// to objects of this class, allowing them to be used more easily in object-oriented code,
// as well as be used for other, derived classes.  The constructor may take either no
// arguments, or three floating-point values.  The Change_To() function calls the OpenGL
// function glColor3d() with the current values of that color, which causes the current
// drawing color to switch to that of the Color calling the function.

#ifndef CUBISH_COLOR
#define CUBISH_COLOR

class Color {
public:
	// Constructors
	Color();
	Color(double Red, double Green, double Blue);

	// Facilitators
	void Change_To();

	// Inspectors
	double Get_Red() const;
	double Get_Green() const;
	double Get_Blue() const;
	bool operator==(const Color& other) const;

	// Mutators
	void Set(double Red, double Green, double Blue);

protected:
	double red_;
	double green_;
	double blue_;
};

#endif // CUBISH_COLOR
