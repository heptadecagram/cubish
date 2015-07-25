
#include "Tile.h"

// Constructors
Tile::Tile() {
}

Tile::Tile(Color_p color) {
	_color=color;
}

// Inspectors
Color_p Tile::Get_Color() const {
	return _color;
}

bool Tile::operator==(const Tile& other) const {
	return _color == other._color;
}

// Mutators
void Tile::Set_Color(Color_p color) {
	_color=color;
}
