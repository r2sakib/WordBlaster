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

        //GUN
        glPushMatrix();
        glScalef(5, 5, 1);


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