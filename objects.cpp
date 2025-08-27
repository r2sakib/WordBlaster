#pragma once

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string>

using namespace std;

const float PI = 3.14159265359;

void circle(float radius, float xc, float yc, float startAngle, float endAngle, int type=GL_LINE_LOOP, int lineWidth=1)
{
    glLineWidth(lineWidth);
    glBegin(type);
        for(int i = startAngle; i < endAngle; i++)
        {
            float pi = PI;
            float A = (i*2*pi)/360;
            float x = radius * cos(A);
            float y = radius * sin(A);
            glVertex2f(x + xc, y + yc);
        }
    glEnd();
}

float* parabola(float eqn_xCoeff, float eqn_const, float startX, float endX)
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


class Text {
    public:
        int r, g, b;
        float x, y;
        void *font;
        string text;
    
        Text() {};

        Text(const string& text, float x=0, float y=0, void *font=GLUT_BITMAP_HELVETICA_18,  int r=255, int g=255, int b=255) {
            this->text = text.c_str();
            this->x = x;
            this->y = y;
            this->font = font;
            this->r = r;
            this->g = g;
            this->b = b;
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
                    circle(curR, x, y, 0, 360, GL_POLYGON);
                glDisable(GL_BLEND);
            }
            else {
                float txt_x;
                float txt_y;

                glPushMatrix();
                glTranslatef(x, y, 0);
                    glColor3ub(r, g, b);
                    circle(radius, 0, 0, 0, 360, GL_POLYGON);
                glPopMatrix();

                txt_x = x + (radius * cosf(PI/4.0f)) + 1;
                txt_y = y + (radius * sinf(PI/4.0f)) + 1;
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
                y -= 0.5f;
                if (y < 0.0f) {
                    explode();
                }
            }
        }
};


