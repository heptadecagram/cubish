
#include "Face.h"

#include <algorithm>
#include <stdexcept>

// The elements of a Face are kept in a dynamic array which is kept in row-major order.

// Constructors
Face::Face(int Width, int Height, Color_p color) {
	M_Height=Height;
	M_Width=Width;

	_tiles.resize(Height*Width, std::make_shared<Tile>(color));
}

Face::Face(const Face &face) {
	M_Height=face.height();
	M_Width=face.length();

	std::copy(face._tiles.begin(), face._tiles.end(), std::back_inserter(_tiles));
}


// Inspectors
int Face::height() const {
	return M_Height;
}

int Face::length() const {
	return M_Width;
}

Tile_p& Face::operator()(int col, int row) {
	return _tiles[M_Width*--row + --col];
}

bool Face::Is_Solved() const {
	return std::all_of(_tiles.begin(), _tiles.end(),
			[this](const Tile_p& t) { return t == _tiles[0]; });
}


// Mutators
// Flood: Make an entire Face a single Color
void Face::Flood(Color_p color) {
	std::fill(_tiles.begin(), _tiles.end(), std::make_shared<Tile>(color));
}

void Face::Rotate_CCW() {
	// Three rights make a left
	Rotate_CW();
	Rotate_CW();
	Rotate_CW();
}

void Face::Rotate_CW() {
	auto temp = _tiles;

	if(M_Height == M_Width) {
		for(auto x = 0; x < M_Width; ++x) {
			for(auto y = 0; y < M_Height; ++y) {
				(*this)(x+1, y+1) = temp[y  +  M_Width * (M_Height-1 - x)];
			}
		}
	}
	else {
		for(auto x = 0; x < M_Width; ++x) {
			for(auto y = 0; y < M_Height; ++y) {
				(*this)(x+1, y+1) = temp[(M_Width - 1 - x)  +  M_Width*(M_Height - 1 - y)];
			}
		}
	}
	return;
}

void Face::Spin_CW() {
	// For squares, just use the Rotate_CW() method
	if(M_Height==M_Width) {
		Rotate_CW();
		return;
	}

	// Create a temporary array of Tile to store the old Face
	auto temp = _tiles;

	// M_Height!=M_Width, so change the two
	auto Temp_Size=M_Height;
	M_Height=M_Width;
	M_Width=Temp_Size;

	for(auto x=0; x<M_Width; ++x)
		for(auto y=0; y<M_Height; ++y)
			// Use a bijection to map the coordinates
			_tiles[x + y*M_Width] = temp[y + M_Height*(M_Width-1-x)];
}

void Face::Spin_CCW() {
	if(M_Height==M_Width)
		Rotate_CCW();
	else {
		Rotate_CW();
		Spin_CW();
	}
}

