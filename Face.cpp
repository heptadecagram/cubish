// File: Face.cpp
// Author: Liam Bryan
// Language: C++
// Last Modified: 2002.03.11
//
// The elements of a Face are kept in a dynamic array which is kept in row-major order.

#include "Face.h"

// Constructors
Face::Face(void) {
	M_Tile=new Tile*[9] ;
	if(M_Tile==NULL)
		Die("Unable to allocate memory for new Face()") ;
	for(int n=0; n<9; n++) {
		M_Tile[n]=new Tile ;
		if(M_Tile[n]==NULL)
			Die("Unable to allocate memory for newFace([%d])", n) ;
	}
	M_Height=3 ;
	M_Width=3 ;
}

Face::Face(int Width, int Height, Color *color) {
	M_Tile=new Tile*[Height*Width] ;
	if(M_Tile==NULL)
		Die("Unable to allocate memory for new Face(%d, %d)", Width, Height) ;
	for(int n=0; n<Height*Width; n++) {
		M_Tile[n]=new Tile ;
		if(M_Tile[n]==NULL)
			Die("Unable to allocate memory for new Face([%d])", n) ;
		M_Tile[n]->Set_Color(color) ;
	}
	M_Height=Height ;
	M_Width=Width ;
}

Face::Face(const Face &face) {
	M_Height=face.Get_Height() ;
	M_Width=face.Get_Width() ;
	if(M_Tile==NULL)
		Die("Unable to allocate memory for new &Face(%d, %d)", M_Width, M_Height) ;
	M_Tile=new Tile*[M_Width*M_Height] ;
	for(int n=0; n<M_Width; n++) 
		for(int nn=0; nn<M_Height; nn++) {
			M_Tile[n+nn*M_Width]=new Tile ;
			if(M_Tile[n+nn*M_Width]==NULL)
				Die("Unable to allocate memory for new &Face([%d])", n+nn*M_Width) ;
			M_Tile[n+nn*M_Width]->Set_Color(face.Get_Tile(n+1, nn+1)->Get_Color() ) ;
		}
}


// Destructors
Face::~Face(void) {
	delete [] M_Tile ;
}


// Facilitators


// Inspectors
int Face::Get_Height(void) const {
	return M_Height ;
}

int Face::Get_Width(void) const {
	return M_Width ;
}

Tile *Face::Get_Tile(int Column, int Row) const {
	if(Row<1 || Row>M_Height)
		Die("Get_Tile(%d, %d) Out of Bounds for [%d][%d]\n", 
				Column, Row, M_Width, M_Height) ;
	if(Column<1 || Column>M_Width)
		Die("Get_Tile(%d, %d) Out of Bounds for [%d][%d]\n", 
				Column, Row, M_Width, M_Height) ;
	return M_Tile[M_Width*(Row-1)+Column-1] ;
}

bool Face::Is_Solved(void) const {
	// If there is only one cube on a face, it is solved
	if(M_Height*M_Width==1)
		return true ;
	// Create a temporary color based on the first color on the face
	Color Temp(M_Tile[0]->Get_Color()->Get_Red(), M_Tile[0]->Get_Color()->Get_Green(),
			M_Tile[0]->Get_Color()->Get_Blue() ) ;
	// Cycle through the face, checking the colors to see if they all match the first
	for(int n=1; n<M_Width*M_Height; n++)
		if(		M_Tile[n]->Get_Color()->Get_Red()!=Temp.Get_Red() ||
				M_Tile[n]->Get_Color()->Get_Green()!=Temp.Get_Green() ||
				M_Tile[n]->Get_Color()->Get_Blue()!=Temp.Get_Blue() )
			return false ;
	return true ;
}


// Mutators
// Flood: Make an entire Face a single Color
void Face::Flood(Color *color) {
	for(int n=0; n<M_Height*M_Width; n++)
		M_Tile[n]->Set_Color(color) ;
}

void Face::Rotate_CCW() {
	// Three rights make a left
	Rotate_CW() ;
	Rotate_CW() ;
	Rotate_CW() ;
}

void Face::Rotate_CW(void) {
	// Store the old values in a temporary array
	Tile **Temp=new Tile*[M_Height*M_Width] ;
	if(Temp==NULL)
		Die("Unable to allocate temporary Rotation Face") ;

	// Copy the old values
	for(int n=0; n<M_Height*M_Width; n++)
		Temp[n]=M_Tile[n] ;

	if(M_Height==M_Width) {
		for(int n=0; n<M_Height; n++)
			for(int nn=0; nn<M_Width; nn++)
				// bijection function that maps the new coordinates
				M_Tile[(M_Width-1-n)+M_Height*nn]=Temp[n*M_Width+nn] ;
	}
	else { // M_Height!=M_Width
		for(int n=0; n<M_Height; n++)
			for(int nn=0; nn<M_Width; nn++)
				// This function actually rotates 180 degrees
				M_Tile[M_Width*M_Height-1-nn-n*M_Width]=Temp[n*M_Width+nn] ;
	}

	delete [] Temp ;
}

void Face::Spin_CW(void) {
	// For squares, just use the Rotate_CW() method
	if(M_Height==M_Width) {
		Rotate_CW() ;
		return ;
	}

	// Create a temporary array of Tile to store the old Face
	Tile **Temp=new Tile*[M_Height*M_Width] ;
	if(Temp==NULL)
		Die("Unable to allocate temporary Spin Face") ;
	
	// Copy the old Face
	for(int n=0; n<M_Height*M_Width; n++)
		Temp[n]=M_Tile[n] ;

	// M_Height!=M_Width, so change the two
	int Temp_Size=M_Height ;
	M_Height=M_Width ;
	M_Width=Temp_Size ;

	for(n=0; n<M_Width; n++)
		for(int nn=0; nn<M_Height; nn++)
			// Use a bijection to map the coordinates
			M_Tile[n+nn*M_Width]=Temp[nn+M_Height*(M_Width-1-n)] ;

	delete [] Temp ;
}

void Face::Spin_CCW(void) {
	if(M_Height==M_Width)
		Rotate_CCW() ;
	else {
		Rotate_CW() ;
		Spin_CW() ;
	}
}

void Face::Set_Tile(int Column, int Row, Tile *tile) {
	if(Row<1 || Row>M_Height)
		Die("Set_Tile(%d, %d) Out of Bounds for [%d][%d]\n", 
				Column, Row, M_Width, M_Height) ;
	if(Column<1 || Column>M_Width)
		Die("Set_Tile(%d, %d) Out of Bounds for [%d][%d]\n", 
				Column, Row, M_Width, M_Height) ;

	M_Tile[M_Width*(Row-1)+Column-1]=tile ;
}

Face &Face::operator =(const Face &face) {
	M_Height=face.Get_Height() ;
	M_Width=face.Get_Width() ;
	// Clear the old array
	delete [] M_Tile ;
	// Make a new array
	M_Tile=new Tile*[M_Width*M_Height] ;
	if(M_Tile==NULL)
		Die("Unable to allocate temporary Equality Face") ;
	
	for(int n=0; n<M_Width; n++) 
		for(int nn=0; nn<M_Height; nn++) {
			M_Tile[n+nn*M_Width]=new Tile ;
			if(M_Tile[n+nn*M_Width]==NULL)
				Die("Unable to allocate Tile for Face([%d])", M_Tile[n+nn*M_Width]) ;
			M_Tile[n+nn*M_Width]->Set_Color(face.Get_Tile(n+1, nn+1)->Get_Color() ) ;
		}
	return *this ;
}