
#include "Tile.h"

// Constructors
Tile::Tile(void) {
}

Tile::Tile(Color_p color) {
	M_Color=color;
}

// Inspectors
Color_p Tile::Get_Color(void) const {
	return M_Color;
}

bool Tile::operator==(const Tile& other) const {
	return M_Color == other.M_Color;
}

// Mutators
void Tile::Set_Color(Color_p color) {
	M_Color=color;
}
