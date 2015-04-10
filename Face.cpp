
#include "Face.h"

#include <stdexcept>

// The elements of a Face are kept in a dynamic array which is kept in row-major order.

// Constructors
Face::Face(void) {
	M_Tile=new Tile*[9];
	for(auto n=0; n<9; n++) {
		M_Tile[n]=new Tile;
	}
	M_Height=3;
	M_Width=3;
}

Face::Face(int Width, int Height, Color *color) {
	M_Tile=new Tile*[Height*Width];
	for(auto n=0; n<Height*Width; n++) {
		M_Tile[n]=new Tile;
		M_Tile[n]->Set_Color(color);
	}
	M_Height=Height;
	M_Width=Width;
}

Face::Face(const Face &face) {
	M_Height=face.Get_Height();
	M_Width=face.Get_Width();
	M_Tile=new Tile*[M_Width*M_Height];
	for(auto n=0; n<M_Width; n++)
		for(auto nn=0; nn<M_Height; nn++) {
			M_Tile[n+nn*M_Width]=new Tile;
			M_Tile[n+nn*M_Width]->Set_Color(face.Get_Tile(n+1, nn+1)->Get_Color() );
		}
}


// Destructors
Face::~Face(void) {
	delete [] M_Tile;
}


// Facilitators


// Inspectors
int Face::Get_Height(void) const {
	return M_Height;
}

int Face::Get_Width(void) const {
	return M_Width;
}

Tile *Face::Get_Tile(int Column, int Row) const {
	if(Row<1 || Row>M_Height)
		throw std::out_of_range("Get_Tile");
	if(Column<1 || Column>M_Width)
		throw std::out_of_range("Get_Tile");
	return M_Tile[M_Width*(Row-1)+Column-1];
}

bool Face::Is_Solved(void) const {
	// If there is only one cube on a face, it is solved
	if(M_Height*M_Width==1)
		return true;
	// Create a temporary color based on the first color on the face
	Color Temp(M_Tile[0]->Get_Color()->Get_Red(), M_Tile[0]->Get_Color()->Get_Green(),
			M_Tile[0]->Get_Color()->Get_Blue() );
	// Cycle through the face, checking the colors to see if they all match the first
	for(auto n=1; n<M_Width*M_Height; n++)
		if(		M_Tile[n]->Get_Color()->Get_Red()!=Temp.Get_Red() ||
				M_Tile[n]->Get_Color()->Get_Green()!=Temp.Get_Green() ||
				M_Tile[n]->Get_Color()->Get_Blue()!=Temp.Get_Blue() )
			return false;
	return true;
}


// Mutators
// Flood: Make an entire Face a single Color
void Face::Flood(Color *color) {
	for(auto n=0; n<M_Height*M_Width; n++)
		M_Tile[n]->Set_Color(color);
}

void Face::Rotate_CCW() {
	// Three rights make a left
	Rotate_CW();
	Rotate_CW();
	Rotate_CW();
}

void Face::Rotate_CW(void) {
	// Store the old values in a temporary array
	Tile **Temp=new Tile*[M_Height*M_Width];

	// Copy the old values
	for(auto n=0; n<M_Height*M_Width; n++)
		Temp[n]=M_Tile[n];

	if(M_Height==M_Width) {
		for(auto n=0; n<M_Height; n++)
			for(auto nn=0; nn<M_Width; nn++)
				// bijection function that maps the new coordinates
				M_Tile[(M_Width-1-n)+M_Height*nn]=Temp[n*M_Width+nn];
	}
	else { // M_Height!=M_Width
		for(auto n=0; n<M_Height; n++)
			for(auto nn=0; nn<M_Width; nn++)
				// This function actually rotates 180 degrees
				M_Tile[M_Width*M_Height-1-nn-n*M_Width]=Temp[n*M_Width+nn];
	}

	delete [] Temp;
}

void Face::Spin_CW(void) {
	// For squares, just use the Rotate_CW() method
	if(M_Height==M_Width) {
		Rotate_CW();
		return;
	}

	// Create a temporary array of Tile to store the old Face
	Tile **Temp=new Tile*[M_Height*M_Width];

	// Copy the old Face
	for(auto n=0; n<M_Height*M_Width; n++)
		Temp[n]=M_Tile[n];

	// M_Height!=M_Width, so change the two
	auto Temp_Size=M_Height;
	M_Height=M_Width;
	M_Width=Temp_Size;

	for(auto n=0; n<M_Width; n++)
		for(auto nn=0; nn<M_Height; nn++)
			// Use a bijection to map the coordinates
			M_Tile[n+nn*M_Width]=Temp[nn+M_Height*(M_Width-1-n)];

	delete [] Temp;
}

void Face::Spin_CCW(void) {
	if(M_Height==M_Width)
		Rotate_CCW();
	else {
		Rotate_CW();
		Spin_CW();
	}
}

void Face::Set_Tile(int Column, int Row, Tile *tile) {
	if(Row<1 || Row>M_Height)
		throw std::out_of_range("Set_Tile");
	if(Column<1 || Column>M_Width)
		throw std::out_of_range("Set_Tile");

	M_Tile[M_Width*(Row-1)+Column-1]=tile;
}

Face &Face::operator =(const Face &face) {
	M_Height=face.Get_Height();
	M_Width=face.Get_Width();
	// Clear the old array
	delete [] M_Tile;
	// Make a new array
	M_Tile=new Tile*[M_Width*M_Height];

	for(auto n=0; n<M_Width; n++)
		for(auto nn=0; nn<M_Height; nn++) {
			M_Tile[n+nn*M_Width]=new Tile;
			M_Tile[n+nn*M_Width]->Set_Color(face.Get_Tile(n+1, nn+1)->Get_Color() );
		}
	return *this;
}
