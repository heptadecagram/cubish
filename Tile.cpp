
#include "Tile.h"

// Constructors
Tile::Tile(void) {
}

Tile::Tile(Color *color) {
	M_Color=color ;
}

// Destructors
Tile::~Tile(void) {
	delete M_Color ;
}

// Facilitators

// Inspectors
Color *Tile::Get_Color(void) const {
	return M_Color ;
}

// Mutators
void Tile::Set_Color(Color *color) {
	M_Color=color ;
}
