
#include "ColorPick.h"

#include "ui.h"

#include "Cube.h"
#include "Output.h"
#include "OptionWindow.h"

bool Is_Color_Window_Open;
int Color_Window_ID;

// This is the address of the old Color
Color Window_Color;

int Color_Window_Size;
bool Color_Off;

void Color_Window(Color Old_Color) {
	// Store the address of the old Color and make a window
	Window_Color=Old_Color;
	Color_Window_ID=glutCreateWindow("Color Picker");

	// Define a reasonable size for the window
	Color_Window_Size=410;
	glutReshapeWindow(Color_Window_Size, Color_Window_Size);
	glutPositionWindow(50, 50);

	// Define some callbacks
	glutDisplayFunc(Color_Display);
	glutMouseFunc(Color_Mouse);
	glutPassiveMotionFunc(NULL);
	glutReshapeFunc(Color_Reshape);

	// Create a viewing matrix in orthographic view
	glViewport(0, 0, Color_Window_Size, Color_Window_Size);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, Color_Window_Size, 0, Color_Window_Size);
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);

	// A reasonable background color
	glClearColor(.6, .6, .6, 0);
}

// The display callback function.  Draws squares of color.
void Color_Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Part is a logical unit of the window's size
	auto Part = Color_Window_Size/33.0;

	// Draw four quadrants in the window
	for(auto n1 = 0; n1 < 4; ++n1) {
		// This switch handles the quadrant translations
		switch(n1) {
		case 0:
			glTranslated(Part, Part, 0);
			break;
		case 1:
			glTranslated(Part*17, Part, 0);
			break;
		case 2:
			glTranslated(Part, 17*Part, 0);
			break;
		case 3:
			glTranslated(17*Part, 17*Part, 0);
			break;
		default:
			break;
		}

		// For each column
		for(auto n2=0; n2<4; n2++) {
			glTranslated(4*n2*Part, 0, 0);

			// For each row
			for(auto n3=0; n3<4; n3++) {
				glTranslated(0, 4*n3*Part, 0);

				// If this is the old color, draw a white box
				// around it to let the user know which one it
				// is
				if(static_cast<int>(Window_Color.red()*3)==n1 &&
						static_cast<int>(Window_Color.green()*3)==n2 &&
						static_cast<int>(Window_Color.blue()*3)==n3) {
					glColor3d(.8, .8, .8);
					glLineWidth(2);
					glBegin(GL_LINE_STRIP); {
						glVertex3d( -Part/2,  -Part/2, 0);
						glVertex3d(7*Part/2,  -Part/2, 0);
						glVertex3d(7*Part/2, 7*Part/2, 0);
						glVertex3d( -Part/2, 7*Part/2, 0);
						glVertex3d( -Part/2,  -Part/2, 0);
					} glEnd();
				}

				// Draw the color, based on where we are
				glColor3d(n1/3.0, n2/3.0, n3/3.0);
				glBegin(GL_QUADS); {
					glVertex3s(0, 0, 0);
					glVertex3s(3*Part, 0, 0);
					glVertex3s(3*Part, 3*Part, 0);
					glVertex3s(0, 3*Part, 0);
				} glEnd();

				// Move back
				glTranslated(0, -4*n3*Part, 0);
			}
			// Move back
			glTranslated(-4*n2*Part, 0, 0);
		}

		// Move back for the last time
		switch(n1) {
		case 0:
			glTranslated(-Part, -Part, 0);
			break;
		case 1:
			glTranslated(-17*Part, -Part, 0);
			break;
		case 2:
			glTranslated(-Part, -17*Part, 0);
			break;
		case 3:
			glTranslated(-17*Part, -17*Part, 0);
			break;
		default:
			break;
		}
	}

	// Draw everything
	glutSwapBuffers();
}

// The mouse callback
void Color_Mouse(int, int State, int X_Coord, int Y_Coord) {
	// Not checking for different buttons.  Why bother?
	if(State==GLUT_DOWN) {
		// Adjust for window coordinates to logical ones
		Y_Coord=Color_Window_Size-Y_Coord;
		auto Part=Color_Window_Size/33.0;
		double Red, Green, Blue;

		// This if-else-else-else block determines the red content
		if(X_Coord>Part && X_Coord<16*Part) {
			if(Y_Coord>Part && Y_Coord<16*Part)
				Red=0;
			else if(Y_Coord>17*Part && Y_Coord<32*Part)
				Red=2/3.0;
			else
				return;
		}
		else if(X_Coord>17*Part && X_Coord<32*Part) {
			if(Y_Coord>Part && Y_Coord<16*Part)
				Red=1/3.0;
			else if(Y_Coord>17*Part && Y_Coord<32*Part)
				Red=1;
			else
				return;
		}
		// If a user did not click in a quadrant, don't do anything
		else
			return;

		// This block determines, by using the X_Coord, the green
		// content of the color clicked upon
		if( (X_Coord>Part && X_Coord<4*Part) ||
			(X_Coord>17*Part && X_Coord<20*Part) )
			Green=0;
		else if( (X_Coord>5*Part && X_Coord<8*Part) ||
			(X_Coord>21*Part && X_Coord<24*Part) )
			Green=1/3.0;
		else if( (X_Coord>9*Part && X_Coord<12*Part) ||
			(X_Coord>25*Part && X_Coord<28*Part) )
			Green=2/3.0;
		else if( (X_Coord>13*Part && X_Coord<16*Part) ||
			(X_Coord>29*Part && X_Coord<32*Part) )
			Green=1;
		// If a user didn't click in a column, don't do anything
		else
			return;

		// Finally, get the blue part by checking rows
		if( (Y_Coord>Part && Y_Coord<4*Part) ||
			(Y_Coord>17*Part && Y_Coord<20*Part) )
			Blue=0;
		else if( (Y_Coord>5*Part && Y_Coord<8*Part) ||
			(Y_Coord>21*Part && Y_Coord<24*Part) )
			Blue=1/3.0;
		else if( (Y_Coord>9*Part && Y_Coord<12*Part) ||
			(Y_Coord>25*Part && Y_Coord<28*Part) )
			Blue=2/3.0;
		else if( (Y_Coord>13*Part && Y_Coord<16*Part) ||
			(Y_Coord>29*Part && Y_Coord<32*Part) )
			Blue=1;
		// Last chance to click somewhere invalid
		else
			return;

		// Change the definition of the old color
		Window_Color.Set(Red, Green, Blue);

		// Prepare to close the window, redraw the option window
		Is_Color_Window_Open=false;
		glutSetWindow(Option_Window_ID);
		Make_All_Option_Lists();
		glutPostRedisplay();

		// Redraw the cube
		glutSetWindow(Window_ID);
		Cube_List=Current_Cube.Make_GL_List();
		glutPostRedisplay();

		// Close the window
		glutDestroyWindow(Color_Window_ID);
	}
}

// This function prevents users from resizing the window into sizes that are
// unrelated to one another, creating distortion.
void Color_Reshape(int X_Size, int Y_Size) {
	// Find the smaller of the two sizes
	double New_Size=X_Size>Y_Size?Y_Size:X_Size;

	// The smaller size defines the new edge of the window
	Color_Window_Size=New_Size;

	// Resize the window
	glutReshapeWindow(Color_Window_Size, Color_Window_Size);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Redefine the viewing matrix
	gluOrtho2D(0, Color_Window_Size, 0, Color_Window_Size);
	glViewport(0, 0, Color_Window_Size, Color_Window_Size);
	glMatrixMode(GL_MODELVIEW);
}
