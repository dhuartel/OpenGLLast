
#pragma once

#include "cTexture.h"

class cSkyBox  
{
public:
	
	cSkyBox();
	virtual ~cSkyBox();

	bool LoadTextures();
	void CreateDL();
	void Render();

private:

	void CreateWithTexture(float x, float y, float z, float width, float height, float length);
	void Create(float x, float y, float z, float width, float height, float length);

	cTexture tex_top,tex_bottom,tex_right,tex_left,tex_front,tex_back;

	int id;
};

