#pragma once

#include <GL/glut.h>
#include <math.h>
#include <string>
#include <vector>
#include <random>
#include <map>

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

void drawHeart(float centerX, float centerY, float scale) {
    glBegin(GL_POLYGON);

    int numSteps = 100;
    for (int i = 0; i <= numSteps; ++i) {
        float t = (float)i / (float)numSteps * 2.0f * M_PI;

        float x = scale * 16.0f * pow(sin(t), 3);
        float y = scale * (13.0f * cos(t) - 5.0f * cos(2 * t) - 2.0f * cos(3 * t) - cos(4 * t));

        glVertex2f(centerX + x, centerY + y);
    }

    glEnd();
}



class PixelArtRenderer {
private:
    static const int CHAR_WIDTH = 5;
    static const int CHAR_HEIGHT = 7;
    static const int CHAR_SPACING = 1;
    
    // Character bitmap data (5x7 pixels, stored as binary patterns)
    std::map<char, std::vector<std::vector<bool>>> charPatterns;
    
    float pixelSize;
    int r, g, b; // Color components
    
    void initializeCharacterPatterns() {
        // Numbers 0-9
        charPatterns['0'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,1,1},
            {1,0,1,0,1},
            {1,1,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['1'] = {
            {0,0,1,0,0},
            {0,1,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0}
        };
        
        charPatterns['2'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {0,0,0,0,1},
            {0,0,0,1,0},
            {0,0,1,0,0},
            {0,1,0,0,0},
            {1,1,1,1,1}
        };
        
        charPatterns['3'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {0,0,0,0,1},
            {0,0,1,1,0},
            {0,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['4'] = {
            {0,0,0,1,0},
            {0,0,1,1,0},
            {0,1,0,1,0},
            {1,0,0,1,0},
            {1,1,1,1,1},
            {0,0,0,1,0},
            {0,0,0,1,0}
        };
        
        charPatterns['5'] = {
            {1,1,1,1,1},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {0,0,0,0,1},
            {0,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['6'] = {
            {0,0,1,1,0},
            {0,1,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['7'] = {
            {1,1,1,1,1},
            {0,0,0,0,1},
            {0,0,0,1,0},
            {0,0,1,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0}
        };
        
        charPatterns['8'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['9'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,1},
            {0,0,0,0,1},
            {0,0,0,1,0},
            {0,1,1,0,0}
        };
        
        // Uppercase letters A-Z
        charPatterns['A'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['B'] = {
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0}
        };
        
        charPatterns['C'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['D'] = {
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0}
        };
        
        charPatterns['E'] = {
            {1,1,1,1,1},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,1}
        };
        
        charPatterns['F'] = {
            {1,1,1,1,1},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0}
        };
        
        charPatterns['G'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,0},
            {1,0,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['H'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['I'] = {
            {0,1,1,1,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0}
        };
        
        charPatterns['J'] = {
            {0,0,1,1,1},
            {0,0,0,1,0},
            {0,0,0,1,0},
            {0,0,0,1,0},
            {0,0,0,1,0},
            {1,0,0,1,0},
            {0,1,1,0,0}
        };
        
        charPatterns['K'] = {
            {1,0,0,0,1},
            {1,0,0,1,0},
            {1,0,1,0,0},
            {1,1,0,0,0},
            {1,0,1,0,0},
            {1,0,0,1,0},
            {1,0,0,0,1}
        };
        
        charPatterns['L'] = {
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,1}
        };
        
        charPatterns['M'] = {
            {1,0,0,0,1},
            {1,1,0,1,1},
            {1,0,1,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['N'] = {
            {1,0,0,0,1},
            {1,1,0,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,0,0,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['O'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['P'] = {
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0}
        };
        
        charPatterns['Q'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,0,1,0},
            {0,1,1,0,1}
        };
        
        charPatterns['R'] = {
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0},
            {1,0,1,0,0},
            {1,0,0,1,0},
            {1,0,0,0,1}
        };
        
        charPatterns['S'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,0},
            {0,1,1,1,0},
            {0,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['T'] = {
            {1,1,1,1,1},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0}
        };
        
        charPatterns['U'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['V'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,0,1,0},
            {0,0,1,0,0}
        };
        
        charPatterns['W'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,1,0,1,1},
            {1,0,0,0,1}
        };
        
        charPatterns['X'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,0,1,0},
            {0,0,1,0,0},
            {0,1,0,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['Y'] = {
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,0,1,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0}
        };
        
        charPatterns['Z'] = {
            {1,1,1,1,1},
            {0,0,0,0,1},
            {0,0,0,1,0},
            {0,0,1,0,0},
            {0,1,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,1}
        };
        
        // Lowercase letters a-z
        charPatterns['a'] = {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {0,0,0,0,1},
            {0,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,1,1},
            {0,1,1,0,1}
        };
        
        charPatterns['b'] = {
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0}
        };
        
        charPatterns['c'] = {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['d'] = {
            {0,0,0,0,1},
            {0,0,0,0,1},
            {0,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,1}
        };
        
        charPatterns['e'] = {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,1,1,1,1},
            {1,0,0,0,0},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['f'] = {
            {0,0,1,1,0},
            {0,1,0,0,1},
            {0,1,0,0,0},
            {1,1,1,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0}
        };
        
        charPatterns['g'] = {
            {0,0,0,0,0},
            {0,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,1},
            {0,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['h'] = {
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['i'] = {
            {0,0,1,0,0},
            {0,0,0,0,0},
            {0,1,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0}
        };
        
        charPatterns['j'] = {
            {0,0,0,1,0},
            {0,0,0,0,0},
            {0,0,1,1,0},
            {0,0,0,1,0},
            {0,0,0,1,0},
            {1,0,0,1,0},
            {0,1,1,0,0}
        };
        
        charPatterns['k'] = {
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,1,0},
            {1,0,1,0,0},
            {1,1,0,0,0},
            {1,0,1,0,0},
            {1,0,0,1,0}
        };
        
        charPatterns['l'] = {
            {0,1,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0}
        };
        
        charPatterns['m'] = {
            {0,0,0,0,0},
            {1,1,0,1,0},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['n'] = {
            {0,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['o'] = {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['p'] = {
            {0,0,0,0,0},
            {1,1,1,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,1,1,1,0},
            {1,0,0,0,0},
            {1,0,0,0,0}
        };
        
        charPatterns['q'] = {
            {0,0,0,0,0},
            {0,1,1,1,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,1},
            {0,0,0,0,1},
            {0,0,0,0,1}
        };
        
        charPatterns['r'] = {
            {0,0,0,0,0},
            {1,0,1,1,0},
            {1,1,0,0,1},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0},
            {1,0,0,0,0}
        };
        
        charPatterns['s'] = {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {1,0,0,0,0},
            {0,1,1,1,0},
            {0,0,0,0,1},
            {1,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['t'] = {
            {0,1,0,0,0},
            {1,1,1,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,1},
            {0,0,1,1,0}
        };
        
        charPatterns['u'] = {
            {0,0,0,0,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,1,1},
            {0,1,1,0,1}
        };
        
        charPatterns['v'] = {
            {0,0,0,0,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {0,1,0,1,0},
            {0,0,1,0,0}
        };
        
        charPatterns['w'] = {
            {0,0,0,0,0},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,0,1,0,1},
            {1,1,0,1,1},
            {1,0,0,0,1}
        };
        
        charPatterns['x'] = {
            {0,0,0,0,0},
            {1,0,0,0,1},
            {0,1,0,1,0},
            {0,0,1,0,0},
            {0,1,0,1,0},
            {1,0,0,0,1},
            {1,0,0,0,1}
        };
        
        charPatterns['y'] = {
            {0,0,0,0,0},
            {1,0,0,0,1},
            {1,0,0,0,1},
            {1,0,0,1,1},
            {0,1,1,0,1},
            {0,0,0,0,1},
            {0,1,1,1,0}
        };
        
        charPatterns['z'] = {
            {0,0,0,0,0},
            {1,1,1,1,1},
            {0,0,0,1,0},
            {0,0,1,0,0},
            {0,1,0,0,0},
            {1,0,0,0,0},
            {1,1,1,1,1}
        };
        
        // Space character
        charPatterns[' '] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };
        
        // Some basic punctuation
        charPatterns['.'] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,1,1,0,0},
            {0,1,1,0,0}
        };
        
        charPatterns[','] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,1,1,0,0},
            {0,1,0,0,0}
        };
        
        charPatterns['!'] = {
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,0,0,0},
            {0,0,1,0,0}
        };
        
        charPatterns['?'] = {
            {0,1,1,1,0},
            {1,0,0,0,1},
            {0,0,0,0,1},
            {0,0,0,1,0},
            {0,0,1,0,0},
            {0,0,0,0,0},
            {0,0,1,0,0}
        };
        
        charPatterns[':'] = {
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,0,0,0}
        };
    }
    
public:
    PixelArtRenderer(float size = 10.0f) : pixelSize(size), r(255), g(255), b(255) {
        initializeCharacterPatterns();
    }
    
    void setColor(int red, int green, int blue) {
        r = red;
        g = green;
        b = blue;
    }
    
    void setPixelSize(float size) {
        pixelSize = size;
    }
    
    void drawPixel(float x, float y) {
        glColor3ub(r, g, b);
        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + pixelSize, y);
            glVertex2f(x + pixelSize, y + pixelSize);
            glVertex2f(x, y + pixelSize);
        glEnd();
    }
    
    void drawCharacter(char c, float startX, float startY) {
        auto it = charPatterns.find(c);
        if (it == charPatterns.end()) {
            return; // Character not found, skip
        }
        
        const auto& pattern = it->second;
        
        for (int row = 0; row < CHAR_HEIGHT; row++) {
            for (int col = 0; col < CHAR_WIDTH; col++) {
                if (pattern[row][col]) {
                    float pixelX = startX + (col * pixelSize);
                    float pixelY = startY - (row * pixelSize); // Y increases downward
                    drawPixel(pixelX, pixelY);
                }
            }
        }
    }
    
    void draw(const std::string& text, float startX, float startY) {
        float currentX = startX;
        
        for (char c : text) {
            drawCharacter(c, currentX, startY);
            currentX += (CHAR_WIDTH + CHAR_SPACING) * pixelSize;
        }
    }
    
    // Get the width that a string will occupy when rendered
    float getStringWidth(const std::string& text) {
        if (text.empty()) return 0;
        return (text.length() * (CHAR_WIDTH + CHAR_SPACING) - CHAR_SPACING) * pixelSize;
    }
    
    // Get the height of characters
    float getCharHeight() {
        return CHAR_HEIGHT * pixelSize;
    }
    
    // Center text horizontally at given Y position
    void drawCentered(const std::string& text, float centerX, float y, float screenWidth) {
        float textWidth = getStringWidth(text);
        float startX = centerX - (textWidth / 2.0f);
        draw(text, startX, y);
    }
};