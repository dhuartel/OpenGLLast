#include "cRect.h"
#include <gl/glut.h>

cRect::cRect() {}
cRect::~cRect(){}

void cRect::Render(int Xo,int Zo,int Xf,int Zf,float r,float g,float b,int h)
{
	bool ilu = false;
	if(glIsEnabled(GL_LIGHTING))
	{
		ilu = true;
		glDisable(GL_LIGHTING);
	}
	
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		glVertex3f(Xo,h,-Zo);
		glVertex3f(Xf,h,-Zo);
		glVertex3f(Xf,h,-Zf);
		glVertex3f(Xo,h,-Zf);
	glEnd();

	if(ilu)	glEnable(GL_LIGHTING);
}
void cRect::RenderTex(int Xo,int Zo,int Xf,int Zf,float r,float g,float b,int h,int id,bool blend)
{
	bool ilu = false;
	if(glIsEnabled(GL_LIGHTING))
	{
		ilu = true;
		glDisable(GL_LIGHTING);
	}
	
	if(blend)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.05f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,id);
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);	glVertex3f(Xo,h,-Zo);
		glTexCoord2f(1,0);	glVertex3f(Xf,h,-Zo);
		glTexCoord2f(1,-1);	glVertex3f(Xf,h,-Zf);
		glTexCoord2f(0,-1);	glVertex3f(Xo,h,-Zf);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	if(blend)
	{
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	if(ilu)	glEnable(GL_LIGHTING);
}
