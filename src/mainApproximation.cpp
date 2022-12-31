#include "Camera.h"
#include <curves/BezierCurve.h>
#include <curves/ControlPoints.h>

#include <cstdlib>

// for mac osx
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
// only for windows
#ifdef _WIN32
#include <windows.h>
#endif
// for windows and linux
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;
/*
 * Utils
 */
Camera* camera;
std::unique_ptr<BezierCurve> inter;
bool track{false};
Point draggedPoint;
/*
 * Params
 */
/// width of the window
int window_width = 800;
/// height of the window
int window_height = 600;
/// color of the Bezier curve
GLdouble curveColor[3] = {1.0, 0.0, 0.0};
/// color of the Control polygon
GLdouble controlColor[3] = {0.0, 0.0, 1.0};
/// color of the background
GLclampf backColor[3] = {1.0, 1.0, 1.0};
/// threshold for clicking distance
double clickThresh{50.0};
/// segments per Bezier curve
std::size_t steps{100};

/**
 * The rendering function
 */
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // drawing the bounding box
    glBegin(GL_LINE_STRIP);
    glColor3d(controlColor[0], controlColor[1], controlColor[2]);
    //    for(const auto& point : bezierCurve->controlPoints)
    for(const auto& point : inter->getControlPoints())
        glVertex2d(point.x, point.y);
    glEnd();
    glPointSize(5.f);

    // drawing the control points
    glBegin(GL_POINTS);
    glColor3d(controlColor[0], controlColor[1], controlColor[2]);
    //    for(const auto& point : bezierCurve->controlPoints)
    for(const auto& point : inter->getControlPoints())
        glVertex2d(point.x, point.y);
    glEnd();

    // drawing the curve
    glBegin(GL_LINE_STRIP);
    glColor3d(curveColor[0], curveColor[1], curveColor[2]);
    for(const auto& point : inter->getCurvePoint())
        glVertex2d(point.x, point.y);
    glEnd();
    glutSwapBuffers();
}

/**
 * The mouse click callback method
 * @param button The glut enum for left or right button
 * @param state Pressed or released
 * @param x x coordinate on the window
 * @param y y coordinate on the window
 */
void mouseClick(int button, int state, int x, int y)
{
    Point n{(double)x, window_height - (double)y};
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // addition
    {
        inter->add(n);
    }
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
        inter->deleteControlPoint(n, clickThresh);
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // start tracking mouse for dragging
    {
        const auto res = inter->getClosestPoint(n, clickThresh);
        if(res.has_value())
        {
            track = true;
            draggedPoint = Point(res.value());
        }
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) // stop tracking
    {
        track = false;
    }
    glutPostRedisplay();
}

/**
 * Callback function for mouse movement
 * @param x The current x-coordinate of the mouse
 * @param y The current y-coordinate of the mouse
 */
void mouseMove(int x, int y)
{
    if(!track)
        return;
    Point p = {(double)x, window_height - (double)y};
    if(inter->updateControlPoint(draggedPoint, p, clickThresh))
    {
        draggedPoint = p;
    }
    glutPostRedisplay();
}

/**
 * Handles key presses
 * @param key the keyboard input given by the user
 * @param x x coordinate of the input
 * @param y y coordinate of the input
 */
void keyPress(unsigned char key, int, int)
{
    switch(key)
    {
        case 'r': inter->reset(); break;
        case 'q': exit(EXIT_SUCCESS);
        default: break;
    }
    glutPostRedisplay();
}

void reshape(int, int) { glutReshapeWindow(window_width, window_height); }

/**
 * Initializes the glut parameters and the GL camera
 */
void initGlut()
{
    int x = 0;
    glutInit(&x, nullptr);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Bezier");
    camera = new Camera(window_width, window_height);
    inter = std::make_unique<BezierCurve>(steps);
    glClearColor(backColor[0], backColor[1], backColor[2], 0);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyPress);
    glutReshapeFunc(reshape);
}

int main(int, char**)
{
    initGlut();
    glutDisplayFunc(draw);
    glutMainLoop();
}
