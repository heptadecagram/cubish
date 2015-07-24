
#include "Dimension.h"

#include "ui.h"

#include "Cube.h"
#include "Face.h"
#include "Output.h"

// These are defined in Output.cpp and OptionWindow.cpp
extern bool Is_Dimension_Window_Open;
extern int Cube_List;
extern int Window_ID, Option_Window_ID, Dimension_Window_ID;
extern Cube Current_Cube;

// This is the width of the window, height is one-fifth this value
int Dimension_Window_Size;

// These variables keep track of the length of the changing dimension
int Old_Length, Current_Length;
char Dimension_Side;

// A Color_List must be used to resize the Cube
Color_p *Dimension_Color_List;

// Display list identifier
int Length_List;

// Window creation function
void Dimension_Window(int Dimension, Color_p *Color_List) {
	Dimension_Color_List=Color_List;
	// Figure out what dimension is being changed, and get ready to
	// use it when the Cube is resized
	switch(Dimension) {
	case 1:
		Old_Length=Current_Cube[1].length();
		Dimension_Side='w';
		break;
	case 2:
		Old_Length=Current_Cube[1].height();
		Dimension_Side='h';
		break;
	case 3:
		Old_Length=Current_Cube[2].length();
		Dimension_Side='d';
		break;
	default:
		break;
	}
	Current_Length=1;

	Dimension_Window_ID=glutCreateWindow("Size Picker");

	Dimension_Window_Size=400;

	glutReshapeWindow(Dimension_Window_Size, Dimension_Window_Size/5.0);
	glutPositionWindow(250, 300);

	// Declare GLUT callbacks
	glutDisplayFunc(Dimension_Display);
	glutMouseFunc(Dimension_Mouse);
	glutPassiveMotionFunc(Dimension_Passive_Motion);
	glutReshapeFunc(Dimension_Reshape);

	// Set up an orthographic viewing matrix
	glViewport(0, 0, Dimension_Window_Size, Dimension_Window_Size/5.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, Dimension_Window_Size, 0, Dimension_Window_Size/5.0);
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(.6, .6, .6, 0);
}

// Display callback for GLUT
void Dimension_Display() {
	// Clear old things
	glClear(GL_COLOR_BUFFER_BIT);

	// Define Part, a logical unit of window measurement, useful for drawing
	auto Part=Dimension_Window_Size/100.0;

	// Draw a skeletal length of the old size
	glLineWidth(2);
	glColor3d(1, 1, 1);

	glScaled(Part, Part, 0);
	glBegin(GL_LINES); {
		glVertex2s(5, 4);
		glVertex2s(5, 2);
	} glEnd();

	glTranslated(5, 0, 0);
	for(auto n = 0; n < Old_Length; ++n) {
		glBegin(GL_LINE_STRIP); {
			glVertex2s( 0, 2);
			glVertex2s(10, 2);
			glVertex2s(10, 4);
		} glEnd();
		glTranslated(10, 0, 0);
	}
	glTranslated(-5 - 10*Old_Length, 0, 0);

	// Draw squares showing the current possible length
	glTranslated(5, 0, 0);
	for(auto n = 0; n < Current_Length; ++n) {
		glColor3d(.3, .3, .3);
		glBegin(GL_QUADS); {
			glVertex2s( 0,  5);
			glVertex2s(10,  5);
			glVertex2s(10, 15);
			glVertex2s( 0, 15);
		} glEnd();
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP); {
			glVertex2s( 0,  5);
			glVertex2s(10,  5);
			glVertex2s(10, 15);
			glVertex2s( 0, 15);
			glVertex2s( 0,  5);
		} glEnd();
		glTranslated(10, 0, 0);
	}
	glTranslated(-5 - 10*Current_Length, 0, 0);
	glScaled(1/Part, 1/Part, 0);

	// Draw everything
	glutSwapBuffers();
}

void Dimension_Mouse(int, int State, int, int) {
	// When a user clicks (doesn't matter what button)
	if(State==GLUT_DOWN) {

		// This window is about to close
		Is_Dimension_Window_Open=false;

		// If there has been no change to the size, don't change how
		// it is randomized
		if(Old_Length!=Current_Length) {
			// Save the state of the Cube before messing with it
			auto Solved=Current_Cube.Is_Solved();
			for(auto n=1; n<=6; n++)
				Current_Cube[n].Flood(Dimension_Color_List[n-1]);
			auto Width=Current_Cube[1].length();
			auto Height=Current_Cube[1].height();
			auto Depth=Current_Cube[2].length();

			// This next if-else-else block figures out what sides
			// should be modified as the result of changing the
			// dimension chosen.  It looks nasty and bloats the
			// code, but it's fast, and doing it more modularly
			// would actually add more bloat rather than take it
			// away.
			if(Dimension_Side=='w') {
				Current_Cube[1] = Face(Current_Length, Height, Dimension_Color_List[0]);
				Current_Cube[3] = Face(Current_Length, Depth, Dimension_Color_List[2]);
				Current_Cube[5] = Face(Current_Length, Depth, Dimension_Color_List[4]);
				Current_Cube[6] = Face(Current_Length, Height, Dimension_Color_List[5]);
			}
			else if(Dimension_Side=='h') {
				Current_Cube[1] = Face(Width, Current_Length, Dimension_Color_List[0]);
				Current_Cube[2] = Face(Depth, Current_Length, Dimension_Color_List[1]);
				Current_Cube[4] = Face(Depth, Current_Length, Dimension_Color_List[3]);
				Current_Cube[6] = Face(Width, Current_Length, Dimension_Color_List[5]);
			}
			else if(Dimension_Side=='d') {
				Current_Cube[2] = Face(Current_Length, Height, Dimension_Color_List[1]);
				Current_Cube[3] = Face(Width, Current_Length, Dimension_Color_List[2]);
				Current_Cube[4] = Face(Current_Length, Height, Dimension_Color_List[3]);
				Current_Cube[5] = Face(Width, Current_Length, Dimension_Color_List[4]);
			}

			Cube_Remake_View();
			glutSetWindow(Dimension_Window_ID);
			// If the cube was not solved, keep it that way
			if(!Solved)
				Current_Cube.Randomize();
		}

		// Redraw the Cube in its window
		glutSetWindow(Window_ID);
		Cube_List=Current_Cube.Make_GL_List();
		glutPostRedisplay();

		// Remove this window
		glutDestroyWindow(Dimension_Window_ID);
	}
}

// Mouse-movement callback function.  Makes the big squares change in number.
void Dimension_Passive_Motion(int X_Coord, int) {
	// Figure out how many blocks the mouse has traveled in the X-axis
	auto X_Part=Dimension_Window_Size/10.0;
	X_Coord-=X_Part/2;

	// Define a minimum length
	Current_Length=1;
	// Don't go further than 9 total blocks
	for(auto n=1; n<9; n++) {
		// If one hasn't traveled past the X-coordinate, prepare to
		// draw another block
		if(X_Coord>n*X_Part) {
			Current_Length++;
		}
	}

	// Redraw the window
	glutPostRedisplay();
}

// This function prevents users from resizing the window into sizes that are
// unrelated to one another, creating distortion.
void Dimension_Reshape(int X_Size, int Y_Size) {
	// Figure out which is smaller, the (appropriate) height or width
	auto New_Size=X_Size>5*Y_Size?5*Y_Size:X_Size;

	// Change the size
	Dimension_Window_Size=New_Size;
	glutReshapeWindow(Dimension_Window_Size, Dimension_Window_Size/5.0);

	// Remake the view
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, Dimension_Window_Size, 0, Dimension_Window_Size/5.0);
	glViewport(0, 0, Dimension_Window_Size, Dimension_Window_Size/5.0);
	glMatrixMode(GL_MODELVIEW);
}
