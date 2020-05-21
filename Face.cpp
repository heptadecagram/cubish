
#include "Face.h"

#include <algorithm>
#include <stdexcept>

// The elements of a Face are kept in a dynamic array which is kept in row-major order.

// Constructors
Face::Face(int Width, int Height, Color_p color) {
	height_=Height;
	width_=Width;

	tiles_.resize(Height*Width, std::make_shared<Tile>(color));
}

Face::Face(const Face &face) {
	height_=face.height();
	width_=face.length();

	std::copy(face.tiles_.begin(), face.tiles_.end(), std::back_inserter(tiles_));
}


// Inspectors
int Face::height() const {
	return height_;
}

int Face::length() const {
	return width_;
}

Tile_p& Face::operator()(int col, int row) {
	return tiles_[width_*--row + --col];
}

bool Face::solved() const {
	return std::all_of(tiles_.begin(), tiles_.end(),
			[this](const Tile_p& t) { return t == tiles_[0]; });
}


// Mutators
// Flood: Make an entire Face a single Color
void Face::Flood(Color_p color) {
	std::fill(tiles_.begin(), tiles_.end(), std::make_shared<Tile>(color));
}

void Face::Rotate_CCW() {
	// Three rights make a left
	Rotate_CW();
	Rotate_CW();
	Rotate_CW();
}

void Face::Rotate_CW() {
	auto temp = tiles_;

	if(height_ == width_) {
		for(auto x = 0; x < width_; ++x) {
			for(auto y = 0; y < height_; ++y) {
				(*this)(x+1, y+1) = temp[y  +  width_ * (height_-1 - x)];
			}
		}
	}
	else {
		for(auto x = 0; x < width_; ++x) {
			for(auto y = 0; y < height_; ++y) {
				(*this)(x+1, y+1) = temp[(width_ - 1 - x)  +  width_*(height_ - 1 - y)];
			}
		}
	}
	return;
}

void Face::Spin_CW() {
	// For squares, just use the Rotate_CW() method
	if(height_==width_) {
		Rotate_CW();
		return;
	}

	// Create a temporary array of Tile to store the old Face
	auto temp = tiles_;

	// height_!=width_, so change the two
	auto Temp_Size=height_;
	height_=width_;
	width_=Temp_Size;

	for(auto x=0; x<width_; ++x)
		for(auto y=0; y<height_; ++y)
			// Use a bijection to map the coordinates
			tiles_[x + y*width_] = temp[y + height_*(width_-1-x)];
}

void Face::Spin_CCW() {
	if(height_==width_)
		Rotate_CCW();
	else {
		Rotate_CW();
		Spin_CW();
	}
}

