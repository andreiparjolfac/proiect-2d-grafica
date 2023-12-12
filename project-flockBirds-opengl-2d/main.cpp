#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <random>
#include <SOIL.h>
using namespace std;

double screenX = 800.0;
double screenY = 600.0;

GLuint texturaID, fundalTexturaID;

class Pasare {
public:
	double offsetx, offsety;
	Pasare(double spawnX, double spawnY) {
		offsetx = spawnX;
		offsety = spawnY;
	}

	void deseneaza_pasare(void) {

		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texturaID);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2i(offsetx, offsety);
		glTexCoord2f(1.0, 0.0); glVertex2i(offsetx + 40, offsety);
		glTexCoord2f(1.0, 1.0); glVertex2i(offsetx + 40, offsety + 40);
		glTexCoord2f(0.0, 1.0); glVertex2i(offsetx, offsety + 40);
		glEnd();

		glDisable(GL_TEXTURE_2D);

	}
};

class Card {
public:
	vector<Pasare*> pasari;
	double coordX, coordY;
	Card(int nr_pasari, double raza_card) {
		random_device rd;
		mt19937 generator(rd());
		uniform_real_distribution<double> dis(0.0, 1.0);
		double rx = dis(generator);
		double ry = dis(generator);
		coordX = raza_card * (1 - rx) + rx * (screenX - raza_card);
		coordY = raza_card * (1 - ry) + ry * (screenY - raza_card);
		for (int i = 0; i < nr_pasari; i++) {
			rx = 2 * (dis(generator)) - 1;
			ry = 2 * (dis(generator)) - 1;
			pasari.push_back(new Pasare(coordX + rx * raza_card, coordY + ry * raza_card));
		}
	}
};

Card c1(7, 60);

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0, screenX, 0.0, screenY);
	texturaID = SOIL_load_OGL_texture("pasareSus.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	fundalTexturaID = SOIL_load_OGL_texture("fundal.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void deseneazaFundal()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fundalTexturaID);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(screenX, 0);
	glTexCoord2f(1, 1); glVertex2f(screenX, screenY);
	glTexCoord2f(0, 1); glVertex2f(0, screenY);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void deseneaza(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	deseneazaFundal();

	for (int i = 0; i < c1.pasari.size(); i++) {
		c1.pasari[i]->deseneaza_pasare();
	}


	glPopMatrix();
	glutSwapBuffers();

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Card de Pasari");
	init();
	glutDisplayFunc(deseneaza);

	//glutMouseFunc(mouse);
	glutMainLoop();
}