#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <vector>
#include <random>

#include "game.cpp"

using namespace std;

void display();
void reshape(int, int);
void animate(int);


vector<Bomb> bombs;
Player player = Player();


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Word Blaster");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, animate, 0);

    glClearColor(1, 1, 1, 1);

    for (int i = 0; i < 5; i++) {
        vector<float> point = getRandomPoint();
        Text newText("word", 0, 0);
        Bomb newBomb(2, newText, point[0], point[1]);
        bombs.push_back(newBomb);
    }

    glutMainLoop();

    return 0;
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // drawCircle(7.0f, 0.0f, -80.0f, 0, 360, GL_POLYGON);

    // text1.draw();
    // text2.draw();
    player.draw();

    // bomb1.draw();
    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].draw();
    }


    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    // text1.animate();
    // text2.animate();

    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].animate();
        if (bombs[i].isDone()) {
            bombs.erase(bombs.begin() + i);
            i--;
        }
    }
}



void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f) {
        gluOrtho2D(-100.0f * aspect, 100.0f * aspect, -100.0f, 100.0f);
    } else {
        gluOrtho2D(-100.0f, 100.0f, -100.0f / aspect, 100.0f / aspect);
    }

    glMatrixMode(GL_MODELVIEW);
}