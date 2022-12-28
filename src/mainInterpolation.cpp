#include <cstdlib>
#include "Camera.h"
#include "InterpolationCurve.h"
#include "ControlPoints.h"

// for mac osx
#ifdef __APPLE__
#include <glm/gtc/type_ptr.hpp>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
// only for windows
#ifdef _WIN32
#include <windows.h>
#endif
// for windows and linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

using namespace std;
/*
 * Utils
 */
Camera* camera;
std::unique_ptr<InterpolationCurve> inter;
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
glm::dvec3 functionalColor{.235, .706, .294};
glm::dvec3 uniformColor{1., 1., .098};
glm::dvec3 distanceColor{.568, .118, .705};
glm::dvec3 rootDistanceColor{.96, .51, .188};
glm::dvec3 chebycheffColor{.0, .5, .5};
/// color of the Control polygon
glm::dvec3 controlColor{0.0, 0.0, 1.0};
/// color of the background
GLclampf backColor[3] = {.85f, .85f, .85f};
/// threshold for clicking distance
double clickThresh{50.0};
/// segments per Bezier curve
std::size_t steps{100};

bool draw_functional{false};
bool draw_uniform{false};
bool draw_polygon{false};
bool draw_distance{false};
bool draw_root_distance{false};
bool draw_chebycheff{false};

void drawCurve(const std::vector<Point>& points, const glm::dvec3& color)
{
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glColor3dv(glm::value_ptr(color));
    for(const auto& point : points)
        glVertex2d(point.x, point.y);
    glEnd();
    glLineWidth(1);
}

/**
 * The rendering function
 */
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // drawing the bounding box
    if(draw_polygon)
    {
        drawCurve(inter->getControlPoints(), controlColor);
    }

    if(draw_functional)
    {
        // drawing the functional curve
        drawCurve(inter->getFunctionalCurve(), functionalColor);
    }

    if(draw_uniform)
    {
        // drawing the uniform curve
        drawCurve(inter->getUniformCurve(), uniformColor);
    }

    if(draw_distance)
    {
        // drawing the distance curve
        drawCurve(inter->getDistanceCurve(), distanceColor);
    }

    if(draw_root_distance)
    {
        // drawing the root distance curve
        drawCurve(inter->getRootDistanceCurve(), rootDistanceColor);
    }

    if(draw_chebycheff)
    {
        // drawing the root distance curve
        drawCurve(inter->getChebycheffCurve(), chebycheffColor);
    }


    // drawing the control points
    glPointSize(8.f);
    glBegin(GL_POINTS);
    glColor3d(controlColor[0], controlColor[1], controlColor[2]);
    for(const auto& point : inter->getControlPoints())
        glVertex2d(point.x, point.y);
    glEnd();
    glPointSize(1.f);

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
        case 'c':
            inter->reset();
            break;
        case 'f':
            draw_functional = !draw_functional;
            break;
        case 'u':
            draw_uniform = !draw_uniform;
            break;
        case 'p':
            draw_polygon = !draw_polygon;
            break;
        case 'd':
            draw_distance = !draw_distance;
            break;
        case 'r':
            draw_root_distance = !draw_root_distance;
            break;
        case 't':
            draw_chebycheff = !draw_chebycheff;
            break;
        case 'q':
            exit(EXIT_SUCCESS);
        default:
            break;
    }
    glutPostRedisplay();
}

void reshape( int, int )
{
    glutReshapeWindow( window_width, window_height);
}

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
    glutCreateWindow("Interpolation");
    camera = new Camera(window_width, window_height);
    InterpolationCurve::Parameters param{0, static_cast<double>(window_width), 0.05};
    inter = std::make_unique<InterpolationCurve>(param);
    glClearColor(backColor[0], backColor[1], backColor[2], 0);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyPress);
    glutReshapeFunc( reshape );
}

int main(int, char**)
{
    initGlut();
    glutDisplayFunc(draw);
    glutMainLoop();
}
