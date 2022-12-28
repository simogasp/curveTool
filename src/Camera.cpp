#include "Camera.h"

Camera::Camera(int Width, int Height)
{
    width = Width;
    height = Height;
    setModelView(glm::dvec3(-1, 0, 5), glm::dvec3(-1, 0, 0), glm::dvec3(0, 1, 0));
    setProjection(45, width / height, 0.1, 50.0);
    enable_key = true;
    is2D = false;
    to2D();
}

void Camera::to2D()
{
    if(is2D)
        return;
    glGetDoublev(GL_PROJECTION_MATRIX, saved_projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, saved_modelview);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    enable_key = false;
    is2D = true;
}

void Camera::to3D()
{
    if(!is2D)
        return;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(saved_modelview);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(saved_projection);
    is2D = false;
}

void Camera ::setProjection(double ViewAngle, double Aspect, double NearDist, double FarDist)
{
    viewAngle = ViewAngle;
    aspect = Aspect;
    nearDist = NearDist;
    farDist = FarDist;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, aspect, nearDist, farDist);
}

void Camera::setModelView(glm::dvec3 Eye, glm::dvec3 Look, glm::dvec3 Up)
{
    eye = Eye;
    look = Look;
    up = Up;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);
}

void Camera::move(glm::dvec3 displacement) const
{
    if(!enable_key)
        return;
    GLdouble temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-displacement.x, -displacement.y, -displacement.z);
    glMultMatrixd(temp);
}

void Camera::roll(double angle) const
{
    if(!enable_key)
        return;
    GLdouble temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-angle, 1.0, .0, .0);
    glMultMatrixd(temp);
}

void Camera::pitch(double angle) const
{
    if(!enable_key)
        return;
    GLdouble temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-angle, .0f, 1.f, .0f);
    glMultMatrixd(temp);
}

void Camera::yaw(double angle) const
{
    if(!enable_key)
        return;
    GLdouble temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-angle, .0f, .0f, 1.f);
    glMultMatrixd(temp);
}