
#include "Output.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <sys/timeb.h>

#include "ui.h"

#include "OptionWindow.h"
#include "Quaternion.h"

// Since OpenGL is written in C, functions and variables must be global in scope
Cube Current_Cube;
Color_p *Color_List;
int Cube_List, Arrow_List;
auto Window_Size=300.0;

int Window_ID, Option_Window_ID;

auto View_Spinning=false, Mouse_Dragging=false, New_View=true, Cube_Twisting=false;
int beginx, beginy;

Vector Start_Vector, End_Vector;
Quaternion Old_Quaternion, New_Quaternion;
Direction Arrow_Direction;

bool Draw_Section;
Vector Menu_Color(.7, .7, .7);
bool Is_Option_Window_Open;
int Section_List, Slice_List, Option_List;
double Slice_Angle, Max_Angle;
double Time_Delay;

void Initialize_Window(int argc, char **argv, Cube& cube, Color_p Color_Array[]) {
	Current_Cube=cube;
	Current_Cube.Randomize();
	Color_List=Color_Array;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	Window_ID=glutCreateWindow("Liam's Cubish");
	glClearColor(.6, .6, .6, 0);
	glDepthFunc(GL_LEQUAL);

	glutDisplayFunc(Display);

	auto Height= Current_Cube[1].height();
	auto Width = Current_Cube[1].length();
	auto Depth = Current_Cube[2].length();

	auto Diagonal=sqrt(Width*Width + Height*Height + Depth*Depth);

	glViewport(0, 0, Window_Size, Window_Size);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 1, Diagonal/2, 3*Diagonal);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 1.4*Diagonal, 0, 0, 0, 0, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();

	Cube_List=Current_Cube.Make_GL_List();
}

// If the viewpoint coordinates have been changed, readjust the matrix
void Rotate_View() {
	double Rotation_Matrix[4][4];

	glPopMatrix();
	glPushMatrix();
	New_Quaternion.Build_Rotation_Matrix(Rotation_Matrix);
	glMultMatrixd(&Rotation_Matrix[0][0]);
	New_View=false;

	double X, Y, Z;
	double Model[16], Projection[16];
	int Viewport[4];

	glGetDoublev(GL_PROJECTION_MATRIX, Projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, Model);
	glGetIntegerv(GL_VIEWPORT, Viewport);

	// Find the coordinates for the option window button
	gluUnProject(5, Window_Size-5, .15, Model, Projection, Viewport, &X, &Y, &Z);

	if(Option_List==0)
		Option_List=glGenLists(1);

	glNewList(Option_List, GL_COMPILE); {
		glColor3d(Menu_Color[1], Menu_Color[2], Menu_Color[3]);
		glBegin(GL_QUADS); {
			glVertex3d(X, Y, Z);
			gluUnProject(25, Window_Size-5, .15, Model, Projection, Viewport, &X, &Y, &Z);
			glVertex3d(X, Y, Z);
			gluUnProject(25, Window_Size-25, .15, Model, Projection, Viewport, &X, &Y, &Z);
			glVertex3d(X, Y, Z);
			gluUnProject(5, Window_Size-25, .15, Model, Projection, Viewport, &X, &Y, &Z);
			glVertex3d(X, Y, Z);
		} glEnd();
	} glEndList(); // Option_List
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(New_View)
		Rotate_View();

	// This if() block attempts to adjust for speed variance on different computers
	static int First_View=1;
	if(First_View<50) {
		time_t Begin, End;
		struct timeb Begin_Milli, End_Milli;

		Begin=time(NULL);
		ftime(&Begin_Milli);

		// Draw things, but don't display them
		glBegin(GL_QUADS); {
			for(auto n1=0; n1<20; n1++)
				for(auto n2=0; n2<20; n2++)
					for(auto n3=0; n3<20; n3++) {
						glVertex3s(n1, n2, n3);
						glVertex3s(n1, n2+1, n3);
						glVertex3s(n1+1, n2+1, n3+1);
						glVertex3s(n1+1, n2, n3+1);
					}
		} glEnd();
		ftime(&End_Milli);
		End=time(NULL);

		int Current_Render_Time;
		if(End==Begin)
			Current_Render_Time=End_Milli.millitm-Begin_Milli.millitm;
		else // End!=Begin
			Current_Render_Time=Begin_Milli.millitm-End_Milli.millitm;
		static auto Average_Render_Time=0.0;

		Average_Render_Time=(First_View*Average_Render_Time + Current_Render_Time)/First_View;
		++First_View;

		glutPostRedisplay();
		if(First_View==50) {
			// After adjusting for speed, set up the callback functions
			Time_Delay=Average_Render_Time;
			glutMouseFunc(Mouse);
			glutPassiveMotionFunc(Passive_Motion);
			glutMotionFunc(Motion);
			glutEntryFunc(NULL);
			glutVisibilityFunc(NULL);
			glutKeyboardFunc(Keyboard);
			glutReshapeFunc(Resize);
		}
		return;
	}


	// If a slice is animated
	if(Draw_Section) {
		glCallList(Section_List);
		glRotatef(Slice_Angle, Start_Vector[1], Start_Vector[2], Start_Vector[3]);
		glCallList(Slice_List);
		glRotatef(-Slice_Angle, Start_Vector[1], Start_Vector[2], Start_Vector[3]);
		glClearColor(.6, .6, .6, 0);
	}
	else {
		Slice_Angle=0;
		glCallList(Cube_List);
		if(Arrow_Direction != Direction::NONE)
			glCallList(Arrow_List);
		if(Current_Cube.Is_Solved() )
			glutIdleFunc(Pulsate_Background_Color);
		else {
			glClearColor(.6, .6, .6, 0);
			glutIdleFunc(NULL);
		}
	}

	glCallList(Option_List);

	glutSwapBuffers();
}

void Rotate_Slice() {
	glutSetWindow(Window_ID);
	if(Arrow_Direction==Direction::Down || Arrow_Direction==Direction::Left)
		Slice_Angle += 0.78 * Time_Delay;
	else
		Slice_Angle -= 0.78 * Time_Delay;
	if(Slice_Angle>Max_Angle || Slice_Angle<-Max_Angle) {
		Draw_Section=false;
		Arrow_Direction=Direction::NONE;
		glutMouseFunc(Mouse);
	}
	Display();
}

#include <iostream>
void Mouse(int Button, int State, int X_Coord, int Y_Coord) {
	if(Button==GLUT_LEFT_BUTTON) {
		if(State==GLUT_DOWN) {
			// Allow for someone clicking the option window button
			if(X_Coord>5 && X_Coord<25 && Y_Coord>5 && Y_Coord<25) {
				if(!Is_Option_Window_Open) {
					New_Window(Color_List);
					Is_Option_Window_Open=true;
				}
				else {
					glutSetWindow(Option_Window_ID);
					glutPopWindow();
				}
			}
			// Someone is rotating the view
			else {
				Mouse_Dragging=true;
				beginx=X_Coord;
				beginy=Y_Coord;
			}
		}
		else // State==GLUT_UP
			// Release the rotation
			Mouse_Dragging=false;
	}
	if(Button==GLUT_RIGHT_BUTTON) {
		if(State==GLUT_DOWN) {
			double X, Y, Z;
			double Model[16], Projection[16];
			int Viewport[4];

			// Figure out how the view is currently oriented
			glGetDoublev(GL_PROJECTION_MATRIX, Projection);
			glGetDoublev(GL_MODELVIEW_MATRIX, Model);
			glGetIntegerv(GL_VIEWPORT, Viewport);

			// Develop pick rays for finding where in the viewing
			// colume the user clicked
			gluUnProject(X_Coord, Viewport[3]-Y_Coord-1, 0, Model, Projection,
					Viewport, &X, &Y, &Z);
			Vector Begin(X, Y, Z);
			gluUnProject(X_Coord, Viewport[3]-Y_Coord-1, 1, Model, Projection,
					Viewport, &X, &Y, &Z);
			Vector End(X, Y, Z);

			// The user is twisting a section of cube
			Start_Vector=Find_Cube_Point(Begin, End);
			End_Vector=Start_Vector;
			Cube_Twisting=true;
		}
		else { // State==GLUT_UP
			Cube_Twisting=false;
			// If there is an arrow present, change that cube
			if(Arrow_Direction != Direction::NONE) {
				auto Cube_Place = Get_Twist_Side(Start_Vector, Arrow_Direction);
				Slice_List = Current_Cube.Make_Slice_GL_List(Cube_Place[0], Cube_Place[1]);
				Section_List = Current_Cube.Make_Section_GL_List(Cube_Place[0], Cube_Place[1]);
				// Twist() returns true if a rotation happened
				Draw_Section = Current_Cube.Twist(Start_Vector, Arrow_Direction);
				if(Draw_Section) {
					Start_Vector = Get_Twist_Vector(Cube_Place[0]);
					// Get ready to animate and redraw
					Cube_List=Current_Cube.Make_GL_List();
					Max_Angle = Cube_Place[2];
					glutIdleFunc(Rotate_Slice);
					glClearColor(.6, .6, .6, 0);
					// No user input while animating!!
					glutMouseFunc(NULL);
				}
				else {
					// Just remove the arrow if the cube did
					// not change
					Arrow_Direction=Direction::NONE;
					glutPostRedisplay();
				}
			}
		}
	}
}

// Standard idle function for moving the viewpoint.  Adds the quaternions
// together to create a single quaternion rotation.
void Animate_Rotation() {
	if(View_Spinning) {
		New_Quaternion=Old_Quaternion+New_Quaternion;
		New_View=true;
		// Things have changed, must redraw
		glutPostRedisplay();
	}
}

// This allows the view change through the trackball to stop spinning after a short time
void Set_Spin(int Value) {
	View_Spinning=(bool)Value;
}

// Animation for a solved cube.  Pulsates the background color.
void Pulsate_Background_Color() {
	glutSetWindow(Window_ID);
	static auto Red=.6, Green=.6, Blue=.6;
	static auto Increasing=true;

	// Time-dependent
	static const auto Increment=Time_Delay/3000;

	if(Increasing)
		Red+=Increment, Green+=Increment, Blue+=Increment;
	else // !Increasing
		Red-=Increment, Green-=Increment, Blue-=Increment;

	// Change colors and redraw
	glClearColor(Red, Green, Blue, 0);
	glutPostRedisplay();

	// Don't go all the way to pure black or white.  Make the image softer.
	if(Red>=.9)
		Increasing=false;
	if(Red<=.1)
		Increasing=true;
}

// Generally only needed to change the color of the option window button when it
// is rolled over
void Passive_Motion(int X_Coord, int Y_Coord) {
	// If the mouse is in the correct corner
	if(X_Coord>5 && X_Coord<25 && Y_Coord>5 && Y_Coord<25) {
		// Give them a special cursor
		glutSetCursor(GLUT_CURSOR_HELP);

		Menu_Color[1]=1;
		Menu_Color[2]=1;
		Menu_Color[3]=0;
		New_View=true;
		glutPostRedisplay();
	}
	// Otherwise, uncolor the option box
	else if(Menu_Color[1]==1) {
		// Restore the old cursor
		glutSetCursor(GLUT_CURSOR_INHERIT);

		Menu_Color[1]=.7;
		Menu_Color[2]=.7;
		Menu_Color[3]=.7;
		New_View=true;
		glutPostRedisplay();
	}
}

// The mouse is clicked and moving for this callback
void Motion(int X_Coord, int Y_Coord) {
	if(Cube_Twisting) {
		double X, Y, Z;
		double Model[16], Projection[16];
		int Viewport[4];

		// Figure out how the viewing matrix is set up
		glGetDoublev(GL_PROJECTION_MATRIX, Projection);
		glGetDoublev(GL_MODELVIEW_MATRIX, Model);
		glGetIntegerv(GL_VIEWPORT, Viewport);

		// Develop pick rays for finding where in the viewing
		// colume the user clicked
		gluUnProject(X_Coord, Viewport[3]-Y_Coord-1, 0, Model, Projection,
				Viewport, &X, &Y, &Z);
		Vector Begin(X, Y, Z);
		gluUnProject(X_Coord, Viewport[3]-Y_Coord-1, 1, Model, Projection,
				Viewport, &X, &Y, &Z);
		Vector End(X, Y, Z);

		End_Vector=Find_Direction(Start_Vector, Begin, End);

		// If an arrow is generated, draw it
		if(Arrow_Direction!=Get_Direction(Start_Vector, End_Vector) ) {
			Arrow_Direction=Get_Direction(Start_Vector, End_Vector);
			Arrow_List=Make_Arrow_GL_List();
			glutPostRedisplay();
		}
	}

	// If we are spinning the cube to view it
	else if(Mouse_Dragging) {
		// Perform a trackball projection from two-space to a
		// quaternion
		Old_Quaternion.Trackball(
				(static_cast<double>(2*beginx) - Window_Size)/Window_Size,
				(Window_Size - static_cast<double>(2*beginy) )/Window_Size,
				(static_cast<double>(2*X_Coord) - Window_Size)/Window_Size,
				(Window_Size - static_cast<double>(2*Y_Coord) )/Window_Size);
		View_Spinning=true;
		beginx=X_Coord;
		beginy=Y_Coord;

		// This function allows the view to rotate
		glutIdleFunc(Animate_Rotation);

		// This function prevents the cube from continuing to rotate
		// for too long
		glutTimerFunc(300, Set_Spin, 0);
	}
}

// The window is square.  Keep it that way.
void Resize(int X_Size, int Y_Size) {
	auto New_Size=X_Size<Y_Size?X_Size:Y_Size;
	glutReshapeWindow(New_Size, New_Size);
	glViewport(0, 0, New_Size, New_Size);
	Window_Size=New_Size;
}

// Just responds to 'Q' and 'q'.  Otherwise useless.
void Keyboard(unsigned char Key, int, int) {
	switch(Key) {
	case 'q':
	case 'Q':
		glutDestroyWindow(Window_ID);
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

int Make_Arrow_GL_List() {
	static auto List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE);

	Current_Cube.View_Side(Start_Vector[1]-1);
	glTranslated(Start_Vector[2]-.5, Current_Cube[Start_Vector[1]].height()-Start_Vector[3]+.5, 0);

	auto Angle=0.0;
	switch(Arrow_Direction) {
		case Direction::Up:
			Angle=-90;
			break;
		case Direction::Down:
			Angle=90;
			break;
		case Direction::Right:
			Angle=180;
			break;
		default:
			break;
	}
	glRotatef(Angle, 0, 0, 1);
	glLineWidth(5);
	glBegin(GL_LINE_STRIP); {
		glVertex3d(0   , 0   , .05);
		glVertex3d(1   , 0   , .05);
		glVertex3d( .75,  .25, .05);
		glVertex3d(1   , 0   , .05);
		glVertex3d( .75, -.25, .05);
	} glEnd();
	glRotatef(-Angle, 0, 0, 1);

	glTranslated(.5-Start_Vector[2], Start_Vector[3] - Current_Cube[Start_Vector[1]].height()-.5, 0);
	Current_Cube.Undo_View_Side(Start_Vector[1]-1);

	glEndList();

	return List_ID;
}

int Get_Cube_Section(Cube& cube, int, int) {
	auto List_ID=glGenLists(1);

	glNewList(List_ID, GL_COMPILE);
	for(auto n1 = 0; n1 < 6; ++n1) {

		Current_Cube.View_Side(n1);

		for(auto n2 = 0; n2 < cube[n1].height(); ++n2) {
			for(auto n3 = 0; n3 < cube[n1].length(); ++n3) {
				cube[n1](n3+1, cube[n1].height() - n2)->Get_Color()->Change_To();
				glBegin(GL_QUADS); {
					glVertex2s(n3  , n2);
					glVertex2s(n3  , n2+1);
					glVertex2s(n3+1, n2+1);
					glVertex2s(n3+1, n2);
				} glEnd();

				glColor3d(0, 0, 0);
				glBegin(GL_LINE_STRIP); {
					glVertex2s(n3  , n2);
					glVertex2s(n3  , n2+1);
					glVertex2s(n3+1, n2+1);
					glVertex2s(n3+1, n2);
					glVertex2s(n3  , n2);
				} glEnd();
			}
		}

		Current_Cube.Undo_View_Side(n1);
	}

	glEndList();

	return List_ID;
}

Vector Find_Cube_Point(Vector Begin, Vector End) {
	auto Change=End-Begin;
	double X_Temp, Y_Temp, Z_Temp;
	auto Half_Width = Current_Cube[1].length()/2.0;
	auto Half_Height=Current_Cube[1].height()/2.0;
	auto Half_Depth=Current_Cube[2].length()/2.0;

	for(auto n=0; n<=6; n++) {
		if(Change[1]!=1 && Change[1]!=2 && Change[1]!=3 && Change[1]!=4 && Change[1]!=5 && Change[1]!=6)
		switch(n) {
		case 1:
			if(Begin[3]>Half_Depth && End[3]<Half_Depth) {
				Z_Temp=Begin[3]-Half_Depth;
				Y_Temp=-Z_Temp*Change[2]/Change[3] + Begin[2];
				X_Temp=-Z_Temp*Change[1]/Change[3] + Begin[1];

				if((Y_Temp<Half_Height && Y_Temp>-Half_Height) && (X_Temp<Half_Width && X_Temp>-Half_Width) )
					Change[1]=1, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Half_Height-Y_Temp+1);
			}
			break;
		case 2:
			if(Begin[1]<-Half_Width && End[1]>-Half_Width) {
				X_Temp=-Half_Width-Begin[1];
				Y_Temp=X_Temp*Change[2]/Change[1] + Begin[2];
				Z_Temp=X_Temp*Change[3]/Change[1] + Begin[3];

				if((Y_Temp<Half_Height && Y_Temp>-Half_Height) && (Z_Temp<Half_Depth && Z_Temp>-Half_Depth) )
					Change[1]=2, Change[2]=int(Z_Temp+Half_Depth+1), Change[3]=int(Half_Height-Y_Temp+1);
			}
			break;
		case 3:
			if(Begin[2]>Half_Height && End[2]<Half_Height) {
				Y_Temp=Begin[2]-Half_Height;
				X_Temp=-Y_Temp*Change[1]/Change[2] + Begin[1];
				Z_Temp=-Y_Temp*Change[3]/Change[2] + Begin[3];

				if((X_Temp<Half_Width && X_Temp>-Half_Width) && (Z_Temp<Half_Depth && Z_Temp>-Half_Depth) )
					Change[1]=3, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Z_Temp+Half_Depth+1);
			}
			break;
		case 4:
			if(Begin[1]>Half_Width && End[1]<Half_Width) {
				X_Temp=Begin[1]-Half_Width;
				Y_Temp=-X_Temp*Change[2]/Change[1] + Begin[2];
				Z_Temp=-X_Temp*Change[3]/Change[1] + Begin[3];

				if((Y_Temp<Half_Height && Y_Temp>-Half_Height) && (Z_Temp<Half_Depth && Z_Temp>-Half_Depth) )
					Change[1]=4, Change[2]=int(Half_Depth-Z_Temp+1), Change[3]=int(Half_Height-Y_Temp+1);
			}
			break;
		case 5:
			if(Begin[2]<-Half_Height && End[2]>-Half_Height) {
				Y_Temp=-Half_Height-Begin[2];
				X_Temp=Y_Temp*Change[1]/Change[2] + Begin[1];
				Z_Temp=Y_Temp*Change[3]/Change[2] + Begin[3];

				if((X_Temp<Half_Width && X_Temp>-Half_Width) && (Z_Temp<Half_Depth && Z_Temp>-Half_Depth) )
					Change[1]=5, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Half_Depth-Z_Temp+1);
			}
			break;
		case 6:
			if(Begin[3]<-Half_Depth && End[3]>-Half_Depth) {
				Z_Temp=-Half_Depth-Begin[3];
				Y_Temp=Z_Temp*Change[2]/Change[3] + Begin[2];
				X_Temp=Z_Temp*Change[1]/Change[3] + Begin[1];

				if((Y_Temp<Half_Height && Y_Temp>-Half_Height) && (X_Temp<Half_Width && X_Temp>-Half_Width) )
					Change[1]=6, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Y_Temp+Half_Height+1);
			}
			break;
		default:
			break;
		}
	}

	if(Change[1]!=1 && Change[1]!=2 && Change[1]!=3 && Change[1]!=4 && Change[1]!=5 && Change[1]!=6)
		Change.Zero();
	return Change;
}

Vector Find_Direction(Vector Initial, Vector Begin, Vector End) {
	auto Change=End-Begin;
	double X_Temp, Y_Temp, Z_Temp;
	auto Half_Width = Current_Cube[1].length()/2.0;
	auto Half_Height= Current_Cube[1].height()/2.0;
	auto Half_Depth = Current_Cube[2].length()/2.0;

	switch(int(Initial[1]) ) {
	case 1:
		Z_Temp=Begin[3]-Half_Depth;
		Y_Temp=-Z_Temp*Change[2]/Change[3] + Begin[2];
		X_Temp=-Z_Temp*Change[1]/Change[3] + Begin[1];

		Change[1]=1, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Half_Height-Y_Temp+1);
		break;
	case 2:
		X_Temp=-Half_Width-Begin[1];
		Y_Temp=X_Temp*Change[2]/Change[1] + Begin[2];
		Z_Temp=X_Temp*Change[3]/Change[1] + Begin[3];

		Change[1]=2, Change[2]=int(Z_Temp+Half_Depth+1), Change[3]=int(Half_Height-Y_Temp+1);
		break;
	case 3:
		Y_Temp=Begin[2]-Half_Height;
		X_Temp=-Y_Temp*Change[1]/Change[2] + Begin[1];
		Z_Temp=-Y_Temp*Change[3]/Change[2] + Begin[3];

		Change[1]=3, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Z_Temp+Half_Depth+1);
		break;
	case 4:
		X_Temp=Begin[1]-Half_Width;
		Y_Temp=-X_Temp*Change[2]/Change[1] + Begin[2];
		Z_Temp=-X_Temp*Change[3]/Change[1] + Begin[3];

		Change[1]=4, Change[2]=int(Half_Depth-Z_Temp+1), Change[3]=int(Half_Height-Y_Temp+1);
		break;
	case 5:
		Y_Temp=-Half_Height-Begin[2];
		X_Temp=Y_Temp*Change[1]/Change[2] + Begin[1];
		Z_Temp=Y_Temp*Change[3]/Change[2] + Begin[3];

		Change[1]=5, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Half_Depth-Z_Temp+1);
		break;
	case 6:
		Z_Temp=-Half_Depth-Begin[3];
		Y_Temp=Z_Temp*Change[2]/Change[3] + Begin[2];
		X_Temp=Z_Temp*Change[1]/Change[3] + Begin[1];

		Change[1]=6, Change[2]=int(X_Temp+Half_Width+1), Change[3]=int(Y_Temp+Half_Height+1);
		break;
	default:
		break;
	}

	if(Change[1]!=1 && Change[1]!=2 && Change[1]!=3 && Change[1]!=4 && Change[1]!=5 && Change[1]!=6)
		Change.Zero();
	return Change;
}

Direction Get_Direction(Vector Begin, Vector End) {
	if(Begin[2]==End[2] && Begin[3]==End[3])
		return Direction::NONE;

	if(std::abs(Begin[2]-End[2]) < std::abs(Begin[3]-End[3]) ) {
		if(Begin[3]>End[3])
			return Direction::Down;
		else
			return Direction::Up;
	}
	else {
		if(Begin[2]>End[2])
			return Direction::Right;
		else
			return Direction::Left;
	}
}

Vector Get_Twist_Vector(int Side) {
	Vector Returner;

	switch(Side) {
	case 1:
		Returner[3]=1;
		break;
	case 2:
		Returner[1]=-1;
		break;
	case 3:
		Returner[2]=1;
		break;
	case 4:
		Returner[1]=1;
		break;
	case 5:
		Returner[2]=-1;
		break;
	case 6:
		Returner[3]=-1;
		break;
	}

	return Returner;
}

std::array<double, 3> Get_Twist_Side(Vector Origin, Direction direction) {
	std::array<double, 3> found;

	switch(direction) {
		case Direction::Up:
		case Direction::Down:
			found[1]=Origin[2];
			break;
		case Direction::Left:
		case Direction::Right:
			found[1]=Origin[3];
			Origin[1]=Origin[1]+6;
			break;
		default:
			break;
			// What should NONE do?
	}

	switch(static_cast<int>(Origin[1])) {
		case 1:
		case 3:
		case 5:
		case 6:
			found[0]=2;
			break;
		case 7:
		case 8:
		case 10:
			found[0]=3;
			break;
		case 2:
		case 9:
			found[0]=6;
			break;
		case 4:
		case 11:
			found[0]=1;
			break;
		case 12:
			found[0]=5;
			break;
	}

	if(Current_Cube[found[0]].height() == Current_Cube[found[0]].length()) {
		found[2]=90;
	}
	else {
		found[2]=180;
	}

	return found;
}

void Cube_Remake_View() {
	glutSetWindow(Window_ID);
	auto Height= Current_Cube[1].height();
	auto Width = Current_Cube[1].length();
	auto Depth = Current_Cube[2].length();

	auto Diagonal=sqrt(Width*Width + Height*Height + Depth*Depth);

	Quaternion::RADIUS = std::max({Width, Height, Depth})/4.0;

	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, Diagonal/2, 3*Diagonal);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 1.4*Diagonal, 0, 0, 0, 0, 1, 0);
	glPushMatrix();

	New_View=true;
	glutPostRedisplay();
}
