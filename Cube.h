//
// The Cube class is an array of six Face classes, aranged in the following order:
//
//     +---+
//     |   |
//     | 2 |
//     |   |
// +---+---+---+
// |   |   |   |
// | 1 | 0 | 3 |
// |   |   |   |
// +---+---+---+
//     |   |
//     | 4 |
//     |   |
//     +---+
//     |   |
//     | 5 |
//     |   |
//     +---+
//
// Internally, lower all Face indicies by one.  Calling Get_Side() requires the use
// of natural numbers.  A Cube can be rotated by calling Rotate_CW() or Rotate_CCW()
// with a side, to indicate the direction of a vector rotation, and an integer offset
// which determines how deep the rotation is.  An offset of 1 rotates on that face,
// an offset of 2 rotates the ring of Tile one layer down.  The Twist() functions work
// by calling the Rotate_*() functions.  Twist() requires the side and coordinates of
// that side, as well as a direction to twist in.  By passing the appropriate
// file pointers, a Cube can be saved or loaded to disk using the Save() and Load()
// functions.  solved() calls the solved() function for each individual Face.
// The Make_*() functions generate OpenGL list identifiers for use in glCallList().
// A Cube can be randomized using the Random() function.

#ifndef CUBISH_CUBE
#define CUBISH_CUBE

#include <array>
#include <fstream>

#include "Face.h"
#include "Vector.h"

// For use in the Twist() functions
enum class Direction : char { NONE, Up, Down, Left, Right };

class Cube {
public:
	Cube() {};

	// Constructors
	Cube(int Width, int Height, int Depth, std::array<Color_p, 6> side_colors);

	// Facilitators
	int Make_GL_List();
	int Make_Section_GL_List(int Side, int Depth);
	int Make_Slice_GL_List(int Side, int Depth);

	void View_Side(int Side);
	void Undo_View_Side(int Side);

	void Save(std::array<Color_p, 6> Color_List, std::ofstream& File);
	void Load(std::array<Color_p, 6> Color_List, std::ifstream& File);

	Face& operator[](const int Side);

	// Mutators
	void Randomize(int Twists=100);
	bool Rotate_CW(int Side, int Offset=1);
	bool Rotate_CCW(int Side, int Offset=1);
	bool Twist(int Side, int Column, int Row, Direction direction);
	bool Twist(Vector Position, Direction direction);

	// Inspectors
	bool solved() const;

private:
	// Private Functions
	int Random(int Max);
	int Set_Front(int Side);

	// Member Variables
	std::array<Face, 6> sides_;
};

#endif // CUBISH_CUBE
