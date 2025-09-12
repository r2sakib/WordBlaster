#pragma once

#include <GL/glut.h>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include "miniaudio.h"

using namespace std;

#include "helperFunctions.cpp"



class Player {
    public:
        float x;
        float y;
        float eyesAngle;
        int dead = false;

        Player(float x=PLAYER_X, float y=PLAYER_Y, float eyesAngle=45) {
            this->x = x;
            this->y = y;
            this->eyesAngle = 3.0f * cos(eyesAngle);
        }

        void draw() {
            glPushMatrix();
            glTranslatef(x, y, 0.0f);

        
            // LEFT LEG
            glColor3ub(96, 57, 19);
            if (dead)
                glColor3ub(162, 162, 162);
            drawElipse(-4.35f, -7.0f, 3.0f, 1.4f, GL_POLYGON);

            glColor3ub(0,0,0);
            glLineWidth(3);
            drawElipse(-4.35f, -7.0f, 3.0f, 1.4f, GL_LINE_LOOP);

            
            // BODY
            glColor3ub(255, 198, 43);
            if (dead)
                glColor3ub(162, 162, 162);
            drawCircle(8, 0, 0, 0, 360, GL_POLYGON);

            glColor3ub(0, 0, 0);
            glLineWidth(3);
            drawCircle(8.2, 0, 0, 0, 360, GL_LINE_LOOP);


            // RIGHT LEG
            glColor3ub(96,57,19);
            if (dead)
                glColor3ub(162, 162, 162);
            drawElipse(4.35f, -7.0f, 3.0f, 1.4f, GL_POLYGON);

            glColor3ub(0,0,0);
            glLineWidth(3);
            drawElipse(4.35f, -7.0f, 3.0f, 1.4f, GL_LINE_LOOP);

            glPushMatrix();
            glTranslatef(eyesAngle, 2.5, 0);
            if (dead)
                glScalef(1.5, 0.2, 0);
                // LEFT EYES
                drawElipse(2.0f, 0.0f, 0.78f, 1.65f, GL_POLYGON);

                // RIGHT EYES
                drawElipse(-2.0f, 0.0f, 0.78f, 1.65f, GL_POLYGON);
            glPopMatrix();

            glPopMatrix();
        }

        void animate(float targetAngle) {
            eyesAngle = 3.0f * cos(targetAngle);
        }
};


class LifeHeart {
    public:
        float x, y;
    
        LifeHeart (float x, float y) {
            this->x = x;
            this->y = y;
        }

        void draw() {
            glPushMatrix();
            glTranslatef(x, y, 1);
            glColor3ub(255, 0, 0);
                drawHeart(0, 0, 0.15);
            glPopMatrix();
        }
};


class Bomb {
    public:
        float radius;
        float x, y;
        float t = 0.0f;
        float distance = 0.0f;

        bool exploding = false;
        bool explosionFinished = false;
        float explosionRadius = radius;
        int explosionDuration = 120;
        int explosionFrame = 0;

        bool explosionSoundPlayed = false;

        Text text;

        static void BOMB_DESIGN() {
            // BOMB SPARK
            vector<vector<float>> BOMB_SPARK_POINTS = {
                {5.9f, 7.5f},{5.74f, 7.27f},{ 5.613f, 7.06f}, {5.52f, 6.74f},{5.5f ,6.5f} ,{5.5f, 6.3f} ,{5.68f ,6.55f} ,
                {5.9f ,6.8f }, {6.067f,6.975f}, {6.31f ,7.15f} ,{6.6f , 7.3f} ,{6.95f,7.374f}, {7.26f,7.3f},{7.52f ,7.34f} ,
                {7.66f,7.216f},{7.493f,7.17f}, {7.306f,7.053f} ,{7.064f, 6.914f} , {6.87f ,6.727f} ,{6.69f, 6.44f},{6.53f,6.19f},
                {6.416f,5.939f},{6.344f,5.705f},{ 6.33f, 5.54f}, { 6.56f, 5.624f }, { 6.8f, 5.76f }, { 7.0f ,5.9f }, {7.125f, 6.06f}, 
                {7.247f,6.18f},{ 7.37f,6.415f}, {7.48f ,6.233f},{7.498f,6.096f},{7.5f,6.0f}
            };

            glLineWidth(1);
            glColor3ub(0,0,0);
            drawPoints(BOMB_SPARK_POINTS, GL_LINE_STRIP);

            drawCircle(1.5f, 6.0f, 6.0f, 90, 360, GL_LINE_STRIP);


            glColor3ub(255, 0, 0);
            drawCircle(1.2f, 6.0f, 6.0f, 90, 360, GL_POLYGON);

            // BOMB HEAD
            vector<vector<float>> BOMB_HEAD_POINTS = {
                {2.85f,4.1f}, {4.24f,2.65f}, {5.0f,3.44f}, {3.578f,4.83f}
            };

            glColor3ub(0,0,0);
            glLineWidth(3);
            drawPoints(BOMB_HEAD_POINTS, GL_LINE_LOOP);
            drawPoints(BOMB_HEAD_POINTS, GL_POLYGON);

            // BOMB BODY
            glColor3ub(88,88,90);
            drawCircle(5, 0, 0, 0, 360, GL_POLYGON);

            glColor3ub(0,0,0);
            glLineWidth(2);
            drawCircle(5, 0, 0, 0, 360, GL_LINE_LOOP);
        }
        

        Bomb(float radius, Text text, float x=0, float y=0) {
            this->radius = radius;
            this->x = x;
            this->y = y;
            this->text = text;
        }

        Bomb() {};

        void explode()
        {
            if (!exploding && !explosionFinished) {
                exploding = true;
                explosionFrame = 0;
                explosionRadius = radius;
            }
        }

        bool isDone() const {
            return explosionFinished;
        }

        void draw()
        {
            if (explosionFinished) {
                return;
            }

            // BOBM EXPLOSION
            if (exploding) {
                float t = (float)explosionFrame / explosionDuration;
                float peakT = 0.3f;

                float outerRadius;
                float innerRadius;
                GLubyte alpha = ((1 - t) * 255);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4ub(255, 0, 0, alpha);
                    if (t < peakT) {
                        float t1 = t / peakT;
                        outerRadius = 0.0f + t1 * (BOMB_EXPLOSION_MAX_RADIUS - 0.0f);

                    } 
                    else {
                        float t2 = (t - peakT) / (1.0f - peakT);
                        outerRadius = BOMB_EXPLOSION_MAX_RADIUS + t2 * (0.0f - BOMB_EXPLOSION_MAX_RADIUS);
                    }

                    drawStar(x, y, outerRadius);
                glDisable(GL_BLEND);
            }
            else {
                float txt_x;
                float txt_y;

                glPushMatrix();
                glTranslatef(x, y, 0);
                glScalef(-0.5f, 0.5f, 1.0f);
                    BOMB_DESIGN();
                glPopMatrix();

                txt_x = x + (radius * cosf(PI/4.0)) + 1;
                txt_y = y + (radius * sinf(PI/4.0)) + 1;
                text.draw(txt_x, txt_y);
            }
        }

        
        void animate(ma_engine* g_audioEngine) {
            if (exploding) {
                explosionFrame++;
                if (explosionFrame >= explosionDuration) {
                    exploding = false;
                    explosionFinished = true;
                }
            } else if (!explosionFinished) {
                // ANIMATION MATH
                float dirX = PLAYER_X - x;
                float dirY = PLAYER_Y - y;

                distance = sqrt(dirX * dirX + dirY * dirY);

                if (distance > 1.0f) {
                    x += (dirX / distance) * BOMB_SPEED;
                    y += (dirY / distance) * BOMB_SPEED;
                }

                
                if (y <= PLAYER_Y + PLAYER_RADIUS) {
                    if(!explosionSoundPlayed){
                        ma_engine_play_sound(g_audioEngine, SOUND_EXPLOSION, NULL);
                        explosionSoundPlayed = true;
                    }
                    explode();
                    livesLeft--;
                }
            }
        }
};


class Gun {
    public:
    float x, y;
    float angleOfAttack;

    Gun (float x=GUN_X, float y=GUN_Y, float angleOfAttack=45) {
        this->x = x;
        this->y = y;
        this->angleOfAttack = angleOfAttack;
    }

    // Gun () {};

    void draw() {
        glPushMatrix();
        
        float displayAngle = angleOfAttack;
        x = GUN_X;
        bool shouldFlip = abs(angleOfAttack) > 90;
        
        if (shouldFlip) {
            displayAngle = 180.0f - angleOfAttack;
            x = -GUN_X;
        }
        
        glTranslatef(x, y, 0);

        if (shouldFlip)
            glScalef(-1.0f, 1.0f, 1.0f); 

        glRotatef(displayAngle, 0, 0, 1);
        

        // GUN STOCK
        vector<vector<float>> GUN_STOCK_POINTS = {
            {-8.76f, -0.84f},   {-15.06f, 0.2f},    {-15.12f, -5.13f}, {-8.73f, -2.18f},
            {-6.87f, -2.78f},   {-3.56f, -0.01f},   {-4.811f, 1.77f},  {-7.1f, 0.5f},
            {-8.83f, 0.88f},    {-15.06f, 0.2f}
        };
        glColor3ub(81, 49, 42);
        drawPoints(GUN_STOCK_POINTS, GL_TRIANGLE_FAN);
        
        // GUN BARREL
        vector<vector<float>> GUN_BARREL_POINTS = {
            {-3.56f, -0.01f}, {7.5f, 0.0f},   {8.62f, 1.4f},  {15.92f, 1.4f},
            {15.92f, 2.62f},  {-3.45f, 2.7f},  {-4.811f, 1.77f}
        };
        glColor3ub(189, 190, 192);
        drawPoints(GUN_BARREL_POINTS, GL_POLYGON);

        // GUN OUTLINE
        std::vector<std::vector<float>> GUN_OUTLINE_POINTS = {
            {-15.06f, 0.2f},  {-15.12f, -5.13f}, {-8.73f, -2.18f}, {-6.87f, -2.78f},
            {-3.56f, -0.01f}, {7.5f, 0.0f},      {8.62f, 1.4f},   {15.92f, 1.4f},
            {15.92f, 2.62f},  {-3.45f, 2.7f},     {-7.1f, 0.2f},   {-8.83f, 0.88f}
        };
        glColor3ub(0,0,0);
        glLineWidth(1.5);
        drawPoints(GUN_OUTLINE_POINTS, GL_LINE_LOOP);

        //TRIGGER
        drawCircle(1.4, 0, 0, 180, 360, GL_LINE_LOOP);
        glPopMatrix();
    }

    float animate (float angleOfAttack) {
        this->angleOfAttack = angleOfAttack;
        draw();
        return x;
    }
};


class Bullet {
    public:
        float x;
        float y;
        float radius = 0.3f;
        bool active = true;
        float angleOfAttack_deg = 0.0f;

        Player* player;
        Bomb* targetBomb;
        Gun* gun;

        Bullet(Bomb* targetBomb, Player* player, Gun* gun, ma_engine* g_audioEngine, float x=GUN_X, float y=GUN_Y) {
            this->x = x;
            this->y = y;

            float angleOfAttack_rad = atan2((targetBomb->y - y), (targetBomb->x - x));
            angleOfAttack_deg = (angleOfAttack_rad * (180.0f / PI));

            this->targetBomb = targetBomb;
            this->player = player;
            // Move the eyes and gun
            player->animate(angleOfAttack_rad);
            this->x = gun->animate(angleOfAttack_deg);

            ma_engine_play_sound(g_audioEngine, "sounds/firing.wav", NULL);

        }

        Bullet() {};

        void draw() {
            if (!active) return;

            vector<vector<float>> BULLET_POINTS = {
                {0, 1.3}, {-0.46, 0.4}, {-0.46, -0.8}, {-0.3, -0.8}, {-0.3, -1}, 
                {0.3, -1}, {0.3, -0.8}, {0.46, -0.8}, {0.46, 0.4}
            };
            glPushMatrix();
            glTranslatef(x, y, 0.0f);
            glRotatef(angleOfAttack_deg -90.0f, 0, 0.0f, 1.0f);

            glPushMatrix();
            glScalef(1.5, 1.5, 1.0f);
                glColor3ub(0, 0, 0);
                glLineWidth(1);
                drawPoints(BULLET_POINTS, GL_LINE_LOOP);
            glPopMatrix();

            glPushMatrix();
            glScalef(1.4, 1.4, 1.0f);
                // glColor3ub(252, 91, 40);
                glColor3ub(66, 73, 89);
                drawPoints(BULLET_POINTS, GL_POLYGON);
            glPopMatrix();
            glPopMatrix();
        }

        void animate() {
            if (!active || targetBomb->isDone()) 
                return;

            // ANIMATION MATH
            float dirX = targetBomb->x - x;
            float dirY = targetBomb->y - y;

            float distance = sqrt(dirX * dirX + dirY * dirY);

            if (distance <= targetBomb->radius) {
                targetBomb->explode();
                active = false;
                score++;
                return;
            }

            if (distance > 0) {
                x += (dirX / distance) * BULLET_SPEED;
                y += (dirY / distance) * BULLET_SPEED;
            }

        }
};


class Background {
    public:

    Background() {}

    void drawRect(float x1, float y1, float x2, float y2, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
        glEnd();
    }


    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
        glEnd();
    }


    void draw() {
        glPushMatrix();
        glScalef(190, 100, 0);

        // Sky blocks
        drawRect(-0.3f, 0.3f, -0.2f, 0.4f, 0.6f, 0.9f, 1.0f);
        drawRect(0.2f, 0.4f, 0.3f, 0.5f, 0.6f, 0.9f, 1.0f);
        drawRect(0.7f, 0.2f, 0.8f, 0.3f, 0.6f, 0.9f, 1.0f);

        // Ground
        drawRect(-1.0f, -1.0f, 1.0f, -0.5f, 0.76f, 0.60f, 0.42f);

        // Ground blocks
        drawRect(-0.9f, -0.9f, -0.8f, -0.8f, 0.85f, 0.70f, 0.50f);
        drawRect(-0.5f, -0.95f, -0.3f, -0.85f, 0.85f, 0.70f, 0.50f);
        drawRect(0.2f, -0.88f, 0.35f, -0.78f, 0.85f, 0.70f, 0.50f);
        

        // Grass blocks
        drawRect(-0.7f, -0.7f, -0.6f, -0.6f, 0.2f, 0.8f, 0.2f);
        drawRect(0.0f, -0.8f, 0.15f, -0.7f, 0.2f, 0.8f, 0.2f);
        drawRect(0.6f, -0.75f, 0.75f, -0.65f, 0.2f, 0.8f, 0.2f);

        // Clouds
        drawRect(-0.7f, 0.6f, -0.5f, 0.7f, 1.0f, 1.0f, 1.0f);
        drawRect(-0.65f, 0.7f, -0.4f, 0.8f, 1.0f, 1.0f, 1.0f);
        drawRect(-0.6f, 0.65f, -0.45f, 0.75f, 0.9f, 0.9f, 0.9f);

        drawRect(0.4f, 0.5f, 0.7f, 0.6f, 1.0f, 1.0f, 1.0f);
        drawRect(0.5f, 0.6f, 0.8f, 0.7f, 1.0f, 1.0f, 1.0f);
        drawRect(0.55f, 0.55f, 0.7f, 0.65f, 0.9f, 0.9f, 0.9f);
 
        glPopMatrix();
    }
};


class Word {
    public:
    string filename;
    vector<string> words = EMBEDDED_WORDS;
    
    Word(string filename) {
        this->filename = filename;
    }

    void loadWords() {
        words.clear();
        ifstream file(filename.c_str());
        string word;

        while (file >> word) {
            words.push_back(word);
        }
    }

    string getRandomWord(vector<Bomb> &currentBombs) {
        if (words.empty()) {
            return "";
        }

        while (true) { 
            int index = getRandomInt(0, words.size());
            string newWord = words[index];

            bool matchFound = false;
            for (const Bomb &bomb : currentBombs) {
                if (bomb.text.str[0] == newWord[0]) {
                    matchFound = true;
                    break;
                }
            }

            if (!matchFound) {
                return newWord;
            }
        }
    }
};


class GameOver {
    public:
    int HIGH_SCORE = 35;
        bool gameOverSoundPlayed = false;

        PixelArtText gameOverTxt = PixelArtText(2);
        PixelArtText scoreTxt = PixelArtText(1);
        PixelArtText highScoreTxt = PixelArtText(0.7);


        void draw(ma_engine *g_audioEngine) {
            if(!gameOverSoundPlayed){
                ma_engine_play_sound(g_audioEngine, SOUND_GAME_OVER, NULL);
                gameOverSoundPlayed = true;
            }

            // To fade the gameplay (opacity lowered)
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0f, 0.0f, 0.0f, 0.30f);
            glBegin(GL_QUADS);
                glVertex2f(190, 100);
                glVertex2f(190, -100);
                glVertex2f(-190, -100);
                glVertex2f(-190, 100);
            glEnd();

            gameOverTxt.setColor(214, 40, 40);
            gameOverTxt.drawCentered("GAME OVER", 0, 30, 190);

            scoreTxt.drawCentered(to_string(score), 0, 0, 190);

            highScoreTxt.drawCentered("HI " + to_string(HIGH_SCORE), 0, -15, 190);
        }
};


class HomePage {
    public: 
    HomePage () {}

    Background background = Background();

    PixelArtText titleTxt = PixelArtText(2);
    PixelArtText startGameTxt = PixelArtText(1);
    PixelArtText exitGameTxt = PixelArtText(1);
    

    void draw() {
        background.draw();
        
        titleTxt.setColor(244, 90, 0);
        titleTxt.drawCentered("WORD BLASTER", 0, 30, 190);

        startGameTxt.setColor(0, 48, 73);
        startGameTxt.drawCentered("Press ENTER to play", 0, 0, 190);

        exitGameTxt.setColor(0, 48, 73);
        exitGameTxt.drawCentered("Press ESC to exit", 0, -20, 190);
    }

};