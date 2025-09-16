#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <list>
#include <vector>
#include <random>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "game.cpp"

using namespace std;


void display();
void reshape(int, int);
void animate(int);
void keyboard(unsigned char key, int x, int y);
void sendBombs(int);
void loadLives();
void cleanup();
void resetGame();


Background background = Background();
Player player = Player();
Gun gun = Gun();
Word word = Word(WORDS_FILE_PATH);
GameOver gameOver = GameOver();
HomePage homepage = HomePage();


list<Bomb> bombs;
vector<Bullet> bullets;
vector<LifeHeart> lifeHearts;

string currentTypedStr;

ma_engine g_audioEngine;
ma_sound g_backgroundSound;


GameState currentState = HOME_SCREEN;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    // FULL SCREEN ON ALL DEVICES
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(screenWidth, screenHeight); 
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Word Blaster");

    glutFullScreen();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, animate, 0);
    glutKeyboardFunc(keyboard);

    // GAME INITIALIZATION
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    animate(0);
    word.loadWords();
    loadLives();

    // Audio init
    ma_result result = ma_engine_init(NULL, &g_audioEngine);
    if (result != MA_SUCCESS) {
        cout << "Failed to initialize audio engine." << endl;
        return -1;
    }

    ma_sound_init_from_file(&g_audioEngine, SOUND_BACKGROUND, MA_SOUND_FLAG_STREAM, NULL, NULL, &g_backgroundSound);
    ma_sound_start(&g_backgroundSound);
    ma_sound_set_looping(&g_backgroundSound, MA_TRUE);
    atexit(cleanup);
    
    glutMainLoop();

    return 0;
}


void sendBombs(int) {
    if (currentState != PLAYING)
        return;

    for (int i = 0; i < NUM_OF_BOMBS_PER_WAVE; i++) {
        vector<float> point = getRandomPoint();
        Text newWord(word.getRandomWord(bombs), 0, 0);
        
        Bomb newBomb(BOMB_RADIUS, newWord, point[0], point[1]);
        bombs.push_back(newBomb);
    }

    if (NUM_OF_BOMBS_PER_WAVE < 10)
        NUM_OF_BOMBS_PER_WAVE++;


    glutTimerFunc(BOMB_WAVE_TIME_SEC*1000, sendBombs, 0);
}


void loadLives() {
    vector<vector<float>> lifeStarPositions = {
        {157, 94}, {164, 94}, {171, 94}
    };

    for (auto lifeStarPosition : lifeStarPositions) {
        LifeHeart newLifeHeart(lifeStarPosition[0], lifeStarPosition[1]);
        lifeHearts.push_back(newLifeHeart);
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (currentState == HOME_SCREEN) {
        homepage.draw();
        glutSwapBuffers();

        return;
    }

    background.draw();
    
    for (auto& bomb : bombs) {
        bomb.draw();
    }
    
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].draw();
    }

    // Decrement life
    if (livesLeft <= 3) {
    for (int i = TOTAL_LIVES-1; i >= TOTAL_LIVES-livesLeft; i--) {
        lifeHearts[i].draw();
    }
    }
    
    
    gun.draw();
    player.draw();

    PixelArtText liveScoreTxt = PixelArtText(0.7);
    liveScoreTxt.setColor(255, 0, 0);
    liveScoreTxt.draw(to_string(score), -173, 95);


    if (currentState == GAME_OVER) {
        ma_sound_stop(&g_backgroundSound);
        gameOver.draw(&g_audioEngine);
    }

    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);

    if (currentState == HOME_SCREEN) {
        homepage.draw();
        return;
    }


    if (livesLeft == 0) {
        player.dead = true;
        currentState = GAME_OVER;
    }

    if (livesLeft > 0) {
        for (auto it = bombs.begin(); it != bombs.end(); ) {
            it->animate(&g_audioEngine);
            if (it->isDone()) {
                it = bombs.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->animate();
            if (!it->active) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }

}


void keyboard(unsigned char key, int x, int y) {
    // Exit game only from home screen
    if (currentState == HOME_SCREEN && key == 27){
        ma_sound soundTransition;
        ma_sound_init_from_file(&g_audioEngine, SOUND_TRANSITION, 0, NULL, NULL, &soundTransition);
        ma_sound_start(&soundTransition);
        while (ma_sound_is_playing(&soundTransition)) {
            ma_sleep(1);
        }

        exit(0);
        return;
    }

    // Go to home screen from game and game over screen
    else if ((currentState == PLAYING || currentState == GAME_OVER) && key == 27) {
        ma_engine_play_sound(&g_audioEngine, SOUND_TRANSITION, NULL);
        resetGame();
        currentState = HOME_SCREEN;
        return;
    }

    // Start game (from home screen)
    else if (currentState == HOME_SCREEN && key == 13) {
        ma_engine_play_sound(&g_audioEngine, SOUND_TRANSITION, NULL);
        currentState = PLAYING;
        sendBombs(0);
        return;
    }

    // restart game
    else if (currentState == GAME_OVER && key == 13) {
        ma_engine_play_sound(&g_audioEngine, SOUND_TRANSITION, NULL);
        resetGame();
        return;
    }

    // Clear current typing word when backspace is pressed
    if (currentState == PLAYING && key == 8) {
        currentTypedStr = "";
        return;
    }
    
    currentTypedStr += key;
    for (Bomb &bomb : bombs) {
        if (bomb.text.str == currentTypedStr) {
            bullets.push_back(Bullet(&bomb, &player, &gun, &g_audioEngine));
            currentTypedStr = "";
            break;
        }
    }
    if (currentTypedStr.length() > 5)
        currentTypedStr = "";
    
    if (currentTypedStr == "infin") {
        livesLeft = 10000;
        currentTypedStr = "";
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


void cleanup() {
    ma_engine_uninit(&g_audioEngine);
}


void resetGame() {
    bombs.clear();
    bullets.clear();
    lifeHearts.clear();
    player.dead = false;
    currentTypedStr = "";
    livesLeft = TOTAL_LIVES;
    score = 0;
    
    sendBombs(0);
    loadLives();

    ma_sound_start(&g_backgroundSound);
    ma_sound_set_looping(&g_backgroundSound, MA_TRUE);
    
    currentState = PLAYING;
}