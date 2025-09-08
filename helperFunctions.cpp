#pragma once

#include <GL/glut.h>
#include <math.h>
#include <string>
#include <vector>
#include <random>

#include "constants.cpp"

using namespace std;

// void playSound(string path) {
//     ma_result result;
//     ma_engine engine;

//     ma_engine_uninit(&engine);
//     result = ma_engine_init(NULL, &engine);
//     if (result != MA_SUCCESS)
//         //

//     ma_engine_play_sound(&engine, path.c_str(), NULL);
// }


vector<float> getRandomPoint() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<float> distribX(-185.0f, 185.0f);
    static std::uniform_real_distribution<float> distribY(100.0f, 200.0f);

    float x = distribX(gen);
    float y = distribY(gen);

    return {x, y};
}

int getRandomInt(int rangeStart, int rangeEnd) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_int_distribution<int> distrib(rangeStart, rangeEnd);

    return distrib(gen);
}


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

void drawStar(float x, float y, float outerRadius, float innerRadiusFactor=0.5) {
    float innerRadius = outerRadius * innerRadiusFactor;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 30) {
            float rad = i * PI / 180.0f;
            float r = (i % 60 == 0) ? outerRadius : innerRadius; 
            glVertex2f(x + r * cos(rad), y + r * sin(rad));
        }
    glEnd();
}
