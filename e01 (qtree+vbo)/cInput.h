#pragma once

#define KEY_NONE	0x00
#define KEY_LEFT	0x01
#define KEY_RIGHT	0x02
#define KEY_UP		0x04
#define KEY_DOWN	0x08

class cInput
{
public:
	cInput();
	~cInput();

	void Init(int window_Xo,int window_Yo,int window_W,int window_H);

	bool KeyIsDown(int k);
	void GetMousePosition(int *x,int *y);
	int  GetMouseX();
	int  GetMouseY();

	void SetKeyDown(int k);
	void SetKeyUp(int k);
	void SetMousePosition(int x,int y);
	void SetMouseX(int x);
	void SetMouseY(int y);
	void SetMouseNotUpdated();
	void CenterMouse();

	bool Updated();
	bool KeysUpdated();
	bool MouseUpdated();

private:
	bool keys[256];
	int  keys_updated;

	int  mouse_X, mouse_Y;
	bool mouse_updated;

	int  window_CX, window_CY;
};
