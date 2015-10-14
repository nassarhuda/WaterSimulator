#ifdef _WIN32
#include <windows.h>
#endif

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "GPGPU.h"

float time = 0.1;
float running = 0;

float waveLength1 = 0.17;
float velocity1 = 0.3;
float amplitude1 = 0.3;
float xdir1 = 1.0;
float ydir1 = 0.1;

float waveLength2 = 4.0;
float velocity2 = 0.5;
float amplitude2 = 0.1;
float xdir2 = 0.5;
float ydir2 = -0.5;

float waveLength3 = 0.22;
float velocity3 = 0.4;
float amplitude3 = 0.7;
float xdir3 = 1.0;
float ydir3 = 0.5;

float waveLength4 = 0.35;
float velocity4 = 0.3;
float amplitude4 = 0.2;
float xdir4 = -0.2;
float ydir4 = -1.2;


static int winWidth = 512;
static int winHeight = 512;
static int winId = -1;
static GLUI *glui;
GLUI_RadioGroup *radio;
int   obj;
GPGPU  *gpgpu;

/*****************************************************************************
*****************************************************************************/
static void
leftButtonDownCB(void)
{
   gpgpu->restart();
}

/*****************************************************************************
*****************************************************************************/
static void
leftButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
middleButtonDownCB(void)
{
}


/*****************************************************************************
*****************************************************************************/
static void
middleButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
rightButtonDownCB(void)
{
}


/*****************************************************************************
*****************************************************************************/
static void
rightButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
mouseCB(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      leftButtonDownCB();
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      leftButtonUpCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
      middleButtonDownCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
      middleButtonUpCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
      rightButtonDownCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
      rightButtonUpCB();
}


/*****************************************************************************
*****************************************************************************/
static void
motionCB(int x, int y)
{
}


/*****************************************************************************
*****************************************************************************/
void
reshapeCB(int width, int height)
{
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    
	// Use orthographic projection
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();   
}

/*****************************************************************************
*****************************************************************************/
void
keyboardCB(unsigned char key, int x, int y)
{
}

/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{		
	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	for (int i = 0; i < 20; ++i) {
		// Update the cells' states
		if (running){
		time = time +0.001;
		//printf("time is %f\n",time);
		gpgpu->update(time,waveLength1,velocity1,amplitude1,xdir1,ydir1,
			waveLength2,velocity2,amplitude2,xdir2,ydir2,
			waveLength3,velocity3,amplitude3,xdir3,ydir3,
			waveLength4,velocity4,amplitude4,xdir4,ydir4,obj);  
		}
		// Display the results
		gpgpu->display();
	}

    glutSwapBuffers();
}

/*****************************************************************************
*****************************************************************************/
void initialize()
{
    // Initialize glew library
    glewInit();

    // Create the gpgpu object
    gpgpu = new GPGPU(512, 512);
}

/*****************************************************************************
*****************************************************************************/

void theButtonCallBack()
{
	running = 1;
}

void MakeGUI()
{
	glui = GLUI_Master.create_glui("GUI", 0, 0, 0);
	glui->add_statictext("Control variables of the waves here");
	glui->add_statictext("Wave 1:");
	glui->add_spinner( "WaveLength1:",GLUI_SPINNER_FLOAT, &waveLength1 );
	glui->add_spinner( "Velocity1:",GLUI_SPINNER_FLOAT, &velocity1 );
	glui->add_spinner( "Amplitude1:",GLUI_SPINNER_FLOAT, &amplitude1 );
	glui->add_spinner( "x-Direction1:",GLUI_SPINNER_FLOAT, &xdir1 );
	glui->add_spinner( "y-Direction1:",GLUI_SPINNER_FLOAT, &ydir1 );

	//GLUI_Spinner *segment_spinner = glui->add_spinner( "WaveLength1:",GLUI_SPINNER_FLOAT, &waveLength1 );
	glui->add_separator();
	glui->add_statictext("Wave 2:");
	glui->add_spinner( "WaveLength2:",GLUI_SPINNER_FLOAT, &waveLength2 );
	glui->add_spinner( "Velocity2:",GLUI_SPINNER_FLOAT, &velocity2 );
	glui->add_spinner( "Amplitude2:",GLUI_SPINNER_FLOAT, &amplitude2);
	glui->add_spinner( "x-Direction2:",GLUI_SPINNER_FLOAT, &xdir2 );
	glui->add_spinner( "y-Direction2:",GLUI_SPINNER_FLOAT, &ydir2 );

	glui->add_separator();
	glui->add_statictext("Wave 3:");
	glui->add_spinner( "WaveLength3:",GLUI_SPINNER_FLOAT, &waveLength3 );
	glui->add_spinner( "Velocity3:",GLUI_SPINNER_FLOAT, &velocity3 );
	glui->add_spinner( "Amplitude3:",GLUI_SPINNER_FLOAT, &amplitude3 );
	glui->add_spinner( "x-Direction3:",GLUI_SPINNER_FLOAT, &xdir3 );
	glui->add_spinner( "y-Direction3:",GLUI_SPINNER_FLOAT, &ydir3 );

	glui->add_separator();
	glui->add_statictext("Wave 4:");
	glui->add_spinner( "WaveLength4:",GLUI_SPINNER_FLOAT, &waveLength4 );
	glui->add_spinner( "Velocity4:",GLUI_SPINNER_FLOAT, &velocity4 );
	glui->add_spinner( "Amplitude4:",GLUI_SPINNER_FLOAT, &amplitude4 );
	glui->add_spinner( "x-Direction4:",GLUI_SPINNER_FLOAT, &xdir4 );
	glui->add_spinner( "y-Direction4:",GLUI_SPINNER_FLOAT, &ydir4 );

	
	GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Normals" );
	radio = new GLUI_RadioGroup( obj_panel,&obj);
	glui->add_radiobutton_to_group(radio, "Without Normals" );
	glui->add_radiobutton_to_group(radio, "With Normals" );
	glui->add_button( "START", 1,(GLUI_Update_CB) theButtonCallBack );
	glui->set_main_gfx_window(winId);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idleFunc);
}



/*****************************************************************************
*****************************************************************************/
int
main(int argc, char *argv[])
{
	// init OpenGL/GLUT
	glutInit(&argc, argv);
	
	// create main window
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	winId = glutCreateWindow("MyWindow");

	initialize();

	// make GLUI GUI
	MakeGUI();
	
	
	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);

	// force initial matrix setup
	reshapeCB(winWidth, winHeight);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutMainLoop();

	return (TRUE);
}
