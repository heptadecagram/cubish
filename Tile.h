#ifndef CUBISH_TILE
#define CUBISH_TILE

// This file defines a Tile class which is a wrapper for a pointer to a Color.  A
// Tile is simply for convenience of the programmer, and does not confer any speed
// optimizations.  The address of the Color the Tile uses may be obtained with
// Get_Color() and set with Set_Color().

#include <memory>

#include "Color.h"

class Tile {
public:
	// Constructors
	Tile();
	Tile(Color color);

	// Facilitators

	// Inspectors
	const Color& Get_Color() const;
	bool operator==(const Tile& other) const;

	// Mutators
	void Set_Color(Color color);

protected:
	Color color_;
};

#endif
