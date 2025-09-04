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
void sendBombs(int);
void loadLives();


Background background = Background();
Player player = Player();
Gun gun = Gun();
Word word = Word(WORDS_FILE_PATH);
GameOver gameOver = GameOver();


vector<Bomb> bombs;
vector<Bullet> bullets;
vector<LifeStar> lifeStars;

string currentTypedStr;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Word Blaster");
    glutFullScreen();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, animate, 0);
    glutKeyboardFunc(keyboard);
    animate(0);

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);


    word.loadWords();
    loadLives();
    sendBombs(0);

    glutMainLoop();

    return 0;
}

void sendBombs(int) {
    if (livesLeft == 0)
        return;

    for (int i = 0; i < 5; i++) {
        vector<float> point = getRandomPoint();
        Text newWord(word.getRandomWord(bombs), 0, 0);
        Bomb newBomb(2, newWord, point[0], point[1]);
        bombs.push_back(newBomb);
    }

    glutTimerFunc(BOMB_WAVE_TIME_SEC*1000, sendBombs, 0);
}


void loadLives() {
    vector<vector<float>> lifeStarPositions = {
        {160, 95}, {166, 95}, {172, 95}
    };

    for (auto lifeStarPosition : lifeStarPositions) {
        LifeStar newLifeStar(lifeStarPosition[0], lifeStarPosition[1]);
        lifeStars.push_back(newLifeStar);
    }
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

    // Decrement life
    for (int i = TOTAL_LIVES-1; i >= TOTAL_LIVES-livesLeft; i--) {
        lifeStars[i].draw();
    }
    
    
    gun.draw();
    player.draw();

    // Draw Score
    Text scoreTxt(to_string(score), -175, 93, GLUT_BITMAP_TIMES_ROMAN_24, 255, 0, 0);
    scoreTxt.draw();

    if (player.dead) {
        gameOver.draw();
    }


    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    if (livesLeft == 0) {
        player.dead = true;
    }

    if (livesLeft > 0) {
        for (int i = 0; i < bombs.size(); i++) {
            bombs[i].animate();
            if (bombs[i].isDone()) {
                bombs.erase(bombs.begin() + i);
                i--;
            }
        }

        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].animate();
            if (!bullets[i].active) {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    }
}


void keyboard(unsigned char key, int x, int y) {
    // Exit full screen
    if (key == 27){
        exit(0);
        return;
    }

    // Go to next word when space is pressed
    if (key == 32) {
        currentTypedStr = "";
        return;
    }
    
    currentTypedStr += key;
    for (Bomb &bomb : bombs) {
        if (bomb.text.str == currentTypedStr) {
            currentTypedStr = "";
            Bullet newBullet(&bomb, &player, &gun);
            bullets.push_back(newBullet);
        }
    }
    if (currentTypedStr.length() > 5)
        currentTypedStr = "";
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