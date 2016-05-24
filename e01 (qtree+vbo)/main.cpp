
#include <windows.h> 
#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include "cScene.h"
#include "cPlayer.h"
#include "cInput.h"
#include "cLog.h"
#include "globals.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//vsync off with OpenGL
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
void setVSync(int interval=1)
{
	const char *extensions = (const char *)glGetString( GL_EXTENSIONS );

	if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
		return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
		if( wglSwapIntervalEXT ) wglSwapIntervalEXT(interval);
	}
}

cScene  Scene;
cPlayer Player;
cInput	Input;

int show, enable;

// Initializations
bool init(void)
{
	bool res;

	srand(13);
	
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);			
	glDepthFunc(GL_LESS);				
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	Player.Init(800,600);
	Player.Move(cVector3D(0,MAP_SIZE>>1,0));
	Player.Rotate(cVector3D(-30,-45,0));

	res = Scene.Init();

	show = 0;
	enable = ENABLE_LIGHTING;

	return res;
}

//Rendering
void GameRender(void)   
{
	static int t1=0;
	static int frames=0;
	int t2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

	Scene.Render(&Player,show,enable);
		
	frames++;
	t2 = glutGet(GLUT_ELAPSED_TIME);

	if(t2-t1>1000)
	{
		char s[128];
		float fps = 1000.0f*(float)frames/(float)(t2-t1);
		sprintf(s,"FPS: %.2f",fps);
		glutSetWindowTitle(s);
		t1 = t2;
		frames = 0;
	}
    glutSwapBuffers();
}

//Reshaping handler (required even in fullscreen-only modes)
void GameReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Calculate the aspect ratio and set the clipping volume
	gluPerspective(45, (float)w/(float)h, 1.0, 10000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Keyboard handler (normal keys)
void GameKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
		case 'W':	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);			break;
		case 's':
		case 'S':	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);			break;
		case 'p':
		case 'P':	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);			break;
		case 'l':
		case 'L':	enable ^= ENABLE_LIGHTING;								
					if(enable & ENABLE_LIGHTING) glEnable(GL_LIGHTING);
					else						 glDisable(GL_LIGHTING);
																		break;
		case 'a':
		case 'A':	show ^= SHOW_AXIS;									break;
		case 'q':
		case 'Q':	show ^= SHOW_QUADTREE;								break;
		case 'u':
		case 'U':	show ^= SHOW_TOP_VIEW;								break;
		case 'o':	
		case 'O':	show ^= SHOW_ORTHO;									break;
		
		case '-':	Player.SetSpeed(Player.GetSpeed()/2.0f);			break;
		case '+':	Player.SetSpeed(Player.GetSpeed()*2.0f);			break;
		
		case '0':	Player.Move(cVector3D(0,MAP_SIZE>>1,0));
					Player.Rotate(cVector3D(-30,-45,0));				break;

		case 27:	exit(0);											break;
	}
}

//Keyboard handler for special keys (like arrow keys and function keys)
void GameSpecialKeys(int a_key, int x, int y)
{
	cVector3D v;

	switch (a_key)
	{
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: Input.SetKeyDown(a_key); break;
	}
}
//Keyboard handler for special key releases.
void GameSpecialKeysUp(int a_key, int x, int y)
{
	switch(a_key)
	{
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: Input.SetKeyUp(a_key);	break;
	}
}
void GameMouseMove(int x, int y)
{
	Input.SetMousePosition(x,y);
}

//Check the user input and performs the rendering
void GameLoop()
{
	// Process Input
	if(Input.Updated())
	{
		Player.Update(&Input);
		if(Input.MouseUpdated())	Input.CenterMouse();
	}
	// Logic
	// ...
	
	// Rendering
	GameRender();
}

void main(int argc, char** argv)
{
	bool res;
	cLog *Log;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	if(0)
	{
		glutGameModeString("800:600:32");
		glutEnterGameMode();
	}
	else
	{
		glutInitWindowPosition(100,100);
		glutInitWindowSize(800,600);
		glutCreateWindow("VBO Terrain");
	}

	Input.Init(100,100,800,600);
	if(glewInit() != GLEW_OK)
	{
		Log = cLog::Instance();
		Log->Msg("Error initializing GLEW.");
		exit(-1);
	}
	res = init();
	if(!res)
	{
		Log = cLog::Instance();
		Log->Msg("Error initializing Scene.");
		exit(-1);
	}

	glutDisplayFunc(GameRender);								// Register the display function
	glutReshapeFunc(GameReshape);								// Register the Reshape handler
	glutKeyboardFunc(GameKeyboard);								// Register the keyboard handler
	glutSpecialFunc(GameSpecialKeys);							// Register special keys handler
	glutSpecialUpFunc(GameSpecialKeysUp);						// Called when a special key released
	glutMotionFunc(GameMouseMove);
	glutPassiveMotionFunc(GameMouseMove);

	glutIdleFunc(GameLoop);										// Process user input and does rendering
	setVSync(0);
	glutMainLoop();												// Go to GLUT main loop
}
