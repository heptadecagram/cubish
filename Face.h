//
// This file defines a Face class, which is a dynamic array of elements of the Tile class.
// A Face can be rotated clockwise and counterclockwise using the Rotate_CW() and
// Rotate_CCW() functions, respectively.  If a Face is not square (equal height and width),
// then the rotation functions will cause the face to be rotated 180 degrees instead of 90.
// A Face can be forced to rotate only 90 degrees by using the Spin_CW() and Spin_CCW()
// functions.  Note that these functions physically change the height and width of the Face.
// The Is_Solved() function will return true if each Tile on the Face has exactly the same
// amounts of red, green, and blue.  Flood() will set each Tile on the Face to the Color
// specified.  Note that a Face is initialized with a single Color that specifies the
// Color for each Tile on the Face.  The default size is 3x3.

#ifndef LIAM_FACE
#define LIAM_FACE

#include <memory>
#include <vector>

#include "Color.h"
#include "Tile.h"

using Tile_p = std::shared_ptr<Tile>;

class Face {
public:
	// Constructors
	Face(void);
	Face(int Width, int Height, Color *color);
	Face(const Face &face);

	// Inspectors
	int height(void) const;
	int length(void) const;

	bool Is_Solved(void) const;
	Tile_p& operator()(int col, int row);

	// Mutators
	void Flood(Color *color);
	void Rotate_CW(void);
	void Rotate_CCW(void);
	void Spin_CW(void);
	void Spin_CCW(void);

protected:
	int M_Height;
	int M_Width;
	std::vector<Tile_p> _tiles;
};

#endif // LIAM_FACE
