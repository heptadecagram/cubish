
#include "Tile.h"

// Constructors
Tile::Tile() {
}

Tile::Tile(Color color) {
	color_=color;
}

// Inspectors
const Color& Tile::Get_Color() const {
	return color_;
}

bool Tile::operator==(const Tile& other) const {
	return color_ == other.color_;
}

// Mutators
void Tile::Set_Color(Color color) {
	color_=color;
}
