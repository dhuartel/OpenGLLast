
#include "cSkyBox.h"
#include <gl/glut.h>

#define GL_CLAMP_TO_EDGE	0x812F

cSkyBox::cSkyBox() {}
cSkyBox::~cSkyBox(){}

bool cSkyBox::LoadTextures()
{
	if(!tex_front.Load ("front.png" ,GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	if(!tex_back.Load  ("back.png"  ,GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	if(!tex_left.Load  ("left.png"  ,GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	if(!tex_right.Load ("right.png" ,GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	if(!tex_top.Load   ("top.png"   ,GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	if(!tex_bottom.Load("bottom.png",GL_RGB,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,true))	return false;
	return true;
}

void cSkyBox::CreateWithTexture(float x, float y, float z, float width, float height, float length)
{
	
	// Turn on texture mapping if it's not already
	glEnable(GL_TEXTURE_2D);

	// Bind the BACK texture of the sky map to the BACK side of the cube
	glBindTexture(GL_TEXTURE_2D, tex_back.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// This centers the sky box around (x, y, z)
	x = x - (width  / 2);
	y = y - (height / 2);
	z = z - (length / 2);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		
	glEnd();
	
	// Bind the FRONT texture of the sky map to the FRONT side of the box
	glBindTexture(GL_TEXTURE_2D, tex_front.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);	
	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
	glBindTexture(GL_TEXTURE_2D, tex_bottom.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
	
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// Bind the TOP texture of the sky map to the TOP side of the box
	glBindTexture(GL_TEXTURE_2D, tex_top.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the TOP Side
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();

	// Bind the LEFT texture of the sky map to the LEFT side of the box
	glBindTexture(GL_TEXTURE_2D, tex_left.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	// Bind the RIGHT texture of the sky map to the RIGHT side of the box
	glBindTexture(GL_TEXTURE_2D, tex_right.GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		

		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}

void cSkyBox::Create(float x, float y, float z, float width, float height, float length)
{
	// This centers the sky box around (x, y, z)
	x = x - (width  / 2);
	y = y - (height / 2);
	z = z - (length / 2);

	glBegin(GL_QUADS);		
		glVertex3f(x + width, y,			z);
		glVertex3f(x + width, y + height, z); 
		glVertex3f(x,			y + height, z);
		glVertex3f(x,			y,			z);
	glEnd();
	glBegin(GL_QUADS);	
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y + height, z + length);
		glVertex3f(x + width, y + height, z + length); 
		glVertex3f(x + width, y,			z + length);
	glEnd();
	glBegin(GL_QUADS);		
		glVertex3f(x,			y,			z);
		glVertex3f(x,			y,			z + length);
		glVertex3f(x + width, y,			z + length); 
		glVertex3f(x + width, y,			z);
	glEnd();
	glBegin(GL_QUADS);		
		glVertex3f(x + width, y + height, z);
		glVertex3f(x + width, y + height, z + length); 
		glVertex3f(x,			y + height,	z + length);
		glVertex3f(x,			y + height,	z);
	glEnd();
	glBegin(GL_QUADS);		
		glVertex3f(x,			y + height,	z);	
		glVertex3f(x,			y + height,	z + length); 
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y,			z);		
	glEnd();
	glBegin(GL_QUADS);		
		glVertex3f(x + width, y,			z);
		glVertex3f(x + width, y,			z + length);
		glVertex3f(x + width, y + height,	z + length); 
		glVertex3f(x + width, y + height,	z);
	glEnd();
}
void cSkyBox::CreateDL()
{
	int c = MAP_SIZE>>1;
	int lim = MAP_SIZE+32;
	
	if(id == -1) id = glGenLists(1);

	glNewList(id,GL_COMPILE);
		Create(c,c,-c,lim,lim,lim);
	glEndList();
}
void cSkyBox::Render()
{
	GLboolean lights;
	lights = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	glColor3f(1.0f,1.0f,1.0f);
	glCallList(id);
	
	if(lights) glEnable(GL_LIGHTING);
}
