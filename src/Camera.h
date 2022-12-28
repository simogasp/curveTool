#pragma once

// for mac osx
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
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
#include <glm/glm.hpp>

class Camera
{
    private:
        glm::dvec3 eye, look, up;
        GLdouble viewAngle, aspect, nearDist, farDist;
        bool enable_key, is2D;
        GLdouble width, height;
        GLdouble saved_projection[16];
        GLdouble saved_modelview[16];

    public:
        /**
         * Initializes a camera for the window
         * @param width The width of the window (pixels)
         * @param height The height of the window (pixels)
         */
        Camera(int Width, int Height);
        /**
         * Switches to 2D view
         */
        void to2D();
        /**
         * Restores the 3D view.
         */
        void to3D();
        /**
         * Sets up the model view matrix
         * @param eye Position of the eye
         * @param look dvec3 to look at from the eye
         * @param up The look-up vector
         */
        void setModelView(glm::dvec3 eye, glm::dvec3 look, glm::dvec3 up);
        /**
         * Sets up the projection matrix
         * @param viewAngle Viewing angle
         * @param aspect Aspect ratio
         * @param nearDist near distance of the camera
         * @param farDist far distance of the camera
         */
        void setProjection(double ViewAngle, double Aspect, double NearDist, double FarDist);
        /**
         * Rotate around x
         * @param angle angle to rotate by
         */
        void roll(double angle) const;
        /**
         * Rotate around y
         * @param angle angle to rotate by
         */
        void pitch(double angle) const;
        /**
         * Rotate around z
         * @param angle angle to rotate by
         */
        void yaw(double angle) const;
        /**
         * Moves the camera by adding the given displacement
         * @param displacement Displacement vector which specifies additions to the camera position
         */
        void move(glm::dvec3 displacement) const;
};
