//
// This file defines a Tile class which is a wrapper for a pointer to a Color.  A
// Tile is simply for convenience of the programmer, and does not confer any speed
// optimizations.  The address of the Color the Tile uses may be obtained with
// Get_Color() and set with Set_Color().

#ifndef LIAM_TILE
#define LIAM_TILE

#include <memory>

#include "Color.h"

using Color_p = std::shared_ptr<Color>;

class Tile {
public:
	// Constructors
	Tile();
	Tile(Color_p color);

	// Facilitators

	// Inspectors
	Color_p Get_Color() const;
	bool operator==(const Tile& other) const;

	// Mutators
	void Set_Color(Color_p color);

protected:
	Color_p _color;
};

#endif // LIAM_TILE
