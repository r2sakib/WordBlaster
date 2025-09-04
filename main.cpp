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
void keyboard(unsigned char key, int x, int y);


Background background = Background();
Player player = Player();
Gun gun = Gun();
Life life = Life();

vector<Bomb> bombs;
vector<Bullet> bullets;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Word Blaster");
    glutFullScreen();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, animate, 0);
    glutKeyboardFunc(keyboard);

    // glClearColor(1, 1, 1, 1);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    for (int i = 0; i < 5; i++) {
        vector<float> point = getRandomPoint();
        Text newText("word", 0, 0);
        Bomb newBomb(2, newText, point[0], point[1]);
        bombs.push_back(newBomb);
    }
    // bullet1 = Bullet(&bombs[0], &player);

    glutMainLoop();

    return 0;
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    background.draw();
    
    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].draw();
    }
    
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].draw();
    }

    
    gun.draw();
    player.draw();
    life.draw();


    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    for (int i = 0; i < bombs.size(); i++) {
        bombs[i].animate();
        // if (bombs[i].isDone()) {
        //     bombs.erase(bombs.begin() + i);
        //     i--;
        // }
    }

    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].animate();
        // if (!bullets[i].active) {
        //     bullets.erase(bullets.begin() + i);
        //     i--;
        // }
    }
    // bomb.animate();
    // bullet1.animate();
}


void keyboard(unsigned char key, int x, int y) {
    // Exit full screen
    if (key == 27) {
        exit(0);
    }

    else {
        if (bombs.size() > 0) {
            Bullet newBullet(&bombs[key-48], &player, &gun);
            bullets.push_back(newBullet);
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