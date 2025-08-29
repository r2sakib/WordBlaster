#pragma once

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

const float PI = 3.14159265359;

const float PLAYER_X = 0.0f;
const float PLAYER_Y = -80.0f;
const float PLAYER_RADIUS = 8.2f;
int lives = 3;

const float GUN_X = 0.0f;
const float GUN_Y = -80.0f;

const float BOMB_RADIUS = 2.0f;
const float BOMB_SPEED = 0.3f;

const float BULLET_SPEED = 1.0f;

struct RGB {
    int r, g, b;
};

RGB TEXT_COLOR = {0, 0, 0};


void drawCircle(float radius, float xc, float yc, float startAngle, float endAngle, int primative=GL_LINE_LOOP)
{
    glBegin(primative);
        for(int i = startAngle; i < endAngle; i++)
        {
            float pi = 3.14159265359;
            float A = (i*2*pi)/360;
            float x = radius * cos(A);
            float y = radius * sin(A);
            glVertex2f(x + xc, y + yc);
        }
    glEnd();
}

void drawElipse(float h, float k, float a, float b, int primative, float y_min=-9999.0f)
{
    glBegin(primative);
    for(int i = 0; i < 200;i++)
            {
                float pi = 3.14159265359;
                float A = (i*2*pi)/200;
                float scale = sqrt(3.1f);
                float x = h + a * scale * cos(A); 
                float y = k + b * scale * sin(A);

                if (y >= y_min)
                    glVertex2f(x, y);
            }
            
    glEnd();
}

void drawPoints(const vector<vector<float>>& points, int primative)
{
    glBegin(primative);
        for (int i = 0; i < points.size(); i++) {
            glVertex2f(points[i][0], points[i][1]);
        }
    glEnd();
}

float* getLineEqn(float x1, float y1, float x2, float y2)
{
    // vector<float> coeffs(2);
    float* coeffs = new float[2];
    coeffs[0] = (y2 - y1) / (x2 - x1); // m
    coeffs[1] = y1 - coeffs[0] * x1;   // b
    return coeffs;
}

float linearInterpolate(float start, float end, float t)
{
    return (1 - t) * start + t * end;
}

float* drawParabola(float eqn_xCoeff, float eqn_const, float startX, float endX)
{
    float* yCoords = new float[2];
    float y;
    glBegin(GL_LINE_STRIP);
        for (float x = startX; x <= endX; x+=0.01) 
        {    
            y = eqn_xCoeff * (x*x) + eqn_const; 
            glVertex2f(x, y);
            if (x == startX) {yCoords[0] = y;}
        }
    glEnd();
    yCoords[1] = y;
    return yCoords;
}

vector<float> getRandomPoint() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<float> distribX(-190.0f, 190.0f);
    static std::uniform_real_distribution<float> distribY(50.0f, 100.0f);

    float x = distribX(gen);
    float y = distribY(gen);

    return {x, y};
}

class Text {
    public:
        int r, g, b;
        float x, y;
        void *font;
        string text;
    
        Text() {};

        Text(const string& text, float x=0, float y=0, void *font=GLUT_BITMAP_HELVETICA_18,  RGB color=TEXT_COLOR) {
            this->text = text.c_str();
            this->x = x;
            this->y = y;
            this->font = font;
            this->r = color.r;
            this->g = color.g;
            this->b = color.b;
        }
        
        void draw(float x, float y)
        {
            this->x = x;
            this->y = y;

            glColor3ub(r, g, b);
            glRasterPos2f(x, y);
            for (char ch : text) {
                glutBitmapCharacter(font, ch);
            }
        }

        void draw()
        {
            this->x = x;
            this->y = y;

            glColor3ub(r, g, b);
            glRasterPos2f(x, y);
            for (char ch : text) {
                glutBitmapCharacter(font, ch);
            }
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
        float explosionMaxRadius = 7.0f;
        int explosionDuration = 60;
        int explosionFrame = 0;

        static void BOMB_DESIGN() {
                // BOMB SPARK
                vector<vector<float>> BOMB_SPARK_POINTS = {
                    {5.9f, 7.5f},{5.74f, 7.27f},{ 5.613f, 7.06f}, {5.52f, 6.74f},{5.5f ,6.5f} ,{5.5f, 6.3f} ,{5.68f ,6.55f} ,
                    {5.9f ,6.8f }, {6.067f,6.975f}, {6.31f ,7.15f} ,{6.6f , 7.3f} ,{6.95f,7.374f}, {7.26f,7.3f},{7.52f ,7.34f} ,
                    {7.66f,7.216f} ,{7.493f,7.17f}, {7.306f,7.053f} ,{7.064f, 6.914f} , {6.87f ,6.727f} ,{6.69f, 6.44f},{6.53f,6.19f},
                    {6.416f,5.939f},{6.344f,5.705f},{ 6.33f, 5.54f}, { 6.56f, 5.624f }, { 6.8f, 5.76f }, { 7.0f ,5.9f }, {7.125f, 6.06f}, 
                    {7.247f,6.18f},{ 7.37f,6.415f}, {7.48f ,6.233f},{7.498f,6.096f},{7.5f,6.0f}
                };

                glLineWidth(1);
                glColor3ub(0,0,0);
                drawPoints(BOMB_SPARK_POINTS, GL_LINE_STRIP);

                drawCircle(1.5f, 6.0f, 6.0f, 90, 360, GL_LINE_STRIP);


                glColor3ub(255,0,0);
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

        Text text;

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
            if (explosionFinished) return;

            if (exploding) {
                float t = (float)explosionFrame / (float)max(1, explosionDuration);
                float curR = explosionRadius + t * (explosionMaxRadius - explosionRadius);
                GLubyte alpha = (GLubyte)(255 * (1.0f - t));

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glColor4ub(255, 0, 0, alpha);
                    drawCircle(curR, x, y, 0, 360, GL_POLYGON);
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

        void animate() {
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
                    explode();
                    lives--;
                }
            }
        }
};


class Bullet {
    public:
        float x = GUN_X;
        float y = GUN_Y;
        float radius = 0.3f;
        bool active = true;
        float angleOfAttack_deg = 0.0f;

        Bomb* targetBomb;

        Bullet(Bomb* targetBomb, float x=GUN_X, float y=GUN_Y) {
            this->x = x;
            this->y = y;

            float angleOfAttack_rad = atan2((targetBomb->y - y), (targetBomb->x - x));
            angleOfAttack_deg = (angleOfAttack_rad * (180.0f / PI)) -90.0f;

            this->targetBomb = targetBomb;
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
            glRotatef(angleOfAttack_deg, 0, 0.0f, 1.0f);

            glPushMatrix();
            glScalef(1.5, 1.5, 1.0f);
                glColor3ub(0, 0, 0);
                glLineWidth(1);
                drawPoints(BULLET_POINTS, GL_LINE_LOOP);
            glPopMatrix();

            glPushMatrix();
            glScalef(1.4, 1.4, 1.0f);
                glColor3ub(252, 91, 40);
                drawPoints(BULLET_POINTS, GL_POLYGON);
            glPopMatrix();
            glPopMatrix();
        }

        void animate() {
            if (!active || targetBomb->isDone()) return;

            // ANIMATION MATH
            float dirX = targetBomb->x - x;
            float dirY = targetBomb->y - y;

            float distance = sqrt(dirX * dirX + dirY * dirY);

            if (distance <= targetBomb->radius) {
                targetBomb->explode();
                active = false;
                return;
            }

            if (distance > 0) {
                x += (dirX / distance) * BULLET_SPEED;
                y += (dirY / distance) * BULLET_SPEED;
            }

        }
};

class Player {
    public:
        float x = PLAYER_X;
        float y = PLAYER_Y;

        Player(float x, float y) {
            this->x = x;
            this->y = y;
        }

        Player() {}

        void draw() {
            glPushMatrix();
            glTranslatef(x, y, 0.0f);

        
            // LEFT LEG
            glColor3ub(96, 57, 19);
            drawElipse(-4.35f, -7.0f, 3.0f, 1.4f, GL_POLYGON);

            glColor3ub(0,0,0);
            glLineWidth(3);
            drawElipse(-4.35f, -7.0f, 3.0f, 1.4f, GL_LINE_LOOP);

            
            // BODY
            glColor3ub(255, 198, 43);
            drawCircle(8, 0, 0, 0, 360, GL_POLYGON);

            glColor3ub(0, 0, 0);
            glLineWidth(3);
            drawCircle(8.2, 0, 0, 0, 360, GL_LINE_LOOP);


            // RIGHT LEG
            glColor3ub(96,57,19);
            drawElipse(4.35f, -7.0f, 3.0f, 1.4f, GL_POLYGON);

            glColor3ub(0,0,0);
            glLineWidth(3);
            drawElipse(4.35f, -7.0f, 3.0f, 1.4f, GL_LINE_LOOP);

            
            // LEFT EYES
            drawElipse(2.0f, 0.0f, 0.78f, 1.65f, GL_POLYGON);

            // RIGHT EYES
            drawElipse(-2.0f, 0.0f, 0.78f, 1.65f, GL_POLYGON);

            glPopMatrix();
        }
};
