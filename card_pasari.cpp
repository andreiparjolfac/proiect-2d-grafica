#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include<windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include < random >
using namespace std;

double screenX = 800.0;
double screenY = 600.0;



class Pasare {
public:
	double offsetx, offsety;
	Pasare(double spawnX, double spawnY) {
		offsetx = spawnX;
		offsety = spawnY;
	}

	void deseneaza_pasare(void) {

		glColor3d(0.0, 0.0, 0.0);

		glBegin(GL_POLYGON);
		glVertex2i(offsetx + 15, offsety + 20);
		glVertex2i(offsetx + 15, offsety + 15);
		glVertex2i(offsetx + 5, offsety);
		glVertex2i(offsetx, offsety);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex2i(offsetx + 15, offsety + 15);
		glVertex2i(offsetx + 20, offsety + 15);
		glVertex2i(offsetx + 20, offsety + 20);
		glVertex2i(offsetx + 15, offsety + 20);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex2i(offsetx + 20, offsety + 15);
		glVertex2i(offsetx + 35, offsety);
		glVertex2i(offsetx + 40, offsety);
		glVertex2i(offsetx + 20, offsety + 20);
		glEnd();
	
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
			rx = 2* (dis(generator)) - 1;
			ry = 2* (dis(generator)) - 1;
			pasari.push_back(new Pasare( coordX + rx*raza_card, coordY+ry*raza_card));
		}
	}
};

Card c1(7, 30);

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0,screenX, 0.0,screenY);
}

void deseneaza(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

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
/*

int main() {

	srand(time(NULL));
	cout << "coordonatele cardului" << endl;
	cout << c1.coordX << " " << c1.coordY << endl;
	for (int i = 0; i < c1.pasari.size(); i++) {
		cout << c1.pasari[i]->offsetx<<"  " << c1.pasari[i]->offsety<<endl;
	}
	return 0;

}
*/
