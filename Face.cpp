
#include "Face.h"

#include <algorithm>
#include <stdexcept>

// The elements of a Face are kept in a dynamic array which is kept in row-major order.

// Constructors
Face::Face(int Width, int Height, Color_p color) {
	_height=Height;
	_width=Width;

	_tiles.resize(Height*Width, std::make_shared<Tile>(color));
}

Face::Face(const Face &face) {
	_height=face.height();
	_width=face.length();

	std::copy(face._tiles.begin(), face._tiles.end(), std::back_inserter(_tiles));
}


// Inspectors
int Face::height() const {
	return _height;
}

int Face::length() const {
	return _width;
}

Tile_p& Face::operator()(int col, int row) {
	return _tiles[_width*--row + --col];
}

bool Face::solved() const {
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

	if(_height == _width) {
		for(auto x = 0; x < _width; ++x) {
			for(auto y = 0; y < _height; ++y) {
				(*this)(x+1, y+1) = temp[y  +  _width * (_height-1 - x)];
			}
		}
	}
	else {
		for(auto x = 0; x < _width; ++x) {
			for(auto y = 0; y < _height; ++y) {
				(*this)(x+1, y+1) = temp[(_width - 1 - x)  +  _width*(_height - 1 - y)];
			}
		}
	}
	return;
}

void Face::Spin_CW() {
	// For squares, just use the Rotate_CW() method
	if(_height==_width) {
		Rotate_CW();
		return;
	}

	// Create a temporary array of Tile to store the old Face
	auto temp = _tiles;

	// _height!=_width, so change the two
	auto Temp_Size=_height;
	_height=_width;
	_width=Temp_Size;

	for(auto x=0; x<_width; ++x)
		for(auto y=0; y<_height; ++y)
			// Use a bijection to map the coordinates
			_tiles[x + y*_width] = temp[y + _height*(_width-1-x)];
}

void Face::Spin_CCW() {
	if(_height==_width)
		Rotate_CCW();
	else {
		Rotate_CW();
		Spin_CW();
	}
}

