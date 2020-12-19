
#include "OptionWindow.h"

#include <array>
#include <cmath>
#include <fstream>
#include <stdexcept>

#include "ui.h"

#include "Output.h"
#include "ColorPick.h"
#include "Dimension.h"

bool Is_Option_Window_Open;
int Option_Window_ID;

// List of colors
std::array<Color, 6> Option_Color_List;

// These keep track of the mouse location
bool Option_Exit, Option_Random, Option_Solved, Option_Save, Option_Load, Option_Color;
bool Option_Width, Option_Height, Option_Depth;

// Store the window's dimensions
int Option_Window_Width, Option_Window_Height;

// Store the OpenGL lists
int Option_Cube_List, Option_Exit_List, Option_Solved_List, Option_Random_List;
int Option_Line_List, Option_Load_List, Option_Save_List;

void New_Window(std::array<Color, 6> Color_Array) {
	// Get numerically set up for the window
	Option_Color_List=Color_Array;
	Option_Window_ID=glutCreateWindow("Options");

	// Define some reasonable dimensions
	Option_Window_Width=200;
	Option_Window_Height=250;
	glutReshapeWindow(Option_Window_Width, Option_Window_Height);
	glutPositionWindow(150, 150);

	// Set up the callbacks
	glutDisplayFunc(Option_Display);
	glutMouseFunc(Option_Mouse);
	glutPassiveMotionFunc(Option_Passive_Motion);
	glutReshapeFunc(Option_Reshape);
	glutEntryFunc(NULL);
	glutIdleFunc(NULL);

	// Define a viewing volume
	glViewport(0, 0, Option_Window_Width, Option_Window_Height);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-Option_Window_Width/2.0, Option_Window_Width/2.0,
			   -Option_Window_Height/2.0, Option_Window_Height/2.0);
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);

	// Get everything ready to draw
	Make_All_Option_Lists();

	glClearColor(.6, .6, .6, 0);
}

// The display callback for GLUT
void Option_Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Call every single list
	glCallList(Option_Cube_List);

	glCallList(Option_Solved_List);
	glCallList(Option_Random_List);

	glCallList(Option_Exit_List);
	glCallList(Option_Line_List);

	glCallList(Option_Load_List);
	glCallList(Option_Save_List);

	// Draw everything
	glutSwapBuffers();
}

// The mouse callback function for GLUT
void Option_Mouse(int Button, int State, int X_Coord, int Y_Coord) {
	if(Button==GLUT_LEFT_BUTTON && State==GLUT_DOWN) {
		if(Option_Exit) {
			glutDestroyWindow(Option_Window_ID);
			if(Is_Color_Window_Open) {
				glutDestroyWindow(Color_Window_ID);
				Is_Color_Window_Open=false;
			}
			if(Is_Dimension_Window_Open) {
				glutDestroyWindow(Dimension_Window_ID);
				Is_Dimension_Window_Open=false;
			}
			glutSetWindow(Window_ID);
			Is_Option_Window_Open=false;
		}
		else if(Option_Solved && !Current_Cube.solved() ) {
			for(auto n = 0; n < 6; ++n)
				Current_Cube[n].Flood(Option_Color_List[n]);
			Option_Cube_List=Make_Option_Cube_List(Option_Color_List);
			Option_Line_List=Make_Option_Line_List();

			glutSetWindow(Window_ID);
			Cube_List=Current_Cube.Make_GL_List();
			glutPostRedisplay();
		}
		else if(Option_Random) {
			Current_Cube.Randomize();

			Option_Line_List=Make_Option_Line_List();
			glutSetWindow(Window_ID);
			Cube_List=Current_Cube.Make_GL_List();
			glutPostRedisplay();
		}
		else if(Option_Color && !Is_Color_Window_Open) {

			auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;
			int Side = 0;

			if(X_Coord<15*X_Part)
				Side=1;
			else if(X_Coord>25*X_Part)
				Side=3;
			else if(Y_Coord<15*Y_Part)
				Side=2;
			else if(Y_Coord>35*Y_Part)
				Side=5;
			else if(Y_Coord>25*Y_Part)
				Side=4;
			else
				Side=0;

			Color_Window(Option_Color_List[Side]);
			Is_Color_Window_Open=true;
		}
		else if(!Is_Dimension_Window_Open &&
				(Option_Width || Option_Height || Option_Depth) ) {
			auto Dimension=Option_Width ? 0 : Option_Height ? 1 : 2;
			Dimension_Window(Dimension, Option_Color_List);
			Is_Dimension_Window_Open=true;
		}
		else if(Option_Save) {
			std::ofstream Save_File("./CubeSave.1", std::ofstream::binary);
			if(!Save_File.is_open())
				throw std::invalid_argument("Could not open savefile for writing");
			Current_Cube.Save(Option_Color_List, Save_File);
		}
		else if(Option_Load) {
			std::ifstream Save_File("./CubeSave.1", std::ifstream::binary);
			if(!Save_File.is_open())
				throw std::invalid_argument("Could not open savefile for reading");
			Current_Cube.Load(Option_Color_List, Save_File);

			Make_All_Option_Lists();
			glutPostRedisplay();

			Cube_Remake_View();
			glutSetWindow(Window_ID);
			Cube_List=Current_Cube.Make_GL_List();
			glutPostRedisplay();
		}
	}
}

// The regualr mouse movement callback for GLUT
void Option_Passive_Motion(int X_Coord, int Y_Coord) {

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	if(
			(15*X_Part < X_Coord && X_Coord < 25*X_Part &&
				5*Y_Part < Y_Coord && Y_Coord < 45*Y_Part) ||
			( 5*X_Part < X_Coord && X_Coord < 35*X_Part &&
			 15*Y_Part < Y_Coord && Y_Coord < 25*Y_Part)
			) {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		Option_Color=true;
		Option_Width=false;
		Option_Height=false;
		Option_Depth=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(
			15*X_Part < X_Coord && X_Coord < 25*X_Part &&
			 2*Y_Part < Y_Coord && Y_Coord <  4*Y_Part) {
		glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
		Option_Width=true;
		Option_Color=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(
			 2*X_Part < X_Coord && X_Coord <  4*X_Part &&
			15*Y_Part < Y_Coord && Y_Coord < 25*Y_Part) {
		glutSetCursor(GLUT_CURSOR_UP_DOWN);
		Option_Height=true;
		Option_Color=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(
			12*X_Part < X_Coord && X_Coord < 14*X_Part &&
			25*Y_Part < Y_Coord && Y_Coord < 35*Y_Part) {
		glutSetCursor(GLUT_CURSOR_UP_DOWN);
		Option_Depth=true;
		Option_Color=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(Option_Color || Option_Width || Option_Height || Option_Depth) {
		glutSetCursor(GLUT_CURSOR_INHERIT);
		Option_Color=false;
		Option_Width=false;
		Option_Height=false;
		Option_Depth=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}

	if(X_Part < X_Coord && X_Coord < 9*X_Part &&
			Y_Part < Option_Window_Height-Y_Coord && Option_Window_Height-Y_Coord < 9*Y_Part) {
		Option_Exit=true;
		Option_Exit_List=Make_Option_Exit_List();
		glutPostRedisplay();
	}
	else if(Option_Exit) {
		Option_Exit=false;
		Option_Exit_List=Make_Option_Exit_List();
		glutPostRedisplay();
	}

	if(
			X_Part < X_Coord && X_Coord < 9*X_Part &&
			Y_Part < Y_Coord && Y_Coord < 9*Y_Part) {
		Option_Solved=true;
		Option_Solved_List=Make_Option_Solved_List();
		glutPostRedisplay();
	}
	else if(Option_Solved) {
		Option_Solved=false;
		Option_Solved_List=Make_Option_Solved_List();
		glutPostRedisplay();
	}

	if(X_Part < Option_Window_Width-X_Coord && Option_Window_Width-X_Coord < 9*X_Part &&
			Y_Part < Y_Coord && Y_Coord<9*Y_Part) {
		Option_Random=true;
		Option_Random_List=Make_Option_Random_List();
		glutPostRedisplay();
	}
	else if(Option_Random) {
		Option_Random=false;
		Option_Random_List=Make_Option_Random_List();
		glutPostRedisplay();
	}

	if(
			30*X_Part < X_Coord && X_Coord < 40*X_Part &&
			30*Y_Part < Y_Coord && Y_Coord < 40*Y_Part) {
		Option_Save=true;
		Option_Save_List=Make_Option_Save_List();
		glutPostRedisplay();
	}
	else if(Option_Save) {
		Option_Save=false;
		Option_Save_List=Make_Option_Save_List();
		glutPostRedisplay();
	}

	if(
			30*X_Part < X_Coord && X_Coord < 40*X_Part &&
			40*Y_Part < Y_Coord && Y_Coord < 50*Y_Part) {
		Option_Load=true;
		Option_Load_List=Make_Option_Load_List();
		glutPostRedisplay();
	}
	else if(Option_Load) {
		Option_Load=false;
		Option_Load_List=Make_Option_Load_List();
		glutPostRedisplay();
	}
}

// Allowing for users that try to create odd-sized windows
void Option_Reshape(int X_Size, int Y_Size) {
	auto New_Size = 5*X_Size > 4*Y_Size ? Y_Size/5.0 : X_Size/4.0;

	Option_Window_Height=New_Size*5;
	Option_Window_Width=New_Size*4;

	glutReshapeWindow(Option_Window_Width, Option_Window_Height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(
			-Option_Window_Width/2.0, Option_Window_Width/2.0,
			-Option_Window_Height/2.0, Option_Window_Height/2.0);

	glViewport(0, 0, Option_Window_Width, Option_Window_Height);

	glMatrixMode(GL_MODELVIEW);
	Make_All_Option_Lists();
}

int Make_Option_Cube_List(std::array<Color, 6> Color_Array) {
	auto List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE);

	auto w = Option_Window_Width/8.0;
	auto h = Option_Window_Height/5.0;
	for(auto n=0; n<6; n++) {
		std::array<double, 2> location{{0,0}};
		switch(n) {
			case 0:
				location[0]=-w;
				location[1]=0;
				break;
			case 1:
				location[0]=-3*w;
				location[1]=0;
				break;
			case 2:
				location[0]=-w;
				location[1]=h;
				break;
			case 3:
				location[0]=w;
				location[1]=0;
				break;
			case 4:
				location[0]=-w;
				location[1]=-h;
				break;
			case 5:
				location[0]=-w;
				location[1]=-2*h;
				break;
			default:
				break;
		}
		glTranslated(location[0], location[1], 0);

		auto color = Color_Array[n];
		glColor3d(color.red(), color.green(), color.blue());
		glBegin(GL_QUADS); {
			glVertex3d(0, 0, 0);
			glVertex3d(0, Option_Window_Height/5.0, 0);
			glVertex3d(Option_Window_Width/4.0, Option_Window_Height/5.0, 0);
			glVertex3d(Option_Window_Width/4.0, 0, 0);
		} glEnd();

		glTranslated(-location[0], -location[1], 0);
	}

	glEndList();

	return List_ID;
}

int Make_Option_Exit_List() {
	static int List_ID = glGenLists(1);

	auto X_Part = Option_Window_Width/40.0, Y_Part = Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glTranslated(-20*X_Part, -25*Y_Part, 0);
		glScaled(X_Part, Y_Part, 0);

		{
			// Draw the box
			glColor3d(.7, .7, .7);
			glBegin(GL_QUADS); {
				glVertex2s(1, 1);
				glVertex2s(9, 1);
				glVertex2s(9, 9);
				glVertex2s(1, 9);
			} glEnd();

			// Colorize the X if it is mouse-overed
			if(Option_Exit) {
				glColor3d(1, 0, 0);
			}
			else {
				glColor3d(.6, .6, .6);
			}

			// Draw the X
			glLineWidth(5);
			glBegin(GL_LINES); {
				glVertex2s(1, 1);
				glVertex2s(9, 9);
				glVertex2s(9, 1);
				glVertex2s(1, 9);
			} glEnd();

		}

		glScaled(1/X_Part, 1/Y_Part, 0);
		glTranslated(20*X_Part, 25*Y_Part, 0);
	} glEndList();

	return List_ID;
}

int Make_Option_Solved_List() {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glTranslated(-20*X_Part, 15*Y_Part, 0);
		glScaled(X_Part, Y_Part, 0);

		// Draw the solved cube
		glBegin(GL_QUADS); {
			auto color = Option_Color_List[0];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(1, 1);
			glVertex2s(7, 1);
			glVertex2s(7, 7);
			glVertex2s(1, 7);

			color = Option_Color_List[2];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(1, 7);
			glVertex2s(3, 9);
			glVertex2s(9, 9);
			glVertex2s(7, 7);

			color = Option_Color_List[3];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(7, 1);
			glVertex2s(7, 7);
			glVertex2s(9, 9);
			glVertex2s(9, 3);
		} glEnd();

		// Draw the outline if it is mouse-overed
		if(Option_Solved) {
			glLineWidth(2);
			glColor3d(0, 0, 0);
			glBegin(GL_LINE_STRIP); {
				glVertex2s(7, 1);
				glVertex2s(1, 1);
				glVertex2s(1, 7);
				glVertex2s(7, 7);
				glVertex2s(7, 1);

				glVertex2s(9, 3);
				glVertex2s(9, 9);
				glVertex2s(7, 7);
				glVertex2s(9, 9);

				glVertex2s(3, 9);
				glVertex2s(1, 7);
			} glEnd();
		}

		glScaled(1/X_Part, 1/Y_Part, 0);
		glTranslated(20*X_Part, -15*Y_Part, 0);
	} glEndList();

	return List_ID;
}

int Make_Option_Random_List() {
	static int List_ID = glGenLists(1);

	auto X_Part = Option_Window_Width/40.0, Y_Part = Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glTranslated(10*X_Part, 15*Y_Part, 0);
		glScaled(X_Part, Y_Part, 0);

		glBegin(GL_QUADS); {
			auto color = Option_Color_List[0];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(1, 1);
			glVertex2s(4, 1);
			glVertex2s(4, 4);
			glVertex2s(1, 4);

			color = Option_Color_List[1];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(7, 1);
			glVertex2s(4, 1);
			glVertex2s(4, 4);
			glVertex2s(7, 4);

			color = Option_Color_List[2];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(1, 7);
			glVertex2s(4, 7);
			glVertex2s(4, 4);
			glVertex2s(1, 4);

			color = Option_Color_List[3];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(7, 7);
			glVertex2s(4, 7);
			glVertex2s(4, 4);
			glVertex2s(7, 4);

			color = Option_Color_List[4];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(1, 7);
			glVertex2s(2, 8);
			glVertex2s(5, 8);
			glVertex2s(4, 7);

			color = Option_Color_List[5];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(2, 8);
			glVertex2s(3, 9);
			glVertex2s(6, 9);
			glVertex2s(5, 8);

			color = Option_Color_List[0];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(4, 7);
			glVertex2s(5, 8);
			glVertex2s(8, 8);
			glVertex2s(7, 7);

			color = Option_Color_List[1];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(5, 8);
			glVertex2s(6, 9);
			glVertex2s(9, 9);
			glVertex2s(8, 8);

			color = Option_Color_List[2];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(7, 7);
			glVertex2s(8, 8);
			glVertex2s(8, 5);
			glVertex2s(7, 4);

			color = Option_Color_List[3];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(8, 8);
			glVertex2s(9, 9);
			glVertex2s(9, 6);
			glVertex2s(8, 5);

			color = Option_Color_List[4];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(7, 4);
			glVertex2s(8, 5);
			glVertex2s(8, 2);
			glVertex2s(7, 1);

			color = Option_Color_List[5];
			glColor3d(color.red(), color.green(), color.blue());
			glVertex2s(8, 5);
			glVertex2s(9, 6);
			glVertex2s(9, 3);
			glVertex2s(8, 2);

		} glEnd();

		if(Option_Random) {
			glLineWidth(2);
			glColor3d(0, 0, 0);
			glBegin(GL_LINE_STRIP); {
				glVertex2s(7, 1);
				glVertex2s(7, 7);
				glVertex2s(1, 7);
				glVertex2s(1, 1);
				glVertex2s(7, 1);

				glVertex2s(9, 3);
				glVertex2s(9, 9);
				glVertex2s(7, 7);
				glVertex2s(1, 7);
				glVertex2s(3, 9);
				glVertex2s(9, 9);
			} glEnd();

			glBegin(GL_LINE_STRIP); {
				glVertex2s(1, 4);
				glVertex2s(7, 4);
				glVertex2s(9, 6);
			} glEnd();

			glBegin(GL_LINE_STRIP); {
				glVertex2s(2, 8);
				glVertex2s(8, 8);
				glVertex2s(8, 2);
			} glEnd();

			glBegin(GL_LINE_STRIP); {
				glVertex2s(4, 1);
				glVertex2s(4, 7);
				glVertex2s(6, 9);
			} glEnd();
		}

		glScaled(1/X_Part, 1/Y_Part, 0);
		glTranslated(-10*X_Part, -15*Y_Part, 0);
	} glEndList();

	return List_ID;
}

int Make_Option_Line_List() {
	static int List_ID = glGenLists(1);

	auto X_Part = Option_Window_Width/40.0, Y_Part = Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glScaled(X_Part, Y_Part, 0);

		glLineWidth(3);
		if(Option_Width) {
			glColor3d(1, 0, 0);
		}
		else {
			glColor3d(0, 0, 0);
		}

		glBegin(GL_LINE_STRIP); {
			glVertex2s(-4, 23);
			glVertex2s(-5, 22);
			glVertex2s(-4, 21);
			glVertex2s(-5, 22);
			glVertex2s( 5, 22);
			glVertex2s( 4, 23);
			glVertex2s( 5, 22);
			glVertex2s( 4, 21);
		} glEnd();

		if(Option_Height) {
			glColor3d(1, 0, 0);
		}
		else {
			glColor3d(0, 0, 0);
		}

		glBegin(GL_LINE_STRIP); {
			glVertex2s(-18,  9);
			glVertex2s(-17, 10);
			glVertex2s(-16,  9);
			glVertex2s(-17, 10);
			glVertex2s(-17,  0);
			glVertex2s(-18,  1);
			glVertex2s(-17,  0);
			glVertex2s(-16,  1);
		} glEnd();

		if(Option_Depth) {
			glColor3d(1, 0, 0);
		}
		else {
			glColor3d(0, 0, 0);
		}

		glBegin(GL_LINE_STRIP); {
			glVertex2s(-8, -1);
			glVertex2s(-7,  0);
			glVertex2s(-6, -1);
			glVertex2s(-7,  0);
			glVertex2s(-7, -10);
			glVertex2s(-8, -9);
			glVertex2s(-7, -10);
			glVertex2s(-6, -9);
		} glEnd();

		glScaled(1/X_Part, 1/Y_Part, 0);
	} glEndList();

	return List_ID;
}

void disk_image(bool outline) {
	glColor3d(0, 0, 1);
	glBegin(GL_QUADS); {
		glVertex2s(3, 2);
		glVertex2s(3, 8);
		glVertex2s(9, 8);
		glVertex2s(9, 2);
	} glEnd();

	// The little circle of the disk
	glColor3d(.7, .7, .7);
	glBegin(GL_POLYGON); {
		for(auto f1 = 5.0; f1 <= 7; f1 += .1)
			glVertex2d(f1, (5 + sqrt(1 - (f1-6)*(f1-6) ) ));
		for(auto f2 = 7.0; f2 >= 5; f2 -= .1)
			glVertex2d(f2, (5 - sqrt(1 - (f2-6)*(f2-6) ) ));
	} glEnd();

	glBegin(GL_TRIANGLES); {
		glVertex2s(6, 2);
		glVertex2s(6, 5);
		glVertex2s(8, 2);
	} glEnd();

	if(outline) {
		glLineWidth(2);
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP); {
			glVertex2s(3, 2);
			glVertex2s(3, 8);
			glVertex2s(9, 8);
			glVertex2s(9, 2);
			glVertex2s(3, 2);
		} glEnd();
		// The little circle of the disk
		glBegin(GL_LINE_STRIP); {
			for(auto f1 = 5.0; f1 <= 7; f1 += .1)
				glVertex2d(f1, (5 + sqrt(1 - (f1-6)*(f1-6) ) ));
			for(auto f2 = 7.0; f2 >= 5; f2 -= .1)
				glVertex2d(f2, (5 - sqrt(1 - (f2-6)*(f2-6) ) ));
		} glEnd();
	}
}


int Make_Option_Load_List() {
	static int List_ID = glGenLists(1);

	auto X_Part = Option_Window_Width/40.0, Y_Part = Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glTranslated(10*X_Part, -25*Y_Part, 0);
		glScaled(X_Part, Y_Part, 0);

		disk_image(Option_Load);

		if(Option_Load) {
			glLineWidth(2);

			glColor3d(1, 0, 0);
			glBegin(GL_QUADS); {
				glVertex2s(3, 4);
				glVertex2s(3, 6);
				glVertex2s(5, 6);
				glVertex2s(5, 4);
			} glEnd();
			glBegin(GL_TRIANGLES); {
				glVertex2s(0, 5);
				glVertex2s(3, 7);
				glVertex2s(3, 3);
			} glEnd();
		}

		glScaled(1/X_Part, 1/Y_Part, 0);
		glTranslated(-10*X_Part, 25*Y_Part, 0);
	} glEndList();

	return List_ID;
}

int Make_Option_Save_List() {
	static int List_ID = glGenLists(1);

	auto X_Part = Option_Window_Width/40.0, Y_Part = Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE); {
		glTranslated(10*X_Part, -15*Y_Part, 0);
		glScaled(X_Part, Y_Part, 0);

		disk_image(Option_Save);

		if(Option_Save) {
			glLineWidth(2);

			glColor3d(1, 0, 0);
			glBegin(GL_QUADS); {
				glVertex2s(0, 6);
				glVertex2s(2, 6);
				glVertex2s(2, 4);
				glVertex2s(0, 4);
			} glEnd();
			glBegin(GL_TRIANGLES); {
				glVertex2s(2, 7);
				glVertex2s(5, 5);
				glVertex2s(2, 3);
			} glEnd();
		}

		glScaled(1/X_Part, 1/Y_Part, 0);
		glTranslated(-10*X_Part, 15*Y_Part, 0);
	} glEndList();

	return List_ID;
}

void Make_All_Option_Lists() {
	Option_Cube_List=Make_Option_Cube_List(Option_Color_List);
	Option_Exit_List=Make_Option_Exit_List();
	Option_Solved_List=Make_Option_Solved_List();
	Option_Random_List=Make_Option_Random_List();
	Option_Line_List=Make_Option_Line_List();
	Option_Load_List=Make_Option_Load_List();
	Option_Save_List=Make_Option_Save_List();
}
