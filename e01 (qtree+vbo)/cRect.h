#pragma once

class cRect
{
public:
	cRect();
	~cRect();

	void Render(int Xo,int Zo,int Xf,int Zf,float r,float g,float b,int h);
	void RenderTex(int Xo,int Zo,int Xf,int Zf,float r,float g,float b,int h,int id,bool blend);
};
