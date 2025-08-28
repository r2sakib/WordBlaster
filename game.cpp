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

const float BOMB_SPEED = 0.000003f;

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


void drawElipse(float h, float k, float a, float b, int primative)
{
    glBegin(primative);
    for(int i=0;i<200;i++)
            {
                float pi=3.1416;
                float A = (i*2*pi)/200;
                float scale = sqrt(3.1f);
                float x = h + a * scale * cos(A); 
                float y = k + b * scale * sin(A);
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
        int r, g, b;
        float radius;
        float x, y;
        float* coeffs;
        float t = 0.0f;

        bool exploding = false;
        bool explosionFinished = false;
        float explosionRadius = radius;
        float explosionMaxRadius = 7.0f;
        int explosionDuration = 60;
        int explosionFrame = 0;

        Text text;

        Bomb(float radius, Text text, float x=0, float y=0, int r=0, int g=0, int b=255) {
            this->radius = radius;
            this->x = x;
            this->y = y;
            this->text = text;

            this->r = r;
            this->g = g;
            this->b = b;
        }

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
                    glColor3ub(r, g, b);
                    drawCircle(radius, 0, 0, 0, 360, GL_POLYGON);
                glPopMatrix();

                txt_x = x + (radius * cosf(PI/4.0f)) + 1;
                txt_y = y + (radius * sinf(PI/4.0f)) + 1;
                text.draw(txt_x, txt_y);
            }
            coeffs =  getLineEqn(x, y, PLAYER_X, PLAYER_Y);
        }

        void animate() {
            if (exploding) {
                explosionFrame++;
                if (explosionFrame >= explosionDuration) {
                    exploding = false;
                    explosionFinished = true;
                }
            } else if (!explosionFinished) {
                t += BOMB_SPEED;
                if (t > 1.0f) t = 1.0f;

                x = linearInterpolate(x, PLAYER_X, t);
                y = linearInterpolate(y, PLAYER_Y, t);

                if (y <= PLAYER_Y + PLAYER_RADIUS) {
                    explode();
                    lives--;
                }
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
