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


Player player = Player();
vector<Bomb> bombs;
Bullet bullet1;


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
    bullet1 = Bullet(&bombs[0]);

    glutMainLoop();

    return 0;
}


// Text newText("word", 0, 0);
// Bomb bomb(2, newText, -80, 80);
// Bullet bullet1(&bombs[0]);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    player.draw();
    // bomb.draw();
    bullet1.draw();

    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].draw();
    }


    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].animate();
        if (bombs[i].isDone()) {
            bombs.erase(bombs.begin() + i);
            i--;
        }
    }
    // bomb.animate();
    bullet1.animate();
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