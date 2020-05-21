
#include "Tile.h"

// Constructors
Tile::Tile() {
}

Tile::Tile(Color_p color) {
	color_=color;
}

// Inspectors
Color_p Tile::Get_Color() const {
	return color_;
}

bool Tile::operator==(const Tile& other) const {
	return color_ == other.color_;
}

// Mutators
void Tile::Set_Color(Color_p color) {
	color_=color;
}
