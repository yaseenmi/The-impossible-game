#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "glut.h"
GLUquadric* a = gluNewQuadric();
float move=1.0;
float theta = 0;
float xi = 0, yi = 1, zi = 5;
float lx = 0, ly = 0, lz = -1;
float speed = 0.5;
float sphereTheta = 0;
float sphereThetaX = 0;
float si = 0;
GLuint  _textureSky, _textureFinish,_texturetrack,_textureball,_texturecube,_texturewall;
char *texels;
int width, height;
float track;
float sp = 0.5;
bool isjumping = false;
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

GLuint loadTexture(char* imagePath)
{
	readBmp(imagePath);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);
	return textureId;
}

void specialkey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (!isjumping)
		{
			si += 1.2;
			isjumping = true;
		}

		break;

	case GLUT_KEY_RIGHT:
		xi -= move;
		sphereThetaX -= 5;
		break;

	case GLUT_KEY_LEFT:
		xi += move;
		sphereThetaX += 5;
		break;
	}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '`': /* Escape key */
		exit(0);
		break;

	case 'w': // for testing

	/*	si += 1;
		if (si >= 1){
			si = 1;
		}*/
		break;
	case 'd': // MOVE right
			xi -= move;
			sphereThetaX -= 5;
		break;
	case ' ': 	//jumping	
		if (!isjumping )
		{
			si += 1.2;
			isjumping = true;
		}
		
		break;
	case 'a': // MOVE left				
			xi += move;
			sphereThetaX += 5;
		break;
	case 'x': // UP
		yi += 0.2;
		break;
	case 'z': // DOWN
		yi -= 0.2;
		break;
	case 'f': // INCREASE SPEED
		speed += 0.2;
		break;
	case 'g': // DECREASE SPEED
		speed -= 0.2;
		break;

	default:
		break;
	}
}
bool detectcollision(int x1, int y1, int z1, int radis1, int x2, int y2, int z2, int radis2)
{
	int xd, yd, zd, Distance;

		//3d
		xd = x2 - x1;
	yd = y2-y1;
	zd = z2 - z1;
	Distance = sqrt(xd*xd + yd*yd + zd*zd);

	if (radis1 + radis2 >= Distance)
		 xi = 0, yi = 1, zi = -10;
	 lx = 0, ly = 0, lz = -1;
		//exit(0);		

		return true; //collision
	return false;    //no collision    
}
int i = 0;
bool round2(int x1, int y1, int z1, int radis1, int x2, int y2, int z2, int radis2)
{
	int xd, yd, zd, Distance;
	
	//3d
	xd = x2 - x1;
	yd = y2 - y1;
	zd = z2 - z1;
	Distance = sqrt(xd*xd + yd*yd + zd*zd);

	


		if (radis1 + radis2 >= Distance)
		{
			
				if (i != 1){


					xi = 0, yi = 1, zi = -15;
					lx = 0, ly = 0, lz = -1;
					speed = 0.2;

					GLfloat mat_specular[] = { 1, 1, 1, 0.0 };
					GLfloat mat_shininess[] = { 3.0 };
					GLfloat light_position[] = { -510, -510, -510, 1 };

					glClearColor(0.0, 0.0, 0.0, 0.0);
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
					glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
					glLightfv(GL_LIGHT0, GL_POSITION, light_position);


					glEnable(GL_LIGHTING);
					glEnable(GL_LIGHT0);
					_texturetrack = loadTexture("bricks.bmp");
					_textureball = loadTexture("finish.bmp");
					_texturewall = loadTexture("grass1.bmp");


					std::cout << i << std::endl;
					i++;
					return true; //collision

				}

				
				exit(0);
		
	}
	
	return false;    //no collision    
}

void animate(void)
{
	glutPostRedisplay();
}
void InitGL(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	_textureSky = loadTexture("sky.bmp");
	_texturetrack = loadTexture("track.bmp");
	_textureball = loadTexture("bricks.bmp");
	_texturecube = loadTexture("download.bmp");
	_textureFinish = loadTexture("win.bmp");
	_texturewall = loadTexture("downloa.bmp");
}

void ReSizeGLScene(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);	// for 3d
	//gluOrtho2D(-10, 10, -10, 10);//for 2d
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DrawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	if (si>0 && isjumping){
		si -= 0.01;
	}
	if (si <= 0)
		isjumping = false;

	// Sky
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureSky);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTranslatef(0, 0, -10);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 1.0, 0.1);		glVertex3f(-10, 10, 0);
	glTexCoord3f(1.0, 1.0, 0.1);		glVertex3f(10, 10, 0);
	glTexCoord3f(1.0, 0.0, 0.1);		glVertex3f(10, -10, 0);
	glTexCoord3f(0.0, 0.0, 0.1);		glVertex3f(-10, -10, 0);
	glEnd();
	glPopMatrix();

	gluLookAt(xi, yi, zi, xi + lx, yi, zi + lz+90, 0, 1, 0);
	
	
	glBindTexture(GL_TEXTURE_2D, _texturewall);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0, 0.0, 0);		glVertex3f(-2.0, 0, 0);
	glTexCoord3f(2.0, 0.0, 0);		glVertex3f(-2.0, -2.5, 0);
	glTexCoord3f(2.0, 70.0, 0);		glVertex3f(-2.0, -2.5, 970);
	glTexCoord3f(0.0, 70.0, 0);		glVertex3f(-2.0, 0, 970);
	glEnd();
	detectcollision(xi, si, zi, 0.2, -3.9, 0, zi, 1.5);
	glPopMatrix();
	

	glBindTexture(GL_TEXTURE_2D, _texturewall);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPushMatrix();
	glTranslatef(0, -0.9, 0);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0, 0.0, 0);		glVertex3f(2.0, 0, 0);
	glTexCoord3f(2.0, 0.0, 0);		glVertex3f(2.0, 2.5, 0);
	glTexCoord3f(2.0, 70.0, 0);		glVertex3f(2.0, 2.5, 970);
	glTexCoord3f(0.0, 70.0, 0);		glVertex3f(2.0, 0, 970);
	detectcollision(xi, si, zi, 0.2, 3.9 ,0 ,zi, 1.5);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, _texturewall);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0, 0.0, 0);		glVertex3f(2.0, 0, 0);
	glTexCoord3f(2.0, 0.0, 0);		glVertex3f(-2.0, 0, 0);
	glTexCoord3f(2.0, 70.0, 0);		glVertex3f(-2.0, 0, 970);
	glTexCoord3f(0.0, 70.0, 0);		glVertex3f(2.0, 0, 970);
	glEnd();
	glPopMatrix();

	
	//finish
				glPushMatrix();			
			glBindTexture(GL_TEXTURE_2D, _textureFinish);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTranslated(0, 0, 970);
			glBegin(GL_POLYGON);
			glTexCoord3f(0.0, 0.0, 0);		glVertex3f(2, -0.5, 0);
			glTexCoord3f(0.20, 0.0, 0);		glVertex3f(2, 0.5, 0);
			glTexCoord3f(0.20, 0.70, 0);		glVertex3f(-2, 0.5, 0);
			glTexCoord3f(0.0, 0.70, 0);		glVertex3f(-2, -0.5, 0);
			glEnd();
	
			
				round2(xi, si, zi, 0.2, 0, 0, 970, 4);

			
			glPopMatrix();
			//track
		glBindTexture(GL_TEXTURE_2D, _texturetrack);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glPushMatrix();
		glTranslatef(0, 1.5, 0);
		glBegin(GL_QUADS);
		glTexCoord3f(0.0, 0.0, 0);		glVertex3f(-2, -2.0, 5);
		glTexCoord3f(2.0, 0.0, 0);		glVertex3f(2.0, -2, 5);
		glTexCoord3f(2.0, 70.0, 0);		glVertex3f(2, -2, 970);
		glTexCoord3f(0.0, 70.0, 0);		glVertex3f(-2, -2, 970);
		glEnd();
		glPopMatrix();
	

		//cube

	glPushMatrix();		
	float x;
	float z;
	float y=0;
	int j=6;
		for (int i = 0; i < 30; i++){
			x = 0;
			z = 30;
		srand(j);
		glTranslatef(x, y, z);
		glutSolidCube(0.8);
		j++;
}
		for (int k = 0; k < 30; k++){
			detectcollision(xi, si, zi, 0.2, x, y, z - 4,0.4);
			
			z += 30;
		}
		glPopMatrix();
		
		glPushMatrix();
		float n;
		float m;
		int s = 6;
		n = 1.5;
		for (int c = 0; c < 38; c++){
			
			m = 25;
			if (n >= 1.5){
				n = 1.5;
				n = n - 2;
			}
			else if (n <= -0.5){
				n = -0.5;
				n = n + 2;
			}
			srand(s);
			glTranslatef(n-0.5, y, m);
			glutSolidCube(0.8);
			s++;
		}
		for (int b = 0; b < 38; b++){
			if (n <= 1.5){
				detectcollision(xi, si, zi, 0.2, n - 3, y, m - 5, 0.1);
				m += 25;
			}
			if (n >=-0.5){
				detectcollision(xi, si, zi, 0.2, n-2, y, m - 4, 0.1);
				m += 25;
			}
		}
		glPopMatrix();




		glPushMatrix();
		float h;
		float l;
		int o = 6;
			h = -1.5;
			for (int c = 0; c < 30; c++){

				l = 32;
				if (h >= 1.5){
					h = 1.5;
					h = h - 2;
				}
				else if (h <= -0.5){
					h = -0.5;
					h = h + 2;
				}
				srand(o);
				glTranslatef(h - 0.5, y, l);
				glutSolidCube(0.8);
				o++;


				for (int b = 0; b < 30; b++){
						if (h <= -0.5){
							detectcollision(xi, si, zi, 0.2, h + 2, y, l - 4, 0.1);
							l += 64;
							}
					if (h >= 1.5){l += 32;
						detectcollision(xi, si, zi, 0.2, h-1, y, l-4, 0.1);
						l += 32;
					}

				}
			}
		glPopMatrix();


		//ball
		sphereTheta += 3.7;
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, _textureball);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			GLUquadricObj *a = gluNewQuadric();
			gluQuadricTexture(a, true);
			
			xi -= speed * lx;
			zi -= speed * lz;
			glTranslatef(xi, si, zi + 3);
			glRotatef(sphereTheta, 1, 0, 0);
			

			gluSphere(a, 0.2, 30, 16);
			glPopMatrix();
	

	gluDeleteQuadric(a);


	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE); // Use single display buffer.
	glutInitWindowSize(2000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("EMPTY WINDOW");
	glutSetCursor(GLUT_CURSOR_NONE);
	InitGL();
	glutIdleFunc(animate);
	glutReshapeFunc(ReSizeGLScene);
	glutDisplayFunc(DrawGLScene);
	glutSpecialFunc(specialkey);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}




