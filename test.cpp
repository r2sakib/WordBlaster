#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

void display();
void reshape(int, int);
void animate(int);




int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Word Blaster");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, animate, 0);

    glClearColor(1, 1, 1, 1);

    glutMainLoop();

    return 0;
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
                float pi = 3.1416;
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


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
        // HEAD
        glColor3f(0, 0, 0);  
        // glLineWidth(1.5);

        // drawElipse(0.0f, 0.0f, 0.65f, 2.5f, GL_LINE_LOOP, 0.95f);
        
        // glColor3ub(163, 110, 91);
        // glPushMatrix();
        // glScalef(0.9f, 0.9f, 1.0f);
        //     drawElipse(0.0f, 0.0f, 0.65f, 2.5f, GL_POLYGON, 0.95f);
        // glPopMatrix();

        // // UPERBODY
        // glColor3f(0, 0, 0);  
        // vector<vector<float>> UPPER_BODY_POINTS = {
        //     {-1.11f,1.0f}, {-1.11f,-4.5f}, {-1.6f,-5.2f}, {1.6f,-5.2f}, {1.11f,-4.5f}, {1.11f,1.0f}};
        // drawPoints(UPPER_BODY_POINTS, GL_LINE_LOOP);

        // glColor3ub(186, 144, 106);
        // glPushMatrix();
        // glScalef(0.7f, 0.7f, 1.0f);
        //     drawPoints(UPPER_BODY_POINTS, GL_POLYGON);
        // glPopMatrix();

        // // LOWERBODY
        // glColor3f(0, 0, 0);  
        // vector<vector<float>> LOWER_BODY_POINTS = {{-1.11f,-4.5f}, {1.11,-4.5} };
        // drawPoints(LOWER_BODY_POINTS, GL_LINE_LOOP);

        // glColor3ub(186, 144, 106);
        // glPushMatrix();
        // glScalef(0.9f, 0.9f, 1.0f);
        //     drawPoints(LOWER_BODY_POINTS, GL_POLYGON);
        // glPopMatrix();

        vector<std::vector<float>> EXPLOSION_POINTS = {
                {-2.8f, 0.0f},    {-0.577f, -0.36f},  {-1.777f, -1.575f}, {-0.12f, -0.536f},
                {0.34f, -1.533f}, {0.44f, -0.134f},   {1.8f, -0.7f},      {1.0f, 0.2f},
                {1.754f, 0.664f}, {0.726f, 0.635f},   {0.6f, 1.3f},       {0.04f, 0.56f},
                {-1.04f, 1.014f}, {-0.5f, 0.1f}
        };

        glColor3f(0, 0, 0);
        glLineWidth(1.5);
        drawPoints(EXPLOSION_POINTS, GL_LINE_LOOP);
        
        glPushMatrix();
        glScalef(0.9, 0.9, 1.0f);
            glColor3f(255, 0, 0);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f); 

            for (const auto& point : EXPLOSION_POINTS) {
                glVertex2f(point[0], point[1]);
            }

            glVertex2f(EXPLOSION_POINTS[0][0], EXPLOSION_POINTS[0][1]);
        glEnd();
        glPopMatrix();

        // glPopMatrix();

        // glPushMatrix();
        // glScalef(10, 10, 1.0f);
        // glBegin(GL_TRIANGLE_FAN);
        //     glVertex2f(0.0f, 0.0f); 

        //     for (const float* point : EXPLOSION_POINTS) {
        //         glVertex2f(point[0], point[1]);
        //     }

        //     glVertex2f(EXPLOSION_POINTS[0][0], EXPLOSION_POINTS[0][1]);
        // glEnd()
        // glPopMatrix();

    glEnd();
        

    glutSwapBuffers();
}


void animate(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, 0);


    

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