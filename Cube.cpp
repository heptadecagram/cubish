
#include "Cube.h"

#include <algorithm>
#include <stdexcept>

#include "ui.h"


// Constructors
Cube::Cube(int Width, int Height, int Depth, std::array<Color, 6> side_colors) {

	sides_[0] = Face(Width, Height, side_colors[0]);
	sides_[1] = Face(Depth, Height, side_colors[1]);
	sides_[2] = Face(Width,  Depth, side_colors[2]);
	sides_[3] = Face(Depth, Height, side_colors[3]);
	sides_[4] = Face(Width,  Depth, side_colors[4]);
	sides_[5] = Face(Width, Height, side_colors[5]);
}

// Facilitators
int Cube::Make_GL_List() {
	static int List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE); {

		for(auto n1 = 0; n1 < 6; ++n1) {
			// Orient the viewing volume to face the correct side
			View_Side(n1);

			for(auto n2 = 0; n2 < sides_[n1].height(); ++n2) {
				for(auto n3 = 0; n3 < sides_[n1].length(); ++n3) {

					auto color = sides_[n1](n3+1, sides_[n1].height() - n2)->Get_Color();
					glColor3d(color.red(), color.green(), color.blue());

					// Raise the height of all colored squares to avoid clipping problems
					glBegin(GL_QUADS); {
						glVertex3d(n3 + 0.1, n2 + 0.1, 0.03);
						glVertex3d(n3 + 0.1, n2 + 0.9, 0.03);
						glVertex3d(n3 + 0.9, n2 + 0.9, 0.03);
						glVertex3d(n3 + 0.9, n2 + 0.1, 0.03);
					} glEnd();
				}
			}

			// Create a black background to put the colored squares on
			glColor3d(0, 0, 0);
			glBegin(GL_QUADS); {
				glVertex2s(                  0,                   0);
				glVertex2s(sides_[n1].length(),                   0);
				glVertex2s(sides_[n1].length(), sides_[n1].height());
				glVertex2s(                  0, sides_[n1].height());
			} glEnd();

			// Now, undo the orientation, returning to the original state
			Undo_View_Side(n1);
		}

	} glEndList();

	return List_ID;
}

// This function creates a OpenGL display list of the cube, minus a single slice
int Cube::Make_Section_GL_List(int Side, int Depth) {
	static auto List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE); {

		// Similar to View_Side(), but only does a rotation, not a translation as well
		switch(Side) {
			case 0:
				break;
			case 1:
				glRotatef(90, 0, -1, 0);
				break;
			case 2:
				glRotatef(90, -1, 0, 0);
				break;
			case 3:
				glRotatef(90, 0, 1, 0);
				break;
			case 4:
				glRotatef(90, 1, 0, 0);
				break;
			case 5:
				glRotatef(180, 1, 0, 0);
				break;
			default:
				break;
		}

		// Start drawing from the specified Side
		auto Old_Front=Set_Front(Side);

		for(auto n1 = (Depth==1?1:0); n1 < (Depth == sides_[1].length() ? 5 : 6); ++n1) {

			// Rotate/translate the appropriate amount for the side
			View_Side(n1);

			for(auto n2 = 0; n2 < sides_[n1].height(); ++n2) {
				for(auto n3 = 0; n3 < sides_[n1].length(); ++n3) {
					// If the current slice is not the removed one, and is also not a front or
					// back side that would be affected, draw it
					if(!((n1==1 && n3 == sides_[n1].length() - Depth) ||
								(n1==2 && n2+1 == Depth) || (n1==3 && n3+1==Depth) ||
								(n1==4 && n2 == sides_[n1].height() - Depth) ) ) {
						// Black background squares
						glColor3d(0, 0, 0);
						glBegin(GL_QUADS); {
							glVertex2s(n3  , n2);
							glVertex2s(n3  , n2+1);
							glVertex2s(n3+1, n2+1);
							glVertex2s(n3+1, n2);
						} glEnd();

						// Pretty colored squares are being drawn here
						auto color = sides_[n1](n3+1, sides_[n1].height() - n2)->Get_Color();
						glColor3d(color.red(), color.green(), color.blue());
						glBegin(GL_QUADS); {
							glVertex3d(n3 + 0.1, n2 + 0.1, .03);
							glVertex3d(n3 + 0.1, n2 + 0.9, .03);
							glVertex3d(n3 + 0.9, n2 + 0.9, .03);
							glVertex3d(n3 + 0.9, n2 + 0.1, .03);
						} glEnd();
					}
				}
			}

			Undo_View_Side(n1);
		}

		// Since Set_Front() was called, this isn't the real front of the Cube
		View_Side(0);

		// Find how far down to go, change to black color
		glTranslated(0, 0, -Depth);
		glColor3d(0, 0, 0);

		// Draw a black square on top, if it is open
		if(Depth != sides_[1].length() ) {
			glBegin(GL_QUADS); {
				glVertex2s(                 0,                  0);
				glVertex2s(                 0, sides_[0].height());
				glVertex2s(sides_[0].length(), sides_[0].height());
				glVertex2s(sides_[0].length(),                  0);
			} glEnd();
		}

		// Move up a bit
		glTranslated(0, 0, 1);

		// If we are not at the top, draw another black square to close off
		// the open side
		if(Depth!=1) {
			glBegin(GL_QUADS); {
				glVertex2s(                 0,                  0);
				glVertex2s(                 0, sides_[0].height());
				glVertex2s(sides_[0].length(), sides_[0].height());
				glVertex2s(sides_[0].length(),                  0);
			} glEnd();
		}

		// Go back to where we used to be
		glTranslated(0, 0, Depth-1);
		Undo_View_Side(0);
		Set_Front(Old_Front);

		// See beginning of this function
		switch(Side) {
			case 0:
				break;
			case 1:
				glRotatef(-90, 0, -1, 0);
				break;
			case 2:
				glRotatef(-90, -1, 0, 0);
				break;
			case 3:
				glRotatef(-90, 0, 1, 0);
				break;
			case 4:
				glRotatef(-90, 1, 0, 0);
				break;
			case 5:
				glRotatef(-180, 1, 0, 0);
				break;
			default:
				break;
		}

	} glEndList();

	return List_ID;
}

Face& Cube::operator[](const int Side) {
	return sides_[Side];
}

// This function draws a single slice of a Cube.  Surprisingly complex to do.
int Cube::Make_Slice_GL_List(int Side, int Depth) {
	static int List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE); {

		// Similar to View_Side(), but only does a rotation, not a translation as well
		switch(Side) {
			case 0:
				break;
			case 1:
				glRotatef(90, 0, -1, 0);
				break;
			case 2:
				glRotatef(90, -1, 0, 0);
				break;
			case 3:
				glRotatef(90, 0, 1, 0);
				break;
			case 4:
				glRotatef(90, 1, 0, 0);
				break;
			case 5:
				glRotatef(180, 1, 0, 0);
				break;
			default:
				break;
		}

		// Start drawing from the specified Side
		auto Old_Front=Set_Front(Side);

		// Since this slice might not be square, we have to adjust for each side
		int Temp_Slice_Length, X_Coord, Y_Coord;
		for(auto n = 1; n < 5; ++n) {

			// Get ready to draw on a side
			View_Side(n);

			if(n==1 || n==3) {
				Temp_Slice_Length = sides_[0].height();
			} else { // n==2 || n==4
				Temp_Slice_Length = sides_[0].length();
			}

			for(auto nn = 0; nn < Temp_Slice_Length; ++nn) {

				switch(n) {
					case 1:
						X_Coord = sides_[1].length() - Depth+1;
						Y_Coord = nn+1;
						break;
					case 2:
						X_Coord = nn+1;
						Y_Coord = Depth;
						break;
					case 3:
						X_Coord = Depth;
						Y_Coord = Temp_Slice_Length-nn;
						break;
					case 4:
						X_Coord = nn+1;
						Y_Coord = sides_[2].height() - Depth+1;
						break;
					default:
						break;
				}

				// Black background squares
				glColor3d(0, 0, 0);
				glBegin(GL_QUADS); {
					glVertex2s(X_Coord-1, Y_Coord-1);
					glVertex2s(X_Coord-1, Y_Coord);
					glVertex2s(X_Coord  , Y_Coord);
					glVertex2s(X_Coord  , Y_Coord-1);
				} glEnd();

				// Colored squares, raised for visibility
				auto color = sides_[n](X_Coord, sides_[n].height() - Y_Coord+1)->Get_Color();
				glColor3d(color.red(), color.green(), color.blue());
				glBegin(GL_QUADS); {
					glVertex3d(X_Coord-0.9, Y_Coord-0.9, 0.03);
					glVertex3d(X_Coord-0.9, Y_Coord-0.1, 0.03);
					glVertex3d(X_Coord-0.1, Y_Coord-0.1, 0.03);
					glVertex3d(X_Coord-0.1, Y_Coord-0.9, 0.03);
				} glEnd();
			}

			// Return from the recent rotation/translation
			Undo_View_Side(n);
		}

		// Set_Front() was called, this is not the real side
		View_Side(0);

		// Move to the correct depth
		glTranslated(0, 0, -Depth);

		// Draw black caps
		glColor3d(0, 0, 0);
		glBegin(GL_QUADS); {
			glVertex2i(                 0,                  0);
			glVertex2i(                 0, sides_[0].height());
			glVertex2i(sides_[0].length(), sides_[0].height());
			glVertex2i(sides_[0].length(),                  0);
		} glEnd();

		// If we are at the bottom, draw the appropriate colored squares
		if(Depth == sides_[1].length()) {
			for(auto n = 0; n < sides_[5].height(); ++n) {
				for(auto nn = 0; nn < sides_[5].length(); ++nn) {
					// Yay pretty colored squares (z-offset for viewability
					auto color = sides_[5](nn+1, n+1)->Get_Color();
					glColor3d(color.red(), color.green(), color.blue());
					glBegin(GL_QUADS); {
						glVertex3d(nn+0.1, n+0.1, -0.03);
						glVertex3d(nn+0.1, n+0.9, -0.03);
						glVertex3d(nn+0.9, n+0.9, -0.03);
						glVertex3d(nn+0.9, n+0.1, -0.03);
					} glEnd();
				}
			}
		}

		// Go up a bit and draw the other black cap
		glTranslated(0, 0, 1);
		glColor3d(0, 0, 0);
		glBegin(GL_QUADS); {
			glVertex3i(                 0,                  0, 0);
			glVertex3i(                 0, sides_[0].height(), 0);
			glVertex3i(sides_[0].length(), sides_[0].height(), 0);
			glVertex3i(sides_[0].length(),                  0, 0);
		} glEnd();

		// If we are at the top, draw the appropriate colored squares
		if(Depth==1) {
			for(auto n = 0; n < sides_[0].height(); ++n) {
				for(auto nn = 0; nn < sides_[0].length(); ++nn) {
					// Change to the right color, draw the square at an offset
					auto color = sides_[0](nn+1, sides_[0].height() - n)->Get_Color();
					glColor3d(color.red(), color.green(), color.blue());
					glBegin(GL_QUADS); {
						glVertex3d(nn+0.1, n+0.1, 0.03);
						glVertex3d(nn+0.1, n+0.9, 0.03);
						glVertex3d(nn+0.9, n+0.9, 0.03);
						glVertex3d(nn+0.9, n+0.1, 0.03);
					} glEnd();
				}
			}
		}

		// Restore the old view
		glTranslated(0, 0, Depth-1);
		Undo_View_Side(0);
		Set_Front(Old_Front);

		// See the beginning of this function for this explanation
		switch(Side) {
			case 0:
				break;
			case 1:
				glRotatef(-90, 0, -1, 0);
				break;
			case 2:
				glRotatef(-90, -1, 0, 0);
				break;
			case 3:
				glRotatef(-90, 0, 1, 0);
				break;
			case 4:
				glRotatef(-90, 1, 0, 0);
				break;
			case 5:
				glRotatef(-180, 1, 0, 0);
				break;
			default:
				break;
		}

	} glEndList(); // List_ID

	return List_ID;
}

// This function reorients the viewing volume and translates to the lower-left corner of
// the Face viewed.  This is to facilitate ease of drawing.  Call Undo_View_Side() to
// reverse the changes this function makes!
void Cube::View_Side(int Side) {
	// Get some measurements
	auto Width  = sides_[0].length();
	auto Height = sides_[0].height();
	auto Depth  = sides_[1].length();

	switch(Side) {
	case 0:
		glTranslated(-Width/2.0, -Height/2.0, Depth/2.0);
		break;
	case 1:
		glRotatef(90, 0, -1, 0);
		glTranslated(-Depth/2.0, -Height/2.0, Width/2.0);
		break;
	case 2:
		glRotatef(90, -1, 0, 0);
		glTranslated(-Width/2.0, -Depth/2.0, Height/2.0);
		break;
	case 3:
		glRotatef(90, 0, 1, 0);
		glTranslated(-Depth/2.0, -Height/2.0, Width/2.0);
		break;
	case 4:
		glRotatef(90, 1, 0, 0);
		glTranslated(-Width/2.0, -Depth/2.0, Height/2.0);
		break;
	case 5:
		glRotatef(180, 1, 0, 0);
		glTranslated(-Width/2.0, -Height/2.0, Depth/2.0);
		break;
	default:
		break;
	}
}

// This function reverses the effects of View_Side().  See that function for
// explanation.
void Cube::Undo_View_Side(int Side) {
	auto Width  = sides_[0].length();
	auto Height = sides_[0].height();
	auto Depth  = sides_[1].length();

	switch(Side) {
	case 0:
		glTranslated(Width/2.0, Height/2.0, -Depth/2.0);
		break;
	case 1:
		glTranslated(Depth/2.0, Height/2.0, -Width/2.0);
		glRotatef(-90, 0, -1, 0);
		break;
	case 2:
		glTranslated(Width/2.0, Depth/2.0, -Height/2.0);
		glRotatef(-90, -1, 0, 0);
		break;
	case 3:
		glTranslated(Depth/2.0, Height/2.0, -Width/2.0);
		glRotatef(-90, 0, 1, 0);
		break;
	case 4:
		glTranslated(Width/2.0, Depth/2.0, -Height/2.0);
		glRotatef(-90, 1, 0, 0);
		break;
	case 5:
		glTranslated(Width/2.0, Height/2.0, -Depth/2.0);
		glRotatef(-180, 1, 0, 0);
		break;
	default:
		break;
	}
}

// This function saves a Cube to an open file pointer.  Pass the appropriate
// Color information in case of sides with the same color.
void Cube::Save(std::array<Color, 6> Color_List, std::ofstream& File) {
	// Iterate through the Color_List, turning the RGB values into bytes,
	// and then put them in the file
	for(int n=0; n<6; n++) {
		char Red = static_cast<char>(255.0*Color_List[n].red());
		char Green = static_cast<char>(255.0*Color_List[n].green());
		char Blue = static_cast<char>(255.0*Color_List[n].blue());
		File << Red << Green << Blue;
	}

	// Put the dimensions of the Cube into the file as bytes
	File << static_cast<char>(sides_[0].length())
		<< static_cast<char>(sides_[0].height())
		<< static_cast<char>(sides_[1].length());

	// These insanely-nested for() loops go through the entire Cube, comparing
	// the addresses of the Colors on the Cube and Color_List.  Note that addresses
	// must be compared and not values.  Then print out the byte value of its
	// Color_List index.
	for(int n1=0; n1<6; n1++)
		for(int n2=0; n2 < sides_[n1].height(); ++n2)
			for(int n3=0; n3 < sides_[n1].length(); ++n3)
				for(int n4=0; n4<6; n4++)
					if(sides_[n1](n3+1, n2+1)->Get_Color() == Color_List[n4]) {
						File << static_cast<char>(n4);
					}
}

// This function loads a Cube from an open file pointer.  It also assigns the
// appropriate Color_List information.  Warning: No exception handling ability yet.
void Cube::Load(std::array<Color, 6>  Color_List, std::ifstream& File) {
	// The first 18 bytes are color information
	for(int n=0; n<6; n++) {
		unsigned char Red=0, Green=0, Blue=0;
		File >> Red >> Green >> Blue;
		Color_List[n].Set(Red/255.0, Green/255.0, Blue/255.0);
	}

	// The next three bytes are dimensions
	unsigned char Width=1, Height=1, Depth=1;
	File >> Width >> Height >> Depth;

	// Make new faces.
	sides_[0] = Face(Width, Height, Color_List[0]);
	sides_[1] = Face(Depth, Height, Color_List[1]);
	sides_[2] = Face(Width,  Depth, Color_List[2]);
	sides_[3] = Face(Depth, Height, Color_List[3]);
	sides_[4] = Face(Width,  Depth, Color_List[4]);
	sides_[5] = Face(Width, Height, Color_List[5]);

	unsigned char Color_Index=0;
	// Now, obtain the Color_List information stored in the file and put it on the
	// Cube.
	for(auto n1=0; n1<6; n1++)
		for(auto n2=0; n2 < sides_[n1].height(); ++n2)
			for(auto n3=0; n3 < sides_[n1].length(); ++n3) {
				File >> Color_Index;
				sides_[n1](n3+1, n2+1)->Set_Color(Color_List[static_cast<int>(Color_Index)]);
			}
}


// Mutators
// This function randomly picks a point on the cube and a direction, then
// calls Twist() based on those numbers.  It is possible to specify a number
// of times to twist, default is 100, standard for competition.
void Cube::Randomize(int Twists) {
	Direction Random_Direction;
	int Random_Side, Random_Row, Random_Column;
	for(auto n=0; n<Twists; n++) {
		Random_Side = Random(6);
		Random_Row = 1 + Random(sides_[Random_Side].height());
		Random_Column = 1 + Random(sides_[Random_Side].length());
		Random_Direction = Direction(1 + Random(4));
		Twist(Random_Side, Random_Column, Random_Row, Random_Direction);
	}
}

// This is the big cube-modifying function.  Given a side and a distance down that
// side, rotate that slice of Cubeness.  Returns true if it was possible to
// rotate in that direction.
bool Cube::Rotate_CW(int Side, int Offset) {
	// Reorient the sides of the Cube so we only have to solve this problem once
	auto Old_Front=Set_Front(Side);

	// These will be used quite a bit
	auto Width  = sides_[0].length();
	auto Height = sides_[0].height();
	auto Depth  = sides_[1].length();

	if(Offset>Depth || Offset<1)
		throw std::out_of_range("Rotate_CW");

	// Do not rotate on a single-thickness axis
	if(Depth==1) {
		Set_Front(Old_Front);
		return false;
	}


	// If the slice that is being turned is square....
	if(Width==Height) {
		for(auto n=0; n<Width; n++) {
			// Here, grab each Tile one at a time from a side on the
			// slice, and put it onto the next side.
			auto Temp_Tile = sides_[4](n+1, Offset);
			sides_[4](n+1, Offset) = sides_[3](Offset, Height-n);
			sides_[3](Offset, Height-n) = sides_[2](Width-n, Depth-Offset+1);
			sides_[2](Width-n, Depth-Offset+1) = sides_[1](Depth-Offset+1, n+1);
			sides_[1](Depth-Offset+1, n+1) = Temp_Tile;
		}
	}
	// This is for a rectangular slice, since a rotation is 180, not 90
	else { // (Width!=Height)
		for(auto n=0; n<Width; n++) {
			// First, exchange one side
			auto Temp_Tile = sides_[4](n+1, Offset);
			sides_[4](n+1, Offset) = sides_[2](Width-n, Depth-Offset+1);
			sides_[2](Width-n, Depth-Offset+1) = Temp_Tile;
		}

		for(auto n=0; n<Height; n++) {
			// Exchange the other side
			auto Temp_Tile = sides_[3](Offset, n+1);
			sides_[3](Offset, n+1) = sides_[1](Depth-Offset+1, Height-n);
			sides_[1](Depth-Offset+1, Height-n) = Temp_Tile;
		}
	}

	// If the Cube is rotated at either end, the Face on that end must also be
	// rotated.  Check for this, do it if necessary.
	if(Offset==1)
		sides_[0].Rotate_CW();
	if(Offset==Depth)
		sides_[5].Rotate_CCW();

	// Change back to where we used to be, and let it be known that the Cube
	// was rotated
	Set_Front(Old_Front);
	return true;
}

bool Cube::Rotate_CCW(int Side, int Offset) {
	// Three rights make a left
	Rotate_CW(Side, Offset);
	Rotate_CW(Side, Offset);
	// We only need to return one of these, since the Offset is the same for
	// each one
	return Rotate_CW(Side, Offset);
}

// This function causes the Cube to be twisted, given a side, coordinate, and direction.
// If the Cube changed in some way, true is returned, otherwise, false.
bool Cube::Twist(int Side, int Column, int Row, Direction direction) {
	// Got to have some place to start and some place to go
	if(direction == Direction::NONE || Side < 0 || 5 < Side) {
		return false;
	}

	// Make life easier, pretend we are twisting from side 1
	auto Old_Front=Set_Front(Side);

	// Figure out where we are rotating, what direction, and if the Cube changed
	bool Returner;
	switch(direction) {
		case Direction::Up:
			Returner=Rotate_CW(1, Column);
			break;
		case Direction::Down:
			Returner=Rotate_CCW(1, Column);
			break;
		case Direction::Left:
			Returner=Rotate_CCW(2, Row);
			break;
		case Direction::Right:
			Returner=Rotate_CW(2, Row);
			break;
		case Direction::NONE:
			throw "Unreachable";
	}

	// Leave the Cube the way we found it
	Set_Front(Old_Front);
	return Returner;
}

// Just an alias for Twist()
bool Cube::Twist(Vector Position, Direction direction) {
	return Twist(Position[1]-1, Position[2], Position[3], direction);
}


// Inspectors
// Is the Cube solved?
bool Cube::solved() const {
	return std::all_of(sides_.begin(), sides_.end(), [](const Face& f){ return f.solved(); });
}

// Private Functions
// The Cube must have some random method for randomizing itself
int Cube::Random(int Limit) {
	static bool Seeded=false;

	// If we haven't called this function before, initialize randomocity
	if(!Seeded) {
		srand(static_cast<unsigned>(time(NULL) ) );
		Seeded = true;
	}

	// It's not fancy because it doesn't have to be.
	return rand() % Limit;
}

// This lifesaving function modifies the entire Cube so that the Cube now treats
// the passed Side as its new front.  This way, algorithms only have to be
// written for one side of the Cube.  The function returns the location of where
// the old side 0 is.  Always remember to set the Cube back to normal after your
// algorithm.
int Cube::Set_Front(int Side) {
	// We need a temporary Face
	auto Front_Face = sides_[0];
	int Returner;

	// This is the core.  Notice that Spin_*() function must be called on
	// some of the Face to preserve the orientation of the Face with regard
	// to the standard Cube.
	switch(Side) {
	case 0:
		// It's already facing 1.  Duh.
		Returner=0;
		break;
	case 1:
		sides_[0] = sides_[1];
		sides_[5].Spin_CW();
		sides_[5].Spin_CW();
		sides_[1] = sides_[5];
		sides_[3].Spin_CW();
		sides_[3].Spin_CW();
		sides_[5] = sides_[3];
		sides_[3] = Front_Face;
		sides_[2].Spin_CCW();
		sides_[4].Spin_CW();
		Returner=3;
		break;
	case 2:
		sides_[0] = sides_[2];
		sides_[2] = sides_[5];
		sides_[5] = sides_[4];
		sides_[4] = Front_Face;
		sides_[1].Spin_CW();
		sides_[3].Spin_CCW();
		Returner=4;
		break;
	case 3:
		sides_[0] = sides_[3];
		sides_[5].Spin_CW();
		sides_[5].Spin_CW();
		sides_[3] = sides_[5];
		sides_[1].Spin_CW();
		sides_[1].Spin_CW();
		sides_[5] = sides_[1];
		sides_[1] = Front_Face;
		sides_[2].Spin_CW();
		sides_[4].Spin_CCW();
		Returner=1;
		break;
	case 4:
		sides_[0] = sides_[4];
		sides_[4] = sides_[5];
		sides_[5] = sides_[2];
		sides_[2] = Front_Face;
		sides_[1].Spin_CCW();
		sides_[3].Spin_CW();
		Returner=2;
		break;
	case 5:
		sides_[0] = sides_[5];
		sides_[5] = Front_Face;
		Front_Face= sides_[2];
		sides_[2] = sides_[4];
		sides_[4] = Front_Face;
		sides_[1].Spin_CW();
		sides_[1].Spin_CW();
		sides_[3].Spin_CW();
		sides_[3].Spin_CW();
		Returner=5;
		break;
	default:
		throw std::out_of_range("SetFront");
	}
	return Returner;
}
