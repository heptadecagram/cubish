//
// This file defines a Face class, which is a dynamic array of elements of the Tile class.
// A Face can be rotated clockwise and counterclockwise using the Rotate_CW() and
// Rotate_CCW() functions, respectively.  If a Face is not square (equal height and width),
// then the rotation functions will cause the face to be rotated 180 degrees instead of 90.
// A Face can be forced to rotate only 90 degrees by using the Spin_CW() and Spin_CCW()
// functions.  Note that these functions physically change the height and width of the Face.
// The solved() function will return true if each Tile on the Face has exactly the same
// amounts of red, green, and blue.  Flood() will set each Tile on the Face to the Color
// specified.  Note that a Face is initialized with a single Color that specifies the
// Color for each Tile on the Face.  The default size is 3x3.

#ifndef CUBISH_FACE
#define CUBISH_FACE

#include <memory>
#include <vector>

#include "Color.h"
#include "Tile.h"

using Tile_p = std::shared_ptr<Tile>;

class Face {
public:
	// Constructors
	Face() : Face(3, 3, NULL) {};
	Face(int Width, int Height, Color_p color);
	Face(const Face &face);

	// Inspectors
	int height() const;
	int length() const;

	bool solved() const;
	Tile_p& operator()(int col, int row);

	// Mutators
	void Flood(Color_p color);
	void Rotate_CW();
	void Rotate_CCW();
	void Spin_CW();
	void Spin_CCW();

protected:
	int _height;
	int _width;
	std::vector<Tile_p> _tiles;
};

#endif // CUBISH_FACE
