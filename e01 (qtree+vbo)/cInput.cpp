#include "cInput.h"
#include <windows.h>
#include <gl/glut.h>

cInput::cInput() {}
cInput::~cInput(){}

void cInput::Init(int window_Xo,int window_Yo,int window_W,int window_H)
{
	//Keys
	keys_updated = KEY_NONE;
	for(int i=0;i<256;i++)	keys[i] = false;
	
	//Mouse
	window_CX = window_Xo + ( window_W >> 1 );
	window_CY = window_Yo + ( window_H >> 1 );
	mouse_updated = false;
	mouse_X = window_CX;
	mouse_Y = window_CY;
	CenterMouse();
	glutSetCursor(GLUT_CURSOR_NONE);
}
bool cInput::KeyIsDown(int k)
{
	return keys[k];
}
void cInput::GetMousePosition(int *x,int *y)
{
	*x = mouse_X;
	*y = mouse_Y;
}
void cInput::SetKeyDown(int k)
{
	keys[k] = true;
	switch(k)
	{
		case GLUT_KEY_LEFT:		keys_updated ^= KEY_LEFT;	break;
		case GLUT_KEY_RIGHT:	keys_updated ^= KEY_RIGHT;	break;
		case GLUT_KEY_UP:		keys_updated ^= KEY_UP;		break;
		case GLUT_KEY_DOWN:		keys_updated ^= KEY_DOWN;	break;
	}
}
void cInput::SetKeyUp(int k)
{
	keys[k] = false;
	switch(k)
	{
		case GLUT_KEY_LEFT:		keys_updated ^= KEY_LEFT;	break;
		case GLUT_KEY_RIGHT:	keys_updated ^= KEY_RIGHT;	break;
		case GLUT_KEY_UP:		keys_updated ^= KEY_UP;		break;
		case GLUT_KEY_DOWN:		keys_updated ^= KEY_DOWN;	break;
	}
}
void cInput::SetMousePosition(int x,int y)
{
	mouse_X = x;
	mouse_Y = y;

	if(mouse_X != window_CX || mouse_Y != window_CY)
	{
		mouse_updated = true;
	}
}
void cInput::SetMouseNotUpdated()
{
	mouse_updated = false;
}
void cInput::CenterMouse()
{
	//if(!(mouse_X == window_CX && mouse_Y == window_CY))
	SetCursorPos(window_CX,window_CY);
}
int cInput::GetMouseX()
{
	return mouse_X;
}
int cInput::GetMouseY()
{
	return mouse_Y;
}
bool cInput::KeysUpdated()
{
	return (keys_updated > 0);
}
bool cInput::MouseUpdated()
{
	return mouse_updated;
}
bool cInput::Updated()
{
	return (KeysUpdated() || MouseUpdated());
}
