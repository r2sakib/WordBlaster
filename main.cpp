#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <string>

#include "objects.cpp"

using namespace std;

void display();
void reshape(int, int);
void animate(int);


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Word Blaster");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, animate, 0);

    glClearColor(0, 0, 0, 1);

    glutMainLoop();

    return 0;
}


Text text1("Hello World", 50, 90);
Text text2("Hello", -50, 90);

Text text("word", 0, 0);
Bomb bomb1(3, text, -50, 80);


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();


    text1.draw();
    text2.draw();

    bomb1.draw();


    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    // text1.animate();
    // text2.animate();

    bomb1.animate();
}



void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f) {
        // Wide window
        gluOrtho2D(-100.0f * aspect, 100.0f * aspect, -100.0f, 100.0f);
    } else {
        // Tall window
        gluOrtho2D(-100.0f, 100.0f, -100.0f / aspect, 100.0f / aspect);
    }

    glMatrixMode(GL_MODELVIEW);
}