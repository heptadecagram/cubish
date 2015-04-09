

#include "Cube.h"

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdexcept>

#include <GLUT/glut.h>

// Constructors
Cube::Cube(int Width, int Height, int Depth,
		   Color *Side_1_Color, Color *Side_2_Color, Color *Side_3_Color,
		   Color *Side_4_Color, Color *Side_5_Color, Color *Side_6_Color) {
	M_Sides[0]=new Face(Width, Height, Side_1_Color) ;
	M_Sides[1]=new Face(Depth, Height, Side_2_Color) ;
	M_Sides[2]=new Face(Width, Depth, Side_3_Color) ;
	M_Sides[3]=new Face(Depth, Height, Side_4_Color) ;
	M_Sides[4]=new Face(Width, Depth, Side_5_Color) ;
	M_Sides[5]=new Face(Width, Height, Side_6_Color) ;

}

// Destructor
Cube::~Cube(void) {
}

// Facilitators
int Cube::Make_GL_List(void) {
	static int List_ID=glGenLists(1) ;

	glNewList(List_ID, GL_COMPILE) ;

	for(int n1=1; n1<=6; n1++) {

		// Orient the viewing volume to face the correct side
		View_Side(n1) ;

		for(int n2=1; n2<=M_Sides[n1-1]->Get_Height(); n2++) {
			for(int n3=1; n3<=M_Sides[n1-1]->Get_Width(); n3++) {
				M_Sides[n1-1]->Get_Tile(n3, M_Sides[n1-1]->Get_Height()-n2+1)->Get_Color()->Change_To() ;
				// Raise the height of all colored squares to avoid clipping problems
				glBegin(GL_QUADS) ;
					glVertex3d(n3-.9, n2-.9, .03) ;
					glVertex3d(n3-.9, n2-.1, .03) ;
					glVertex3d(n3-.1, n2-.1, .03) ;
					glVertex3d(n3-.1, n2-.9, .03) ;
				glEnd() ; // GL_QUADS
			}
		}

		// Create a black background to put the colored squares on
		glColor3d(0, 0, 0) ;
		glBegin(GL_QUADS) ;
			glVertex3i(0, 0, 0) ;
			glVertex3i(M_Sides[n1-1]->Get_Width(), 0, 0) ;
			glVertex3i(M_Sides[n1-1]->Get_Width(), M_Sides[n1-1]->Get_Height(), 0) ;
			glVertex3i(0, M_Sides[n1-1]->Get_Height(), 0) ;
		glEnd() ; // GL_QUADS

		// Now, undo the orientation, returning to the original state
		Undo_View_Side(n1) ;
	}

	glEndList() ; // List_ID

	return List_ID ;
}

// This function creates a OpenGL display list of the cube, minus a single slice
int Cube::Make_Section_GL_List(int Side, int Depth) {
	static int List_ID=glGenLists(1) ;

	glNewList(List_ID, GL_COMPILE) ;

	// Similar to View_Side(), but only does a rotation, not a translation as well
	switch(Side) {
	case 1:
		break ;
	case 2:
		glRotatef(90, 0, -1, 0) ;
		break ;
	case 3:
		glRotatef(90, -1, 0, 0) ;
		break ;
	case 4:
		glRotatef(90, 0, 1, 0) ;
		break ;
	case 5:
		glRotatef(90, 1, 0, 0) ;
		break ;
	case 6:
		glRotatef(180, 1, 0, 0) ;
		break ;
	default:
		break ;
	}

	// Start drawing from the specified Side
	int Old_Front=Set_Front(Side) ;

	for(int n1=(Depth==1?2:1); n1<=(Depth==M_Sides[1]->Get_Width()?5:6); n1++) {

		// Rotate/translate the appropriate amount for the side
		View_Side(n1) ;

		for(int n2=1; n2<=M_Sides[n1-1]->Get_Height(); n2++) {
			for(int n3=1; n3<=M_Sides[n1-1]->Get_Width(); n3++) {
				// If the current slice is not the removed one, and is also not a front or
				// back side that would be affected, draw it
				if(!((n1==2 && n3==M_Sides[n1-1]->Get_Width()-Depth+1) ||
					 (n1==3 && n2==Depth) || (n1==4 && n3==Depth) ||
					 (n1==5 && n2==M_Sides[n1-1]->Get_Height()-Depth+1) ) ) {
					// Black background squares
					glColor3d(0, 0, 0) ;
					glBegin(GL_QUADS) ;
						glVertex3i(n3-1, n2-1, 0) ;
						glVertex3i(n3-1, n2, 0) ;
						glVertex3i(n3, n2, 0) ;
						glVertex3i(n3, n2-1, 0) ;
					glEnd() ; // GL_QUADS

					// Pretty colored squares are being drawn here
					M_Sides[n1-1]->Get_Tile(n3, M_Sides[n1-1]->Get_Height()-n2+1)->Get_Color()->Change_To() ;
					glBegin(GL_QUADS) ;
						glVertex3d(n3-.9, n2-.9, .03) ;
						glVertex3d(n3-.9, n2-.1, .03) ;
						glVertex3d(n3-.1, n2-.1, .03) ;
						glVertex3d(n3-.1, n2-.9, .03) ;
					glEnd() ; // GL_QUADS
				}
			}
		}

		Undo_View_Side(n1) ;
	}

	// Since Set_Front() was called, this isn't the real front of the Cube
	View_Side(1) ;

	// Find how far down to go, change to black color
	glTranslatef(0, 0, -Depth) ;
	glColor3d(0, 0, 0) ;

	// Draw a black square on top, if it is open
	if(Depth!=M_Sides[1]->Get_Width() ) {
		glBegin(GL_QUADS) ;
			glVertex3i(0, 0, 0) ;
			glVertex3i(0, M_Sides[0]->Get_Height(), 0) ;
			glVertex3i(M_Sides[0]->Get_Width(), M_Sides[0]->Get_Height(), 0) ;
			glVertex3i(M_Sides[0]->Get_Width(), 0, 0) ;
		glEnd() ; // GL_QUADS
	}

	// Move up a bit
	glTranslatef(0, 0, 1) ;

	// If we are not at the top, draw another black square to close off
	// the open side
	if(Depth!=1) {
		glBegin(GL_QUADS) ;
			glVertex3i(0, 0, 0) ;
			glVertex3i(0, M_Sides[0]->Get_Height(), 0) ;
			glVertex3i(M_Sides[0]->Get_Width(), M_Sides[0]->Get_Height(), 0) ;
			glVertex3i(M_Sides[0]->Get_Width(), 0, 0) ;
		glEnd() ; // GL_QUADS
	}

	// Go back to where we used to be
	glTranslatef(0, 0, Depth-1) ;
	Undo_View_Side(1) ;
	Set_Front(Old_Front) ;

	// See beginning of this function
	switch(Side) {
	case 1:
		break ;
	case 2:
		glRotatef(-90, 0, -1, 0) ;
		break ;
	case 3:
		glRotatef(-90, -1, 0, 0) ;
		break ;
	case 4:
		glRotatef(-90, 0, 1, 0) ;
		break ;
	case 5:
		glRotatef(-90, 1, 0, 0) ;
		break ;
	case 6:
		glRotatef(-180, 1, 0, 0) ;
		break ;
	default:
		break ;
	}

	glEndList() ; // List_ID

	return List_ID ;
}

// This function draws a single slice of a Cube.  Surprisingly complex to do.
int Cube::Make_Slice_GL_List(int Side, int Depth) {
	static int List_ID=glGenLists(1) ;

	glNewList(List_ID, GL_COMPILE) ;

	// Similar to View_Side(), but only does a rotation, not a translation as well
	switch(Side) {
	case 1:
		break ;
	case 2:
		glRotatef(90, 0, -1, 0) ;
		break ;
	case 3:
		glRotatef(90, -1, 0, 0) ;
		break ;
	case 4:
		glRotatef(90, 0, 1, 0) ;
		break ;
	case 5:
		glRotatef(90, 1, 0, 0) ;
		break ;
	case 6:
		glRotatef(180, 1, 0, 0) ;
		break ;
	default:
		break ;
	}

	// Start drawing from the specified Side
	int Old_Front=Set_Front(Side) ;

	// Since this slice might not be square, we have to adjust for each side
	int Temp_Slice_Length, X_Coord, Y_Coord ;
	for(int n=2; n<=5; n++) {

		// Get ready to draw on a side
		View_Side(n) ;

		if(n==2 || n==4)
			Temp_Slice_Length=M_Sides[0]->Get_Height() ;
		else // n==3 || n==5
			Temp_Slice_Length=M_Sides[0]->Get_Width() ;

		for(int nn=1; nn<=Temp_Slice_Length; nn++) {

			switch(n) {
			case 2:
				X_Coord=M_Sides[1]->Get_Width()-Depth+1 ;
				Y_Coord=nn ;
				break ;
			case 3:
				X_Coord=nn ;
				Y_Coord=Depth ;
				break ;
			case 4:
				X_Coord=Depth ;
				Y_Coord=Temp_Slice_Length-nn+1 ;
				break ;
			case 5:
				X_Coord=nn ;
				Y_Coord=M_Sides[2]->Get_Height()-Depth+1 ;
				break ;
			default:
				break ;
			}

			// Black background squares
			glColor3d(0, 0, 0) ;
			glBegin(GL_QUADS) ;
				glVertex3i(X_Coord-1, Y_Coord-1, 0) ;
				glVertex3i(X_Coord-1, Y_Coord, 0) ;
				glVertex3i(X_Coord, Y_Coord, 0) ;
				glVertex3i(X_Coord, Y_Coord-1, 0) ;
				glVertex3i(X_Coord-1, Y_Coord-1, 0) ;
			glEnd() ; // GL_QUADS

			// Colored squares, raised for visibility
			M_Sides[n-1]->Get_Tile(X_Coord, M_Sides[n-1]->Get_Height()-Y_Coord+1)->Get_Color()->Change_To() ;
			glBegin(GL_QUADS) ;
				glVertex3d(X_Coord-.9, Y_Coord-.9, .03) ;
				glVertex3d(X_Coord-.9, Y_Coord-.1, .03) ;
				glVertex3d(X_Coord-.1, Y_Coord-.1, .03) ;
				glVertex3d(X_Coord-.1, Y_Coord-.9, .03) ;
			glEnd() ; // GL_QUADS
		}

		// Return from the recent rotation/translation
		Undo_View_Side(n) ;
	}

	// Set_Front() was called, this is not the real side
	View_Side(1) ;

	// Move to the correct depth
	glTranslatef(0, 0, -Depth) ;

	// Draw black caps
	glColor3d(0, 0, 0) ;
	glBegin(GL_QUADS) ;
		glVertex3i(0, 0, 0) ;
		glVertex3i(0, M_Sides[0]->Get_Height(), 0) ;
		glVertex3i(M_Sides[0]->Get_Width(), M_Sides[0]->Get_Height(), 0) ;
		glVertex3i(M_Sides[0]->Get_Width(), 0, 0) ;
	glEnd() ; // GL_QUADS

	// If we are at the bottom, draw the appropriate colored squares
	if(Depth==M_Sides[1]->Get_Width() )
		for(int n=1; n<=M_Sides[5]->Get_Height(); n++)
			for(int nn=1; nn<=M_Sides[5]->Get_Width(); nn++) {
				// Yay pretty colored squares (z-offset for viewability
				M_Sides[5]->Get_Tile(nn, n)->Get_Color()->Change_To() ;
				glBegin(GL_QUADS) ;
					glVertex3d(nn-.9, n-.9, -.03) ;
					glVertex3d(nn-.9, n-.1, -.03) ;
					glVertex3d(nn-.1, n-.1, -.03) ;
					glVertex3d(nn-.1, n-.9, -.03) ;
				glEnd() ; // GL_QUADS
			}

	// Go up a bit and draw the other black cap
	glTranslatef(0, 0, 1) ;
	glColor3d(0, 0, 0) ;
	glBegin(GL_QUADS) ;
		glVertex3i(0, 0, 0) ;
		glVertex3i(0, M_Sides[0]->Get_Height(), 0) ;
		glVertex3i(M_Sides[0]->Get_Width(), M_Sides[0]->Get_Height(), 0) ;
		glVertex3i(M_Sides[0]->Get_Width(), 0, 0) ;
	glEnd() ; // GL_QUADS

	// If we are at the top, draw the appropriate colored squares
	if(Depth==1)
		for(int n=1; n<=M_Sides[0]->Get_Height(); n++)
			for(int nn=1; nn<=M_Sides[0]->Get_Width(); nn++) {
				// Change to the right color, draw the square at an offset
				M_Sides[0]->Get_Tile(nn, M_Sides[0]->Get_Height()-n+1)->Get_Color()->Change_To() ;
				glBegin(GL_QUADS) ;
					glVertex3d(nn-.9, n-.9, .03) ;
					glVertex3d(nn-.9, n-.1, .03) ;
					glVertex3d(nn-.1, n-.1, .03) ;
					glVertex3d(nn-.1, n-.9, .03) ;
				glEnd() ; // GL_QUADS
			}

	// Restore the old view
	glTranslatef(0, 0, Depth-1) ;
	Undo_View_Side(1) ;
	Set_Front(Old_Front) ;

	// See the beginning of this function for this explanation
	switch(Side) {
	case 1:
		break ;
	case 2:
		glRotatef(-90, 0, -1, 0) ;
		break ;
	case 3:
		glRotatef(-90, -1, 0, 0) ;
		break ;
	case 4:
		glRotatef(-90, 0, 1, 0) ;
		break ;
	case 5:
		glRotatef(-90, 1, 0, 0) ;
		break ;
	case 6:
		glRotatef(-180, 1, 0, 0) ;
		break ;
	default:
		break ;
	}

	glEndList() ; // List_ID

	return List_ID ;
}

// This function reorients the viewing volume and translates to the lower-left corner of
// the Face viewed.  This is to facilitate ease of drawing.  Call Undo_View_Side() to
// reverse the changes this function makes!
void Cube::View_Side(int Side) {
	// Get some measurements
	int Width, Height, Depth ;
	Width=M_Sides[0]->Get_Width() ;
	Height=M_Sides[0]->Get_Height() ;
	Depth=M_Sides[1]->Get_Width() ;

	switch(Side) {
	case 1:
		glTranslatef(-(float)Width/2, -(float)Height/2, (float)Depth/2) ;
		break ;
	case 2:
		glRotatef(90, 0, -1, 0) ;
		glTranslatef(-(float)Depth/2, -(float)Height/2, (float)Width/2) ;
		break ;
	case 3:
		glRotatef(90, -1, 0, 0) ;
		glTranslatef(-(float)Width/2, -(float)Depth/2, (float)Height/2) ;
		break ;
	case 4:
		glRotatef(90, 0, 1, 0) ;
		glTranslatef(-(float)Depth/2, -(float)Height/2, (float)Width/2) ;
		break ;
	case 5:
		glRotatef(90, 1, 0, 0) ;
		glTranslatef(-(float)Width/2, -(float)Depth/2, (float)Height/2) ;
		break ;
	case 6:
		glRotatef(180, 1, 0, 0) ;
		glTranslatef(-(float)Width/2, -(float)Height/2, (float)Depth/2) ;
		break ;
	default:
		break ;
	}
}

// This function reverses the effects of View_Side().  See that function for
// explanation.
void Cube::Undo_View_Side(int Side) {
	int Width, Height, Depth ;
	Width=M_Sides[0]->Get_Width() ;
	Height=M_Sides[0]->Get_Height() ;
	Depth=M_Sides[1]->Get_Width() ;

	switch(Side) {
	case 1:
		glTranslatef((float)Width/2, (float)Height/2, -(float)Depth/2) ;
		break ;
	case 2:
		glTranslatef((float)Depth/2, (float)Height/2, -(float)Width/2) ;
		glRotatef(-90, 0, -1, 0) ;
		break ;
	case 3:
		glTranslatef((float)Width/2, (float)Depth/2, -(float)Height/2) ;
		glRotatef(-90, -1, 0, 0) ;
		break ;
	case 4:
		glTranslatef((float)Depth/2, (float)Height/2, -(float)Width/2) ;
		glRotatef(-90, 0, 1, 0) ;
		break ;
	case 5:
		glTranslatef((float)Width/2, (float)Depth/2, -(float)Height/2) ;
		glRotatef(-90, 1, 0, 0) ;
		break ;
	case 6:
		glTranslatef((float)Width/2, (float)Height/2, -(float)Depth/2) ;
		glRotatef(-180, 1, 0, 0) ;
		break ;
	default:
		break ;
	}
}

// This function saves a Cube to an open file pointer.  Pass the appropriate
// Color information in case of sides with the same color.
void Cube::Save(Color **Color_List, FILE *File) {
	// Iterate through the Color_List, turning the RGB values into bytes,
	// and then put them in the file
	for(int n=0; n<6; n++) {
		int Red=255.0*Color_List[n]->Get_Red() ;
		int Green=255.0*Color_List[n]->Get_Green() ;
		int Blue=255.0*Color_List[n]->Get_Blue() ;
		fprintf(File, "%c%c%c", Red, Green, Blue) ;
	}

	// Put the dimensions of the Cube into the file as bytes
	fprintf(File, "%c%c%c", M_Sides[0]->Get_Width(), M_Sides[0]->Get_Height(),
			M_Sides[1]->Get_Width() ) ;

	// These insanely-nested for() loops go through the entire Cube, comparing
	// the addresses of the Colors on the Cube and Color_List.  Note that addresses
	// must be compared and not values.  Then print out the byte value of its
	// Color_List index.
	for(int n1=0; n1<6; n1++)
		for(int n2=1; n2<=M_Sides[n1]->Get_Height(); n2++)
			for(int n3=1; n3<=M_Sides[n1]->Get_Width(); n3++)
				for(char n4=0; n4<6; n4++)
					if(M_Sides[n1]->Get_Tile(n3, n2)->Get_Color()==Color_List[(int)n4])
						fprintf(File, "%c", n4) ;
}

// This function loads a Cube from an open file pointer.  It also assigns the
// appropriate Color_List information.  Warning: No exception handling ability yet.
void Cube::Load(Color **Color_List, FILE *File) {
	// The first 18 bytes are color information
	for(int n=0; n<6; n++) {
		char Red=0, Green=0, Blue=0 ;
		fscanf(File, "%c%c%c", &Red, &Green, &Blue) ;
		Color_List[n]->Set(Red/255.0, Green/255.0, Blue/255.0) ;
	}

	// The next three bytes are dimensions
	char Width=1, Height=1, Depth=1 ;
	fscanf(File, "%c%c%c", &Width, &Height, &Depth) ;

	// Make new faces.  Memory is already allocated, so just point things
	// differently.
	Face Temp_Face_1(Width, Height, Color_List[0]) ;
	Face Temp_Face_2(Depth, Height, Color_List[1]) ;
	Face Temp_Face_3(Width, Depth, Color_List[2]) ;
	Face Temp_Face_4(Depth, Height, Color_List[3]) ;
	Face Temp_Face_5(Width, Depth, Color_List[4]) ;
	Face Temp_Face_6(Width, Height, Color_List[5]) ;
	*M_Sides[0]=Temp_Face_1 ;
	*M_Sides[1]=Temp_Face_2 ;
	*M_Sides[2]=Temp_Face_3 ;
	*M_Sides[3]=Temp_Face_4 ;
	*M_Sides[4]=Temp_Face_5 ;
	*M_Sides[5]=Temp_Face_6 ;

	char Color_Index=0 ;
	// Now, obtain the Color_List information stored in the file and put it on the
	// Cube.
	for(int n1=0; n1<6; n1++)
		for(int n2=1; n2<=M_Sides[n1]->Get_Height(); n2++)
			for(int n3=1; n3<=M_Sides[n1]->Get_Width(); n3++) {
				fscanf(File, "%c", &Color_Index) ;
				M_Sides[n1]->Get_Tile(n3, n2)->Set_Color(Color_List[(int)Color_Index]) ;
			}

}


// Mutators
// This function randomly picks a point on the cube and a direction, then
// calls Twist() based on those numbers.  It is possible to specify a number
// of times to twist, default is 100, standard for competition.
void Cube::Randomize(int Twists) {
	Direction Random_Direction ;
	int Random_Side, Random_Row, Random_Column ;
	for(int n=0; n<Twists; n++) {
		Random_Side=Random(6) ;
		Random_Row=Random(M_Sides[Random_Side-1]->Get_Height() ) ;
		Random_Column=Random(M_Sides[Random_Side-1]->Get_Width() ) ;
		Random_Direction=Direction(Random(4) ) ;
		Twist(Random_Side, Random_Column, Random_Row, Random_Direction) ;
	}
}

// This is the big cube-modifying function.  Given a side and a distance down that
// side, rotate that slice of Cubeness.  Returns true if it was possible to
// rotate in that direction.
bool Cube::Rotate_CW(int Side, int Offset) {
	// Reorient the sides of the Cube so we only have to solve this problem once
	int Old_Front=Set_Front(Side) ;

	// These will be used quite a bit
	int Width=M_Sides[0]->Get_Width() ;
	int Height=M_Sides[0]->Get_Height() ;
	int Depth=M_Sides[1]->Get_Width() ;

	if(Offset>Depth || Offset<1)
		throw std::out_of_range("Rotate_CW");

	// Do not rotate on a single-thickness axis
	if(Depth==1) {
		Set_Front(Old_Front) ;
		return false ;
	}

	// This will be the placeholder for information
	Tile *Temp_Tile ;

	// If the slice that is being turned is square....
	if(Width==Height)
		for(int n=0; n<Width; n++) {
			// Here, grab each Tile one at a time from a side on the
			// slice, and put it onto the next side.
			Temp_Tile=M_Sides[4]->Get_Tile(n+1, Offset) ;
			M_Sides[4]->Set_Tile(n+1, Offset, M_Sides[3]->Get_Tile(Offset, Height-n) ) ;
			M_Sides[3]->Set_Tile(Offset, Height-n, M_Sides[2]->Get_Tile(Width-n, Depth-Offset+1) ) ;
			M_Sides[2]->Set_Tile(Width-n, Depth-Offset+1, M_Sides[1]->Get_Tile(Depth-Offset+1, n+1) ) ;
			M_Sides[1]->Set_Tile(Depth-Offset+1, n+1, Temp_Tile) ;
		}
	// This is for a rectangular slice, since a rotation is 180, not 90
	else { // (Width!=Height)
		for(int n=0; n<Width; n++) {
			// First, exchange one side
			Temp_Tile=M_Sides[4]->Get_Tile(n+1, Offset) ;
			M_Sides[4]->Set_Tile(n+1, Offset, M_Sides[2]->Get_Tile(Width-n, Depth-Offset+1) ) ;
			M_Sides[2]->Set_Tile(Width-n, Depth-Offset+1, Temp_Tile) ;
		}

		for(int n=0; n<Height; n++) {
			// Exchange the other side
			Temp_Tile=M_Sides[3]->Get_Tile(Offset, n+1) ;
			M_Sides[3]->Set_Tile(Offset, n+1, M_Sides[1]->Get_Tile(Depth-Offset+1, Height-n) ) ;
			M_Sides[1]->Set_Tile(Depth-Offset+1, Height-n, Temp_Tile) ;
		}
	}

	// If the Cube is rotated at either end, the Face on that end must also be
	// rotated.  Check for this, do it if necessary.
	if(Offset==1)
		M_Sides[0]->Rotate_CW() ;
	if(Offset==Depth)
		M_Sides[5]->Rotate_CCW() ;

	// Change back to where we used to be, and let it be known that the Cube
	// was rotated
	Set_Front(Old_Front) ;
	return true ;
}

bool Cube::Rotate_CCW(int Side, int Offset) {
	// Three rights make a left
	Rotate_CW(Side, Offset) ;
	Rotate_CW(Side, Offset) ;
	// We only need to return one of these, since the Offset is the same for
	// each one
	return Rotate_CW(Side, Offset) ;
}

// This function causes the Cube to be twisted, given a side, coordinate, and direction.
// If the Cube changed in some way, true is returned, otherwise, false.
bool Cube::Twist(int Side, int Column, int Row, Direction direction) {
	// Got to have some place to start and some place to go
	if(!Side || direction==NONE)
		return false ;

	// Make life easier, pretend we are twisting from side 1
	int Old_Front=Set_Front(Side) ;

	// Figure out where we are rotating, what direction, and if the Cube changed
	bool Returner ;
	switch(direction) {
	case Up:
		Returner=Rotate_CW(2, Column) ;
		break ;
	case Down:
		Returner=Rotate_CCW(2, Column) ;
		break ;
	case Left:
		Returner=Rotate_CCW(3, Row) ;
		break ;
	case Right:
		Returner=Rotate_CW(3, Row) ;
		break ;
	default:
		throw std::out_of_range("Twist");
	}

	// Leave the Cube the way we found it
	Set_Front(Old_Front) ;
	return Returner ;
}

// Just an alias for Twist()
bool Cube::Twist(Vector Position, Direction direction) {
	return Twist(Position[1], Position[2], Position[3], direction) ;
}


// Inspectors
// This function returns the address of a side.  Useful for inspecting and mutating.
Face *Cube::Get_Face(int Side) const {
	if(Side>6 || Side<1)
		throw std::out_of_range("Get_Face");
	return M_Sides[Side-1] ;
}

// Is the Cube solved?
bool Cube::Is_Solved(void) const {
	for(int n=0; n<6; n++)
		if(!M_Sides[n]->Is_Solved() )
			return false ;
	return true ;
}

// Private Functions
// The Cube must have some random method for randomizing itself
int Cube::Random(int Max) {
	static bool Seeded=false ;

	// If we haven't called this function before, initialize randomocity
	if(!Seeded) {
		srand(unsigned(time(NULL) ) ) ;
		Seeded=true ;
	}

	// It's not fancy because it doesn't have to be.
	return rand()%Max+1 ;
}

// This lifesaving function modifies the entire Cube so that the Cube now treats
// the passed Side as its new front.  This way, algorithms only have to be
// written for one side of the Cube.  The function returns the location of where
// the old side 1 is.  Always remember to set the Cube back to normal after your
// algorithm.
int Cube::Set_Front(int Side) {
	// We need a temporary Face
	Face *Front_Face=M_Sides[0] ;
	int Returner ;

	// This is the core.  Notice that Spin_*() function must be called on
	// some of the Face to preserve the orientation of the Face with regard
	// to the standard Cube.
	switch(Side) {
	case 1:
		// It's already facing 1.  Duh.
		Returner=1 ;
		break ;
	case 2:
		M_Sides[0]=M_Sides[1] ;
		M_Sides[5]->Spin_CW() ;
		M_Sides[5]->Spin_CW() ;
		M_Sides[1]=M_Sides[5] ;
		M_Sides[3]->Spin_CW() ;
		M_Sides[3]->Spin_CW() ;
		M_Sides[5]=M_Sides[3] ;
		M_Sides[3]=Front_Face ;
		M_Sides[2]->Spin_CCW() ;
		M_Sides[4]->Spin_CW() ;
		Returner=4 ;
		break ;
	case 3:
		M_Sides[0]=M_Sides[2] ;
		M_Sides[2]=M_Sides[5] ;
		M_Sides[5]=M_Sides[4] ;
		M_Sides[4]=Front_Face ;
		M_Sides[1]->Spin_CW() ;
		M_Sides[3]->Spin_CCW() ;
		Returner=5 ;
		break ;
	case 4:
		M_Sides[0]=M_Sides[3] ;
		M_Sides[5]->Spin_CW() ;
		M_Sides[5]->Spin_CW() ;
		M_Sides[3]=M_Sides[5] ;
		M_Sides[1]->Spin_CW() ;
		M_Sides[1]->Spin_CW() ;
		M_Sides[5]=M_Sides[1] ;
		M_Sides[1]=Front_Face ;
		M_Sides[2]->Spin_CW() ;
		M_Sides[4]->Spin_CCW() ;
		Returner=2 ;
		break ;
	case 5:
		M_Sides[0]=M_Sides[4] ;
		M_Sides[4]=M_Sides[5] ;
		M_Sides[5]=M_Sides[2] ;
		M_Sides[2]=Front_Face ;
		M_Sides[1]->Spin_CCW() ;
		M_Sides[3]->Spin_CW() ;
		Returner=3 ;
		break ;
	case 6:
		M_Sides[0]=M_Sides[5] ;
		M_Sides[5]=Front_Face ;
		Front_Face=M_Sides[2] ;
		M_Sides[2]=M_Sides[4] ;
		M_Sides[4]=Front_Face ;
		M_Sides[1]->Spin_CW() ;
		M_Sides[1]->Spin_CW() ;
		M_Sides[3]->Spin_CW() ;
		M_Sides[3]->Spin_CW() ;
		Returner=6 ;
		break ;
	default:
		throw std::out_of_range("SetFront");
	}
	return Returner ;
}
