#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glut.h"

//1- Enable Texture (Line 80)
//2- LoadTexture (Line 68)
//3- BindTexture (Line 72)
//4-

float _angle = 0.0;
GLuint _textureBrick, _textureDoor, _textureGrass, _textureRoof, _textureWindow, _textureSky;
char *texels;
int width, height;

void readBmp(char *filename)
{
	FILE *fd;
	fd = fopen(filename, "rb");
	if (fd == NULL)
	{
		printf("Error: fopen failed\n");
		return;
	}
	unsigned char header[54];
	// Read header
	fread(header, sizeof(unsigned char), 54, fd);
	// Capture dimensions
	width = *(int*)&header[18];
	height = *(int*)&header[22];
	int padding = 0;
	// Calculate padding
	while ((width * 3 + padding) % 4 != 0)
	{
		padding++;
	}
	// Compute new width, which includes padding
	int widthnew = width * 3 + padding;
	// Allocate memory to store image data (non-padded)
	texels = (char *)malloc(width * height * 3 * sizeof(char));
	if (texels == NULL)
	{
		printf("Error: Malloc failed\n");
		return;
	}
	// Allocate temporary memory to read widthnew size of data
	unsigned char* data = (unsigned char *)malloc(widthnew * sizeof (unsigned int));
	// Read row by row of data and remove padded data.
	for (int i = 0; i<height; i++)
	{
		// Read widthnew length of data
		fread(data, sizeof(unsigned char), widthnew, fd);
		// Retain width length of data, and swizzle RB component.
		// BMP stores in BGR format, my usecase needs RGB format
		for (int j = 0; j < width * 3; j += 3)
		{
			int index = (i * width * 3) + (j);
			texels[index + 0] = data[j + 2];
			texels[index + 1] = data[j + 1];
			texels[index + 2] = data[j + 0];
		}
	}
	free(data);
	fclose(fd);
}

GLuint loadTexture(char* image, int width, int height) 
{
	GLuint textureId;
	glGenTextures(1, &textureId); 
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);							
	return textureId;				
}

void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	// Sky
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, _textureSky);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-10);
		glBegin(GL_QUADS);
			glTexCoord3f(0.0,1.0,0.1);		glVertex3f(-10,	10,		0);
			glTexCoord3f(1.0,1.0,0.1);		glVertex3f(10,	10,		0);
			glTexCoord3f(1.0,0.0,0.1);		glVertex3f(10,	-10,	0);
			glTexCoord3f(0.0,0.0,0.1);		glVertex3f(-10,	-10,	0);
		glEnd();
	glPopMatrix();	
		
	// Grass
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, _textureGrass);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-6);
		glRotatef(_angle, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord3f(0.0,70.0,1);		glVertex3f(-50,	-1.5,	50);
			glTexCoord3f(0.0,0.0,-1);		glVertex3f(-50,	-1.5,	-50);
			glTexCoord3f(70.0,0.0,-1);		glVertex3f(50,	-1.5,	-50);
			glTexCoord3f(70.0,70.0,1);		glVertex3f(50,	-1.5,	50);
		glEnd();
	glPopMatrix();

	// Front side
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, _textureBrick);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-6);
		glRotatef(_angle, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);  // Wall
			glTexCoord3f(0.0,2.0,0.1);		glVertex3f(-2,	0,		1);
			glTexCoord3f(4.0,2.0,0.1);		glVertex3f(2,	0,		1);
			glTexCoord3f(4.0,0.0,0.1);		glVertex3f(2,	-1.5,	1);
			glTexCoord3f(0.0,0.0,0.1);		glVertex3f(-2,	-1.5,	1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, _textureRoof);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);  // Roof
			glTexCoord3f(0.0,2.0,0);		glVertex3f(-2.2,	0.5,	0);
			glTexCoord3f(4.0,2.0,0);		glVertex3f(2.2,		0.5,	0);
			glTexCoord3f(4.0,0.0,1.25);		glVertex3f(2.2,		-0.1,	1.25);
			glTexCoord3f(0.0,0.0,1.25);		glVertex3f(-2.2,	-0.1,	1.25);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, _textureDoor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);  // Door
			glTexCoord3f(0.0,1.0,1.0001);	glVertex3f(-0.3,	-0.4,	1.0001);
			glTexCoord3f(1.0,1.0,1.0001);	glVertex3f(0.3,		-0.4,	1.0001);
			glTexCoord3f(1.0,0.0,1.0001);	glVertex3f(0.3,		-1.5,	1.0001);
			glTexCoord3f(0.0,0.0,1.0001);	glVertex3f(-0.3,	-1.5,	1.0001);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, _textureWindow);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);  // Window Left
			glTexCoord3f(0.0,1.0,1.0001);	glVertex3f(-1.5,	-0.3,	1.0001);
			glTexCoord3f(1.0,1.0,1.0001);	glVertex3f(-0.75,	-0.3,	1.0001);
			glTexCoord3f(1.0,0.0,1.0001);	glVertex3f(-0.75,	-0.8,	1.0001);
			glTexCoord3f(0.0,0.0,1.0001);	glVertex3f(-1.5,	-0.8,	1.0001);
		glEnd();

		glBegin(GL_QUADS);  // Window Right
			glTexCoord3f(0.0,1.0,1.0001);	glVertex3f(1.5,		-0.3,	1.0001);
			glTexCoord3f(1.0,1.0,1.0001);	glVertex3f(0.75,	-0.3,	1.0001);
			glTexCoord3f(1.0,0.0,1.0001);	glVertex3f(0.75,	-0.8,	1.0001);
			glTexCoord3f(0.0,0.0,1.0001);	glVertex3f(1.5,		-0.8,	1.0001);
		glEnd();
	glPopMatrix();

	// Back side
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureBrick);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-6);
		glRotatef(_angle, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);  // Wall
			glTexCoord3f(0.0,2.0,-1);		glVertex3f(-2,0,-1);
			glTexCoord3f(4.0,2.0,-1);		glVertex3f(2,0,-1);
			glTexCoord3f(4.0,0.0,-1);		glVertex3f(2,-1.5,-1);
			glTexCoord3f(0.0,0.0,-1);		glVertex3f(-2,-1.5,-1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, _textureRoof);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);  // Roof
			glTexCoord3f(0.0,2.0,0);		glVertex3f(-2.2,0.5,0);
			glTexCoord3f(4.0,2.0,0);		glVertex3f(2.2,0.5,0);
			glTexCoord3f(4.0,0.0,-1.25);	glVertex3f(2.2,-0.1,-1.25);
			glTexCoord3f(0.0,0.0,-1.25);	glVertex3f(-2.2,-0.1,-1.25);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, _textureWindow);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);  // Window Left
			glTexCoord3f(0.0,1.0,-1.0001);	glVertex3f(-1.5,-0.3,-1.0001);
			glTexCoord3f(1.0,1.0,-1.0001);	glVertex3f(-0.75,-0.3,-1.0001);
			glTexCoord3f(1.0,0.0,-1.0001);	glVertex3f(-0.75,-0.8,-1.0001);
			glTexCoord3f(0.0,0.0,-1.0001);	glVertex3f(-1.5,-0.8,-1.0001);
		glEnd();

		glBegin(GL_QUADS);  // Window Right
			glTexCoord3f(0.0,1.0,1.0001);	glVertex3f(1.5,-0.3,-1.0001);
			glTexCoord3f(1.0,1.0,1.0001);	glVertex3f(0.75,-0.3,-1.0001);
			glTexCoord3f(1.0,0.0,1.0001);	glVertex3f(0.75,-0.8,-1.0001);
			glTexCoord3f(0.0,0.0,1.0001);	glVertex3f(1.5,-0.8,-1.0001);
		glEnd();
	glPopMatrix();

	// Right side
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, _textureBrick);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-6);
		glRotatef(_angle, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);  // Wall
			glTexCoord3f(0.0,2.0,1);		glVertex3f(2,0,1);
			glTexCoord3f(2.0,2.0,-1);		glVertex3f(2,0,-1);
			glTexCoord3f(2.0,0.0,-1);		glVertex3f(2,-1.5,-1);
			glTexCoord3f(0.0,0.0,1);		glVertex3f(2,-1.5,1);
		glEnd();

		glBegin(GL_TRIANGLES);  // Wall Upper
			glTexCoord3f(0.0,1.0,0);		glVertex3f(2,0.5,0);
			glTexCoord3f(1.0,0.0,1);		glVertex3f(2,0,1);
			glTexCoord3f(-1.0,0.0,-1);		glVertex3f(2,0,-1);
		glEnd();
	glPopMatrix();

	// Left side
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, _textureBrick);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTranslatef(0,0,-6);
		glRotatef(_angle, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);  // Wall
			glTexCoord3f(0.0,2.0,1);		glVertex3f(-2,0,1);
			glTexCoord3f(2.0,2.0,-1);		glVertex3f(-2,0,-1);
			glTexCoord3f(2.0,0.0,-1);		glVertex3f(-2,-1.5,-1);
			glTexCoord3f(0.0,0.0,1);		glVertex3f(-2,-1.5,1);
		glEnd();

		glBegin(GL_TRIANGLES);  // Wall Upper
			glTexCoord3f(0.0,1.0,0);		glVertex3f(-2,0.5,0);
			glTexCoord3f(1.0,0.0,1);		glVertex3f(-2,0,1);
			glTexCoord3f(-1.0,0.0,-1);		glVertex3f(-2,0,-1);
		glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

void mySpecialFunc(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
			_angle += 1;
			if (_angle > 360) _angle = 0.0;
			break;
		case GLUT_KEY_LEFT:
			_angle -= 1;
			if (_angle > 360) _angle = 0.0;
			break;
	}
	glutPostRedisplay();
}

void Initialize() 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	readBmp("sky.bmp");
	_textureSky = loadTexture(texels, width, height);

	readBmp("grass1.bmp");
	_textureGrass = loadTexture(texels, width, height);

	readBmp("bricks.bmp");
	_textureBrick = loadTexture(texels, width, height);


	readBmp("roof.bmp");
	_textureRoof = loadTexture(texels, width, height);

	readBmp("door.bmp");
	_textureDoor = loadTexture(texels, width, height);

	readBmp("window.bmp");
	_textureWindow = loadTexture(texels, width, height);
}

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv) 
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800,600);
	glutCreateWindow("Textured House");
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(resize);
	glutSpecialFunc(mySpecialFunc);
	glutDisplayFunc(renderScene);
	Initialize();
	glutMainLoop();
	return 0;
}