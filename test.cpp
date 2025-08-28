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


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    
    glColor3ub(255, 0, 0);
    glLineWidth(2);
    drawCircle(7.0f, 0.0f, 0.0f, 0, 360, GL_LINE_LOOP);

    glColor3ub(0, 0, 0);
    glPushMatrix();
    glScalef(0.8f, 0.8f, 0);
        drawCircle(7.0f, 0.0f, 0.0f, 0, 360, GL_POLYGON);
    glPopMatrix();
    
    vector<vector<float>> points = {
        {-2.0, 1.0f}, {-1.0, 2.0}, {0.0, 2.5}, {1.0, 2.0}, {2.0, 1.0f}
    };

    drawPoints(points, GL_LINE_STRIP);

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