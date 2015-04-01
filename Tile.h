// File: Tile.h
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.02.12
//
// This file defines a Tile class which is a wrapper for a pointer to a Color.  A 
// Tile is simply for convenience of the programmer, and does not confer any speed
// optimizations.  The address of the Color the Tile uses may be obtained with 
// Get_Color() and set with Set_Color().

#ifndef LIAM_TILE
#define LIAM_TILE

#include "Color.h"

class Tile {
public:
	// Constructors
	Tile(void) ;
	Tile(Color *color) ;

	// Destructor
	~Tile(void) ;

	// Facilitators

	// Inspectors
	Color *Get_Color(void) const ;

	// Mutators
	void Set_Color(Color *color) ;

protected:
	Color *M_Color ;
} ;

#endif // LIAM_TILE