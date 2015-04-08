
#include "Dimension.h"

#include <GLUT/glut.h>

#include "Cube.h"
#include "Face.h"
#include "Output.h"

// These are defined in Output.cpp and OptionWindow.cpp
extern bool Is_Dimension_Window_Open ;
extern int Cube_List ;
extern int Window_ID, Option_Window_ID, Dimension_Window_ID ;
extern Cube *Current_Cube ;

// This is the width of the window, height is one-fifth this value
int Dimension_Window_Size ;

// These variables keep track of the length of the changing dimension
int Old_Length, Current_Length ;
char Dimension_Side ;

// A Color_List must be used to resize the Cube
Color **Dimension_Color_List ;

// Display list identifier
int Length_List ;

// Window creation function
void Dimension_Window(int Dimension, Color **Color_List) {
	Dimension_Color_List=Color_List ;
	// Figure out what dimension is being changed, and get ready to
	// use it when the Cube is resized
	switch(Dimension) {
	case 1:
		Old_Length=Current_Cube->Get_Face(1)->Get_Width() ;
		Dimension_Side='w' ;
		break ;
	case 2:
		Old_Length=Current_Cube->Get_Face(1)->Get_Height() ;
		Dimension_Side='h' ;
		break ;
	case 3:
		Old_Length=Current_Cube->Get_Face(2)->Get_Width() ;
		Dimension_Side='d' ;
		break ;
	default:
		break ;
	}
	Current_Length=1 ;

	Dimension_Window_ID=glutCreateWindow("Size Picker") ;

	Dimension_Window_Size=400 ;

	glutReshapeWindow(Dimension_Window_Size, Dimension_Window_Size/5.0) ;
	glutPositionWindow(250, 300) ;

	// Declare GLUT callbacks
	glutDisplayFunc(Dimension_Display) ;
	glutMouseFunc(Dimension_Mouse) ;
	glutPassiveMotionFunc(Dimension_Passive_Motion) ;
	glutReshapeFunc(Dimension_Reshape) ;

	// Set up an orthographic viewing matrix
	glViewport(0, 0, Dimension_Window_Size, Dimension_Window_Size/5.0) ;
	glMatrixMode(GL_PROJECTION) ;
	gluOrtho2D(0, Dimension_Window_Size, 0, Dimension_Window_Size/5.0) ;
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0) ;

	glMatrixMode(GL_MODELVIEW) ;

	glClearColor(.6, .6, .6, 0) ;
}

// Display callback for GLUT
void Dimension_Display(void) {
	// Clear old things
	glClear(GL_COLOR_BUFFER_BIT) ;

	// Define Part, a logical unit of window measurement, useful for drawing
	float Part=Dimension_Window_Size/100.0 ;

	// Draw a skeletal length of the old size
	glLineWidth(2) ;
	glColor3f(1, 1, 1) ;
	glBegin(GL_LINES) ;
		glVertex3f(5*Part, 4*Part, 0) ;
		glVertex3f(5*Part, 2*Part, 0) ;
	glEnd() ; // GL_LINES
	for(int n=1; n<=Old_Length; n++) {
		glBegin(GL_LINE_STRIP) ;
			glVertex3f((5+10*(n-1) )*Part, 2*Part, 0) ;
			glVertex3f((5+10*n)*Part, 2*Part, 0) ;
			glVertex3f((5+10*n)*Part, 4*Part, 0) ;
		glEnd() ; // GL_LINE_STRIP
	}

	// Draw squares showing the current possible length
	for(int nn=0; nn<Current_Length; nn++) {
		glColor3f(.3, .3, .3) ;
		glBegin(GL_QUADS) ;
			glVertex3f((5+10*nn)*Part, 5*Part, 0) ;
			glVertex3f((15+10*nn)*Part, 5*Part, 0) ;
			glVertex3f((15+10*nn)*Part, 15*Part, 0) ;
			glVertex3f((5+10*nn)*Part, 15*Part, 0) ;
		glEnd() ; // GL_QUADS
		glColor3f(0, 0, 0) ;
		glBegin(GL_LINE_STRIP) ;
			glVertex3f((5+10*nn)*Part, 5*Part, 0) ;
			glVertex3f((15+10*nn)*Part, 5*Part, 0) ;
			glVertex3f((15+10*nn)*Part, 15*Part, 0) ;
			glVertex3f((5+10*nn)*Part, 15*Part, 0) ;
			glVertex3f((5+10*nn)*Part, 5*Part, 0) ;
		glEnd() ; // GL_LINE_STRIP
	}

	// Draw everything
	glutSwapBuffers() ;
}

void Dimension_Mouse(int Button, int State, int X_Coord, int Y_Coord) {
	// When a user clicks (doesn't matter what button)
	if(State==GLUT_DOWN) {

		// This window is about to close
		Is_Dimension_Window_Open=false ;

		// If there has been no change to the size, don't change how
		// it is randomized
		if(Old_Length!=Current_Length) {
			// Save the state of the Cube before messing with it
			bool Solved=Current_Cube->Is_Solved() ;
			for(int n=1; n<=6; n++)
				Current_Cube->Get_Face(n)->Flood(Dimension_Color_List[n-1]) ;
			int Width, Height, Depth ;
			Width=Current_Cube->Get_Face(1)->Get_Width() ;
			Height=Current_Cube->Get_Face(1)->Get_Height() ;
			Depth=Current_Cube->Get_Face(2)->Get_Width() ;

			// This next if-else-else block figures out what sides
			// should be modified as the result of changing the
			// dimension chosen.  It looks nasty and bloats the
			// code, but it's fast, and doing it more modularly
			// would actually add more bloat rather than take it
			// away.
			if(Dimension_Side=='w') {
				Face Temp_Face_1(Current_Length, Height, Dimension_Color_List[0]) ;
				*(Current_Cube->Get_Face(1) )=Temp_Face_1 ;
				Face Temp_Face_2(Current_Length, Depth, Dimension_Color_List[2]) ;
				*(Current_Cube->Get_Face(3) )=Temp_Face_2 ;
				Face Temp_Face_3(Current_Length, Depth, Dimension_Color_List[4]) ;
				*(Current_Cube->Get_Face(5) )=Temp_Face_3 ;
				Face Temp_Face_4(Current_Length, Height, Dimension_Color_List[5]) ;
				*(Current_Cube->Get_Face(6) )=Temp_Face_4 ;
			}
			else if(Dimension_Side=='h') {
				Face Temp_Face_1(Width, Current_Length, Dimension_Color_List[0]) ;
				*(Current_Cube->Get_Face(1) )=Temp_Face_1 ;
				Face Temp_Face_2(Depth, Current_Length, Dimension_Color_List[1]) ;
				*(Current_Cube->Get_Face(2) )=Temp_Face_2 ;
				Face Temp_Face_3(Depth, Current_Length, Dimension_Color_List[3]) ;
				*(Current_Cube->Get_Face(4) )=Temp_Face_3 ;
				Face Temp_Face_4(Width, Current_Length, Dimension_Color_List[5]) ;
				*(Current_Cube->Get_Face(6) )=Temp_Face_4 ;
			}
			else if(Dimension_Side=='d') {
				Face Temp_Face_1(Current_Length, Height, Dimension_Color_List[1]) ;
				*(Current_Cube->Get_Face(2) )=Temp_Face_1 ;
				Face Temp_Face_2(Width, Current_Length, Dimension_Color_List[2]) ;
				*(Current_Cube->Get_Face(3) )=Temp_Face_2 ;
				Face Temp_Face_3(Current_Length, Height, Dimension_Color_List[3]) ;
				*(Current_Cube->Get_Face(4) )=Temp_Face_3 ;
				Face Temp_Face_4(Width, Current_Length, Dimension_Color_List[4]) ;
				*(Current_Cube->Get_Face(5) )=Temp_Face_4 ;
			}

			Cube_Remake_View() ;
			glutSetWindow(Dimension_Window_ID) ;
			// If the cube was not solved, keep it that way
			if(!Solved)
				Current_Cube->Randomize() ;
		}

		/*
		glutSetWindow(Option_Window_ID) ;
		Make_All_Option_Lists() ;
		glutPostRedisplay() ;
		*/

		// Redraw the Cube in its window
		glutSetWindow(Window_ID) ;
		Cube_List=Current_Cube->Make_GL_List() ;
		glutPostRedisplay() ;

		// Remove this window
		glutDestroyWindow(Dimension_Window_ID) ;
	}
}

// Mouse-movement callback function.  Makes the big squares change in number.
void Dimension_Passive_Motion(int X_Coord, int Y_Coord) {
	// Figure out how many blocks the mouse has traveled in the X-axis
	float X_Part=Dimension_Window_Size/10.0 ;
	X_Coord-=X_Part/2 ;

	// Define a minimum length
	Current_Length=1 ;
	// Don't go further than 9 total blocks
	for(int n=1; n<9; n++)
		// If one hasn't traveled past the X-coordinate, prepare to
		// draw another block
		if(X_Coord>n*X_Part)
			Current_Length++ ;

	// Redraw the window
	glutPostRedisplay() ;
}

// This function prevents users from resizing the window into sizes that are
// unrelated to one another, creating distortion.
void Dimension_Reshape(int X_Size, int Y_Size) {
	// Figure out which is smaller, the (appropriate) height or width
	int New_Size=X_Size>5*Y_Size?5*Y_Size:X_Size ;

	// Change the size
	Dimension_Window_Size=New_Size ;
	glutReshapeWindow(Dimension_Window_Size, Dimension_Window_Size/5.0) ;

	// Remake the view
	glMatrixMode(GL_PROJECTION) ;
	glPushMatrix() ;
	glLoadIdentity() ;

	gluOrtho2D(0, Dimension_Window_Size, 0, Dimension_Window_Size/5.0) ;
	glViewport(0, 0, Dimension_Window_Size, Dimension_Window_Size/5.0) ;
	glMatrixMode(GL_MODELVIEW) ;
}
