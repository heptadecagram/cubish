
#include "OptionWindow.h"

#include <cmath>
#include <fstream>
#include <stdexcept>

#include <GLUT/glut.h>

#include "Output.h"
#include "ColorPick.h"
#include "Dimension.h"

// Defined in Output.cpp
extern int Window_ID, Option_Window_ID;
extern int Cube_List;
extern bool Is_Option_Window_Open, Draw_Section;
extern Cube Current_Cube;

// These values keep tract of subwindows of this window
bool Is_Color_Window_Open, Is_Dimension_Window_Open;
int Color_Window_ID, Dimension_Window_ID;

// List of colors
Color **Option_Color_List;

// These keep track of the mouse location
bool Option_Exit, Option_Random, Option_Solved, Option_Save, Option_Load, Option_Color;
bool Option_Width, Option_Height, Option_Depth;

// Store the window's dimensions
int Option_Window_Width, Option_Window_Height;

// Store the OpenGL lists
int Option_Cube_List, Option_Exit_List, Option_Solved_List, Option_Random_List;
int Option_Line_List, Option_Load_List, Option_Save_List;

void New_Window(Color **Color_Array) {
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
void Option_Display(void) {
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
		else if(Option_Solved && !Current_Cube.Is_Solved() ) {
			for(auto n=1; n<=6; n++)
				Current_Cube[n].Flood(Option_Color_List[n-1]);
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
			int Side;

			if(X_Coord<15*X_Part)
				Side=2;
			else if(X_Coord>25*X_Part)
				Side=4;
			else if(Y_Coord<15*Y_Part)
				Side=3;
			else if(Y_Coord>35*Y_Part)
				Side=6;
			else if(Y_Coord>25*Y_Part)
				Side=5;
			else
				Side=1;

			Color_Window(Option_Color_List[Side-1]);
			Is_Color_Window_Open=true;
		}
		else if(!Is_Dimension_Window_Open &&
				(Option_Width || Option_Height || Option_Depth) ) {
			auto Dimension=Option_Width?1:Option_Height?2:3;
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
			std::ifstream Save_File("./CubeSave.1", std::ofstream::binary);
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

	if((X_Coord>15*X_Part && X_Coord<25*X_Part && Y_Coord>5*Y_Part && Y_Coord<45*Y_Part) ||
	   (X_Coord>5*X_Part && X_Coord<35*X_Part && Y_Coord>15*Y_Part && Y_Coord<25*Y_Part) ) {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		Option_Color=true;
		Option_Width=false;
		Option_Height=false;
		Option_Depth=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(X_Coord>15*X_Part && X_Coord<25*X_Part &&
			Y_Coord>2*Y_Part && Y_Coord<4*Y_Part) {
		glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
		Option_Width=true;
		Option_Color=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(X_Coord>2*X_Part && X_Coord<4*X_Part &&
			Y_Coord>15*Y_Part && Y_Coord<25*Y_Part) {
		glutSetCursor(GLUT_CURSOR_UP_DOWN);
		Option_Height=true;
		Option_Color=false;
		Option_Line_List=Make_Option_Line_List();
		glutPostRedisplay();
	}
	else if(X_Coord>12*X_Part && X_Coord<14*X_Part &&
			Y_Coord>25*Y_Part && Y_Coord<35*Y_Part) {
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

	if(X_Coord>X_Part && X_Coord<9*X_Part &&
			Option_Window_Height-Y_Coord>Y_Part &&
			Option_Window_Height-Y_Coord<9*Y_Part) {
		Option_Exit=true;
		Option_Exit_List=Make_Option_Exit_List();
		glutPostRedisplay();
	}
	else
		if(Option_Exit) {
			Option_Exit=false;
			Option_Exit_List=Make_Option_Exit_List();
			glutPostRedisplay();
		}

	if(X_Coord>X_Part && X_Coord<9*X_Part &&
			Y_Coord>Y_Part && Y_Coord<9*Y_Part) {
		Option_Solved=true;
		Option_Solved_List=Make_Option_Solved_List();
		glutPostRedisplay();
	}
	else
		if(Option_Solved) {
			Option_Solved=false;
			Option_Solved_List=Make_Option_Solved_List();
			glutPostRedisplay();
		}

	if(Option_Window_Width-X_Coord>X_Part && Option_Window_Width-X_Coord<9*X_Part &&
			Y_Coord>Y_Part && Y_Coord<9*Y_Part) {
		Option_Random=true;
		Option_Random_List=Make_Option_Random_List();
		glutPostRedisplay();
	}
	else
		if(Option_Random) {
			Option_Random=false;
			Option_Random_List=Make_Option_Random_List();
			glutPostRedisplay();
		}
	if(X_Coord>30*X_Part && X_Coord<40*X_Part && Y_Coord>30*Y_Part && Y_Coord<40*Y_Part) {
		Option_Save=true;
		Option_Save_List=Make_Option_Save_List();
		glutPostRedisplay();
	}
	else
		if(Option_Save) {
			Option_Save=false;
			Option_Save_List=Make_Option_Save_List();
			glutPostRedisplay();
		}

	if(X_Coord>30*X_Part && X_Coord<40*X_Part && Y_Coord>40*Y_Part && Y_Coord<50*Y_Part) {
		Option_Load=true;
		Option_Load_List=Make_Option_Load_List();
		glutPostRedisplay();
	}
	else
		if(Option_Load) {
			Option_Load=false;
			Option_Load_List=Make_Option_Load_List();
			glutPostRedisplay();
		}
}

// Allowing for users that try to create odd-sized windows
void Option_Reshape(int X_Size, int Y_Size) {
	auto New_Size=X_Size*5>Y_Size*4?Y_Size/5.0:X_Size/4.0;

	Option_Window_Height=New_Size*5;
	Option_Window_Width=New_Size*4;

	glutReshapeWindow(Option_Window_Width, Option_Window_Height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(-Option_Window_Width/2.0, Option_Window_Width/2.0,
			-Option_Window_Height/2.0, Option_Window_Height/2.0);

	glViewport(0, 0, Option_Window_Width, Option_Window_Height);

	glMatrixMode(GL_MODELVIEW);
	Make_All_Option_Lists();
}

int Make_Option_Cube_List(Color **Color_Array) {
	auto List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE);

	for(auto n=0; n<6; n++) {
		Vector Location;
		switch(n+1) {
		case 1:
			Location[1]=-Option_Window_Width/8.0;
			Location[2]=0;
			break;
		case 2:
			Location[1]=-3*Option_Window_Width/8.0;
			Location[2]=0;
			break;
		case 3:
			Location[1]=-Option_Window_Width/8.0;
			Location[2]=Option_Window_Height/5.0;
			break;
		case 4:
			Location[1]=Option_Window_Width/8.0;
			Location[2]=0;
			break;
		case 5:
			Location[1]=-Option_Window_Width/8.0;
			Location[2]=-Option_Window_Height/5.0;
			break;
		case 6:
			Location[1]=-Option_Window_Width/8.0;
			Location[2]=-2*Option_Window_Height/5.0;
			break;
		default:
			break;
		}
		glTranslated(Location[1], Location[2], 0);
		Color_Array[n]->Change_To();
		glBegin(GL_QUADS);
			glVertex3d(0, 0, 0);
			glVertex3d(0, Option_Window_Height/5.0, 0);
			glVertex3d(Option_Window_Width/4.0, Option_Window_Height/5.0, 0);
			glVertex3d(Option_Window_Width/4.0, 0, 0);
		glEnd(); // GL_QUADS
		glTranslated(-Location[1], -Location[2], 0);

	}

	glEndList();

	return List_ID;
}

int Make_Option_Exit_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glTranslated(-20*X_Part, -25*Y_Part, 0);
	glColor3d(.7, .7, .7);
	glBegin(GL_QUADS);
		glVertex3d(X_Part, Y_Part, 0);
		glVertex3d(9*X_Part, Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(X_Part, 9*Y_Part, 0);
	glEnd(); // GL_QUADS

	if(Option_Exit)
		glColor3d(1, 0, 0);
	else
		glColor3d(.6, .6, .6);
	glLineWidth(5);
	glBegin(GL_LINES);
		glVertex3d(X_Part, Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(9*X_Part, Y_Part, 0);
		glVertex3d(X_Part, 9*Y_Part, 0);
	glEnd(); // GL_LINES
	glTranslated(20*X_Part, 25*Y_Part, 0);

	glEndList(); // List_ID

	return List_ID;
}

int Make_Option_Solved_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glTranslated(-20*X_Part, 15*Y_Part, 0);
	glBegin(GL_QUADS);
		Option_Color_List[0]->Change_To();
		glVertex3d(X_Part, Y_Part, 0);
		glVertex3d(7*X_Part, Y_Part, 0);
		glVertex3d(7*X_Part, 7*Y_Part, 0);
		glVertex3d(X_Part, 7*Y_Part, 0);

		Option_Color_List[2]->Change_To();
		glVertex3d(X_Part, 7*Y_Part, 0);
		glVertex3d(3*X_Part, 9*Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(7*X_Part, 7*Y_Part, 0);

		Option_Color_List[3]->Change_To();
		glVertex3d(7*X_Part, Y_Part, 0);
		glVertex3d(7*X_Part, 7*Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(9*X_Part, 3*Y_Part, 0);
	glEnd(); // GL_QUADS

	if(Option_Solved) {
		glLineWidth(2);
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3d(7*X_Part, Y_Part, 0);
			glVertex3d(X_Part, Y_Part, 0);
			glVertex3d(X_Part, 7*Y_Part, 0);
			glVertex3d(7*X_Part, 7*Y_Part, 0);
			glVertex3d(7*X_Part, Y_Part, 0);

			glVertex3d(9*X_Part, 3*Y_Part, 0);
			glVertex3d(9*X_Part, 9*Y_Part, 0);
			glVertex3d(7*X_Part, 7*Y_Part, 0);
			glVertex3d(9*X_Part, 9*Y_Part, 0);

			glVertex3d(3*X_Part, 9*Y_Part, 0);
			glVertex3d(X_Part, 7*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP
	}

	glTranslated(20*X_Part, -15*Y_Part, 0);

	glEndList(); // List_ID

	return List_ID;
}

int Make_Option_Random_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glTranslated(10*X_Part, 15*Y_Part, 0);
	glBegin(GL_QUADS);
		Option_Color_List[0]->Change_To();
		glVertex3d(X_Part, Y_Part, 0);
		glVertex3d(4*X_Part, Y_Part, 0);
		glVertex3d(4*X_Part, 4*Y_Part, 0);
		glVertex3d(X_Part, 4*Y_Part, 0);

		Option_Color_List[1]->Change_To();
		glVertex3d(7*X_Part, Y_Part, 0);
		glVertex3d(4*X_Part, Y_Part, 0);
		glVertex3d(4*X_Part, 4*Y_Part, 0);
		glVertex3d(7*X_Part, 4*Y_Part, 0);

		Option_Color_List[2]->Change_To();
		glVertex3d(X_Part, 7*Y_Part, 0);
		glVertex3d(4*X_Part, 7*Y_Part, 0);
		glVertex3d(4*X_Part, 4*Y_Part, 0);
		glVertex3d(X_Part, 4*Y_Part, 0);

		Option_Color_List[3]->Change_To();
		glVertex3d(7*X_Part, 7*Y_Part, 0);
		glVertex3d(4*X_Part, 7*Y_Part, 0);
		glVertex3d(4*X_Part, 4*Y_Part, 0);
		glVertex3d(7*X_Part, 4*Y_Part, 0);

		Option_Color_List[4]->Change_To();
		glVertex3d(X_Part, 7*Y_Part, 0);
		glVertex3d(2*X_Part, 8*Y_Part, 0);
		glVertex3d(5*X_Part, 8*Y_Part, 0);
		glVertex3d(4*X_Part, 7*Y_Part, 0);

		Option_Color_List[5]->Change_To();
		glVertex3d(2*X_Part, 8*Y_Part, 0);
		glVertex3d(3*X_Part, 9*Y_Part, 0);
		glVertex3d(6*X_Part, 9*Y_Part, 0);
		glVertex3d(5*X_Part, 8*Y_Part, 0);

		Option_Color_List[0]->Change_To();
		glVertex3d(4*X_Part, 7*Y_Part, 0);
		glVertex3d(5*X_Part, 8*Y_Part, 0);
		glVertex3d(8*X_Part, 8*Y_Part, 0);
		glVertex3d(7*X_Part, 7*Y_Part, 0);

		Option_Color_List[1]->Change_To();
		glVertex3d(5*X_Part, 8*Y_Part, 0);
		glVertex3d(6*X_Part, 9*Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(8*X_Part, 8*Y_Part, 0);

		Option_Color_List[2]->Change_To();
		glVertex3d(7*X_Part, 7*Y_Part, 0);
		glVertex3d(8*X_Part, 8*Y_Part, 0);
		glVertex3d(8*X_Part, 5*Y_Part, 0);
		glVertex3d(7*X_Part, 4*Y_Part, 0);

		Option_Color_List[3]->Change_To();
		glVertex3d(8*X_Part, 8*Y_Part, 0);
		glVertex3d(9*X_Part, 9*Y_Part, 0);
		glVertex3d(9*X_Part, 6*Y_Part, 0);
		glVertex3d(8*X_Part, 5*Y_Part, 0);

		Option_Color_List[4]->Change_To();
		glVertex3d(7*X_Part, 4*Y_Part, 0);
		glVertex3d(8*X_Part, 5*Y_Part, 0);
		glVertex3d(8*X_Part, 2*Y_Part, 0);
		glVertex3d(7*X_Part, Y_Part, 0);

		Option_Color_List[5]->Change_To();
		glVertex3d(8*X_Part, 5*Y_Part, 0);
		glVertex3d(9*X_Part, 6*Y_Part, 0);
		glVertex3d(9*X_Part, 3*Y_Part, 0);
		glVertex3d(8*X_Part, 2*Y_Part, 0);

	glEnd(); // GL_QUADS

	if(Option_Random) {
		glLineWidth(2);
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3d(7*X_Part, Y_Part, 0);
			glVertex3d(7*X_Part, 7*Y_Part, 0);
			glVertex3d(X_Part, 7*Y_Part, 0);
			glVertex3d(X_Part, Y_Part, 0);
			glVertex3d(7*X_Part, Y_Part, 0);

			glVertex3d(9*X_Part, 3*Y_Part, 0);
			glVertex3d(9*X_Part, 9*Y_Part, 0);
			glVertex3d(7*X_Part, 7*Y_Part, 0);
			glVertex3d(1*X_Part, 7*Y_Part, 0);
			glVertex3d(3*X_Part, 9*Y_Part, 0);
			glVertex3d(9*X_Part, 9*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP

		glBegin(GL_LINE_STRIP);
			glVertex3d(X_Part, 4*Y_Part, 0);
			glVertex3d(7*X_Part, 4*Y_Part, 0);
			glVertex3d(9*X_Part, 6*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP

		glBegin(GL_LINE_STRIP);
			glVertex3d(2*X_Part, 8*Y_Part, 0);
			glVertex3d(8*X_Part, 8*Y_Part, 0);
			glVertex3d(8*X_Part, 2*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP

		glBegin(GL_LINE_STRIP);
			glVertex3d(4*X_Part, Y_Part, 0);
			glVertex3d(4*X_Part, 7*Y_Part, 0);
			glVertex3d(6*X_Part, 9*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP
	}

	glTranslated(-10*X_Part, -15*Y_Part, 0);

	glEndList(); // List_ID

	return List_ID;
}

int Make_Option_Line_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glLineWidth(3);
	if(Option_Width)
		glColor3d(1, 0, 0);
	else
		glColor3d(0, 0, 0);

	glBegin(GL_LINE_STRIP);
		glVertex3d(-4*X_Part, 23*Y_Part, 0);
		glVertex3d(-5*X_Part, 22*Y_Part, 0);
		glVertex3d(-4*X_Part, 21*Y_Part, 0);
		glVertex3d(-5*X_Part, 22*Y_Part, 0);
		glVertex3d(5*X_Part, 22*Y_Part, 0);
		glVertex3d(4*X_Part, 23*Y_Part, 0);
		glVertex3d(5*X_Part, 22*Y_Part, 0);
		glVertex3d(4*X_Part, 21*Y_Part, 0);
	glEnd(); // GL_LINE_STRIP

	if(Option_Height)
		glColor3d(1, 0, 0);
	else
		glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
		glVertex3d(-18*X_Part, 9*Y_Part, 0);
		glVertex3d(-17*X_Part, 10*Y_Part, 0);
		glVertex3d(-16*X_Part, 9*Y_Part, 0);
		glVertex3d(-17*X_Part, 10*Y_Part, 0);
		glVertex3d(-17*X_Part, 0, 0);
		glVertex3d(-18*X_Part, 1*Y_Part, 0);
		glVertex3d(-17*X_Part, 0, 0);
		glVertex3d(-16*X_Part, 1*Y_Part, 0);
	glEnd(); // GL_LINE_STRIP

	if(Option_Depth)
		glColor3d(1, 0, 0);
	else
		glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
		glVertex3d(-8*X_Part, -1*Y_Part, 0);
		glVertex3d(-7*X_Part, 0, 0);
		glVertex3d(-6*X_Part, -1*Y_Part, 0);
		glVertex3d(-7*X_Part, 0, 0);
		glVertex3d(-7*X_Part, -10*Y_Part, 0);
		glVertex3d(-8*X_Part, -9*Y_Part, 0);
		glVertex3d(-7*X_Part, -10*Y_Part, 0);
		glVertex3d(-6*X_Part, -9*Y_Part, 0);
	glEnd(); // GL_LINE_STRIP

	glEndList(); // List_ID

	return List_ID;
}

int Make_Option_Load_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glTranslated(10*X_Part, -25*Y_Part, 0);

	glColor3d(0, 0, 1);
	glBegin(GL_QUADS);
		glVertex3d(3*X_Part, 2*Y_Part, 0);
		glVertex3d(3*X_Part, 8*Y_Part, 0);
		glVertex3d(9*X_Part, 8*Y_Part, 0);
		glVertex3d(9*X_Part, 2*Y_Part, 0);
	glEnd(); // GL_QUADS

	glColor3d(.7, .7, .7);
	glBegin(GL_POLYGON);
		for(auto f1=5.0; f1<=7; f1+=.1)
			glVertex3d(f1*X_Part, (5+sqrt(1-(f1-6)*(f1-6) ) )*Y_Part, 0);
		for(auto f2=5.0; f2<=7; f2+=.1)
			glVertex3d(f2*X_Part, (5-sqrt(1-(f2-6)*(f2-6) ) )*Y_Part, 0);
	glEnd(); // GL_POLYGON

	glBegin(GL_TRIANGLES);
		glVertex3d(6*X_Part, 2*Y_Part, 0);
		glVertex3d(6*X_Part, 5*Y_Part, 0);
		glVertex3d(8*X_Part, 2*Y_Part, 0);
	glEnd(); // GL_TRIANGLES

	if(Option_Load) {
		glLineWidth(2);
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3d(3*X_Part, 2*Y_Part, 0);
			glVertex3d(3*X_Part, 8*Y_Part, 0);
			glVertex3d(9*X_Part, 8*Y_Part, 0);
			glVertex3d(9*X_Part, 2*Y_Part, 0);
			glVertex3d(3*X_Part, 2*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP
		glBegin(GL_LINE_STRIP);
			for(auto f1=5.0; f1<=7; f1+=.1)
				glVertex3d(f1*X_Part, (5+sqrt(1-(f1-6)*(f1-6) ) )*Y_Part, 0);
			for(auto f2=7.0; f2>=5; f2-=.1)
				glVertex3d(f2*X_Part, (5-sqrt(1-(f2-6)*(f2-6) ) )*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP

		glColor3d(1, 0, 0);
		glBegin(GL_QUADS);
			glVertex3d(3*X_Part, 4*Y_Part, 0);
			glVertex3d(3*X_Part, 6*Y_Part, 0);
			glVertex3d(5*X_Part, 6*Y_Part, 0);
			glVertex3d(5*X_Part, 4*Y_Part, 0);
		glEnd(); // GL_QUADS
		glBegin(GL_TRIANGLES);
			glVertex3d(0, 5*Y_Part, 0);
			glVertex3d(3*X_Part, 7*Y_Part, 0);
			glVertex3d(3*X_Part, 3*Y_Part, 0);
		glEnd(); // GL_TRIANGLES
	}

	glTranslated(-10*X_Part, 25*Y_Part, 0);

	glEndList(); // List_ID

	return List_ID;
}

int Make_Option_Save_List(void) {
	static int List_ID=glGenLists(1);

	auto X_Part=Option_Window_Width/40.0, Y_Part=Option_Window_Height/50.0;

	glNewList(List_ID, GL_COMPILE);

	glTranslated(10*X_Part, -15*Y_Part, 0);

	glColor3d(0, 0, 1);
	glBegin(GL_QUADS);
		glVertex3d(3*X_Part, 2*Y_Part, 0);
		glVertex3d(3*X_Part, 8*Y_Part, 0);
		glVertex3d(9*X_Part, 8*Y_Part, 0);
		glVertex3d(9*X_Part, 2*Y_Part, 0);
	glEnd(); // GL_QUADS

	glColor3d(.7, .7, .7);
	glBegin(GL_POLYGON);
		for(auto f1=5.0; f1<=7; f1+=.1)
			glVertex3d(f1*X_Part, (5+sqrt(1-(f1-6)*(f1-6) ) )*Y_Part, 0);
		for(auto f2=5.0; f2<=7; f2+=.1)
			glVertex3d(f2*X_Part, (5-sqrt(1-(f2-6)*(f2-6) ) )*Y_Part, 0);
	glEnd(); // GL_POLYGON

	glBegin(GL_TRIANGLES);
		glVertex3d(6*X_Part, 2*Y_Part, 0);
		glVertex3d(6*X_Part, 5*Y_Part, 0);
		glVertex3d(8*X_Part, 2*Y_Part, 0);
	glEnd(); // GL_TRIANGLES

	if(Option_Save) {
		glLineWidth(2);
		glColor3d(0, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3d(3*X_Part, 2*Y_Part, 0);
			glVertex3d(3*X_Part, 8*Y_Part, 0);
			glVertex3d(9*X_Part, 8*Y_Part, 0);
			glVertex3d(9*X_Part, 2*Y_Part, 0);
			glVertex3d(3*X_Part, 2*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP
		glBegin(GL_LINE_STRIP);
			for(auto f1=5.0; f1<=7; f1+=.1)
				glVertex3d(f1*X_Part, (5+sqrt(1-(f1-6)*(f1-6) ) )*Y_Part, 0);
			for(auto f2=7.0; f2>=5; f2-=.1)
				glVertex3d(f2*X_Part, (5-sqrt(1-(f2-6)*(f2-6) ) )*Y_Part, 0);
		glEnd(); // GL_LINE_STRIP

		glColor3d(1, 0, 0);
		glBegin(GL_QUADS);
			glVertex3d(0, 6*Y_Part, 0);
			glVertex3d(2*X_Part, 6*Y_Part, 0);
			glVertex3d(2*X_Part, 4*Y_Part, 0);
			glVertex3d(0, 4*Y_Part, 0);
		glEnd(); // GL_QUADS
		glBegin(GL_TRIANGLES);
			glVertex3d(2*X_Part, 7*Y_Part, 0);
			glVertex3d(5*X_Part, 5*Y_Part, 0);
			glVertex3d(2*X_Part, 3*Y_Part, 0);
		glEnd(); // GL_TRIANGLES
	}

	glTranslated(-10*X_Part, 15*Y_Part, 0);

	glEndList(); // List_ID

	return List_ID;
}

void Make_All_Option_Lists(void) {
	Option_Cube_List=Make_Option_Cube_List(Option_Color_List);
	Option_Exit_List=Make_Option_Exit_List();
	Option_Solved_List=Make_Option_Solved_List();
	Option_Random_List=Make_Option_Random_List();
	Option_Line_List=Make_Option_Line_List();
	Option_Load_List=Make_Option_Load_List();
	Option_Save_List=Make_Option_Save_List();
}
