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

        vector<vector<float>> BULLET_POINTS = {
            { 0.001f, -1.151f }, { -0.297f, -1.149f }, { -0.3f, -1.06f },
            { -0.383f, -0.887f }, { -0.461f, -0.8876f }, { -0.4653f, 0.3798f },
            { -0.38f, 0.38f },   { -0.3796f, 0.636f }, { -0.294f, 0.636f },
            { -0.2944f, 0.806f }, { -0.2104f, 0.806f }, { -0.2104f, 0.9776f },
            { -0.126f, 0.9773f }, { -0.126f, 1.062f },  { -0.0426f, 1.0625f },
            { -0.0426f, 1.149f }, { 0.0f, 1.15f },
            { 0.0426f, 1.149f }, { 0.0426f, 1.0625f }, { 0.126f, 1.062f },
            { 0.126f, 0.9773f }, { 0.2104f, 0.9776f }, { 0.2104f, 0.806f },
            { 0.2944f, 0.806f }, { 0.294f, 0.636f },   { 0.3796f, 0.636f },
            { 0.38f, 0.38f }, { 0.4653f, 0.3798f }, { 0.461f, -0.8876f },
        };
        glPushMatrix();
        glScalef(1, 1, 1.0f);
            drawPoints(BULLET_POINTS, GL_LINE_LOOP);
        glPopMatrix();
        

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