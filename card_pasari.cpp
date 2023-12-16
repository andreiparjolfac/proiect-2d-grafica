#include <iostream>
#include <cmath>
#include <vector>
#include <windows.h>
#include <GL/freeglut.h>
#include <random>
#include <SOIL.h>

#define M_PI (3.141592653589793238462643383279502884L)

using namespace std;

double screenX = 800.0;
double screenY = 600.0;

double viteza_animatie = 1.0 / 50.0;
static GLdouble i = 0.0;
static GLdouble j = 0.0;
static GLdouble rotationAngle = 0.0f;
static GLdouble destX;
static GLdouble destY;

static GLdouble dx;
static GLdouble dy;

GLuint texturiPasare[2],fundalTexturaID;
int pozAripiPasare = 0;

random_device rd;
mt19937 generator(rd());
uniform_real_distribution<double> dis(0.0, 1.0);


bool rightOrientation = true;

class Pasare {
public:
    double offsetx, offsety;
    bool zboara = false;
    double currentRotation = 0.0;
    double targetRotation = 0.0;
    int startTime = 0;

    Pasare(double spawnX, double spawnY) {
        offsetx = spawnX;
        offsety = spawnY;
    }

    void deseneaza_pasare(void) {
        if (zboara) {
            offsetx += dx;
            offsety += dy;
        }

        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        double elapsedSeconds = static_cast<double>(currentTime - startTime) / 10000.0;

        if (elapsedSeconds > 0.0) {
            double interpolationFactor = min(1.0, elapsedSeconds / 1.0);
            currentRotation = rotationAngle * interpolationFactor + currentRotation * (1.0 - interpolationFactor);
        }

        glPushMatrix();

        glTranslated(offsetx + 20, offsety + 20, 0.0);
        glRotated(currentRotation, 0.0, 0.0, 1.0);

        if (!rightOrientation) {
            glScalef(-1.0, 1.0, 1.0);
        }

        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturiPasare[pozAripiPasare]);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2i(-20, -20);
        glTexCoord2f(1.0, 0.0); glVertex2i(20, -20);
        glTexCoord2f(1.0, 1.0); glVertex2i(20, 20);
        glTexCoord2f(0.0, 1.0); glVertex2i(-20, 20);
        glEnd();

        glPopMatrix();
        glLoadIdentity();
        glDisable(GL_TEXTURE_2D);
    }

};

class Card {
public:
    vector<Pasare*> pasari;
    double coordX, coordY, raza_card;
    Card(int nr_pasari, double raza_card) {
        double rx = dis(generator);
        double ry = dis(generator);
        this->raza_card = raza_card;
        coordX = raza_card * (1 - rx) + rx * (screenX - raza_card);
        coordY = raza_card * (1 - ry) + ry * (screenY - raza_card);
        for (int i = 0; i < nr_pasari; i++) {
            rx = 2 * (dis(generator)) - 1;
            ry = 2 * (dis(generator)) - 1;
            pasari.push_back(new Pasare(coordX + rx * raza_card, coordY + ry * raza_card));
        }
    }

    void zboara_la(GLdouble X, GLdouble Y) {
        destX = X;
        destY = Y;

        if (destX - coordX < 0)
            rightOrientation = false;
        else
            rightOrientation = true;

        for (int i = 0; i < pasari.size(); i++) {
            pasari[i]->zboara = true;
            pasari[i]->startTime = glutGet(GLUT_ELAPSED_TIME);
            pasari[i]->targetRotation = atan2(destY - pasari[i]->offsety, destX - pasari[i]->offsetx) * (180.0 / M_PI);
        }
    }
};

Card c1(7, 60);

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    gluOrtho2D(0.0, screenX, 0.0, screenY);
    texturiPasare[0] = SOIL_load_OGL_texture("pasareSus.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texturiPasare[1] = SOIL_load_OGL_texture("pasareJos.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    fundalTexturaID = SOIL_load_OGL_texture("fundal.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void deseneazaFundal() {
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

void deseneaza(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    deseneazaFundal();
    glLoadIdentity();

    for (int i = 0; i < c1.pasari.size(); i++) {
        c1.pasari[i]->deseneaza_pasare();
    }

    glutSwapBuffers();

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, screenX, 0.0, screenY, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void miscas(void) {

    double lastDy = dy;

    if (abs(c1.coordX + i - destX) < 5) {
        dx = 0;
    }
    else {
        if (destX > c1.coordX) {
            dx = viteza_animatie;
        }
        else {
            dx = -viteza_animatie;
        }
    }

    if (abs(c1.coordY + j - destY) < 5) {
        dy = 0;
    }
    else {
        if ((destY < c1.coordY and destX < c1.coordX) or (destY > c1.coordY and destX < c1.coordX))
        {
            dy = -1 * ((destY - c1.coordY) / (destX - c1.coordX) + 3 * cos(-i / 3)) * viteza_animatie;
        }
        if ((destY < c1.coordY and destX > c1.coordX) or (destY > c1.coordY and destX > c1.coordX)) {
            dy = ((destY - c1.coordY) / (destX - c1.coordX) + 3 * cos(i / 3)) * viteza_animatie;
        }
    }

    if (abs(c1.coordY + j - destY) < 5 and abs(c1.coordX + i - destX) < 5) {
        c1.coordX += i;
        c1.coordY += j;
        i = 0;
        j = 0;
        //rotationAngle = 0;
    }

    if ((lastDy > 0 && dy < 0) || (lastDy < 0 && dy > 0))
        pozAripiPasare = 1 - abs(pozAripiPasare);

    i += dx;
    j += dy;

    
    double angleToMouse = atan2(destY - c1.coordY, destX - c1.coordX);
    //rotationAngle = angleToMouse * (180.0 / M_PI);

    if (dx < 0)
    {
        rotationAngle = 180 + angleToMouse * (180.0 / M_PI);
    }
    else
    {
        if(dx > 0)
            rotationAngle = fmod(angleToMouse * (180.0 / M_PI) + 360, 360);
    }
   
    cout << rotationAngle << endl;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            c1.zboara_la(x, screenY - y);
        glutIdleFunc(miscas);
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN and (abs(c1.coordX - x) < c1.raza_card) and (abs(c1.coordY - (screenY - y)) < c1.raza_card)) {
            c1.zboara_la(dis(generator) * screenX, dis(generator) * screenY);
            glutIdleFunc(miscas);
        }
        break;
    default:
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenX, screenY);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Card de Pasari");
    init();
    glutDisplayFunc(deseneaza);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
