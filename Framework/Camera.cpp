
/*
This camera class controlls the position, view direction and projection of the camera for your scene.
You may change the implementation of these functions to improve the quality of the camera.
*/

#include "MyScene.h"
#include "Scene.h"
#include "Camera.h"
#include "VectorMaths.h"

Camera::Camera() : projection(true), wKey(0), sKey(0), aKey(0), dKey(0), qKey(0), eKey(0), cKey(0), last_cKey(0), currentButton(0), mouseX(0), mouseY(0)
{
    // set the camera position to start at (0,0,0)
    eye[0] = 0.0f;
    eye[1] = 900.f;
    eye[2] = 250.f;

    // set the view direction vector of the camera to be (0,0,-1)
    vd[0] = 0.0f;
    vd[1] = 0.0f;
    vd[2] = -1.0f;

    // set the planar forward direction vector of the camera to be (0,0,-1)
    forward[0] = 0.0f;
    forward[1] = 0.0f;
    forward[2] = -1.0f;

    // set the right vector to point along the x axis
    right[0] = 1.0f;
    right[1] = 0.0f;
    right[2] = 0.0f;

    // set the up vector of the camera to be up the y axis
    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;
}

Camera::~Camera()
{
}

void Camera::GetEyePosition( float &x, float &y, float &z) const
{
    x = eye[0];
    y = eye[1];
    z = eye[2];
}

void Camera::GetViewDirection( float &x, float &y, float &z) const
{
    x = vd[0];
    y = vd[1];
    z = vd[2];
}

void Camera::GetForwardVector( float &x, float &y, float &z) const
{
    x = forward[0];
    y = forward[1];
    z = forward[2];
}

void Camera::GetRightVector( float &x, float &y, float &z) const
{
    x = right[0];
    y = right[1];
    z = right[2];
}

void Camera::GetUpVecotor( float &x, float &y, float &z) const
{
    x = up[0];
    y = up[1];
    z = up[2];
}

void Camera::SetUpCamera()
{
    Projection(projection);
}

void Camera::Projection(bool perspective)
{
    int windowWidth = Scene::GetWindowWidth();
    int windowHeight = Scene::GetWindowHeight();

    // clear the old projection matrix by loading the identity matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float distance = sqrt(pow((float)MyScene::skybox_size*2,2)*3);
    if (perspective)
    {
        gluPerspective(90,(GLdouble)windowWidth/(GLdouble)windowHeight,1.0,distance);
    }
    else
    {
        glOrtho(-windowWidth/2,windowWidth/2,-windowHeight/2,windowHeight/2,1.0,distance);
    }

    // clear the old model view matrix by loading the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], eye[0]+vd[0], eye[1]+vd[1], eye[2]+vd[2], up[0], up[1], up[2]);
}

void Camera::SetViewport( const int& width, const int& height )
{
    // the screen has been resized so change the viewport (area openGL draws into) to cover the entire window.
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

void Camera::Update( const double& deltaTime )
{
    // Update the eye and lookat in the update function rather than the HandleKey function
    // to obtain smooth motion. This works because HandleKey is not called EVERY frame where as
    // Update is. Therefore we update the positions in Update to move the camera each frame, rather
    // than in the KeyPress function which would only update the camera every so often and give a
    // 'jittery' feel to the camera.

    // variable to control how fast the camera moves in any one direction
    float speed = 10.f;

    if ( aKey )
    {
        sub(eye, right, eye, speed);
    }

    if ( dKey )
    {
        add(eye, right, eye, speed);
    }

    if ( wKey )
    {
        add(eye, forward, eye, speed);
    }

    if ( sKey )
    {
        sub(eye, forward, eye, speed);
    }

    if ( qKey )
    {
        add(eye, up, eye, speed);
    }

    if ( eKey )
    {
        sub(eye, up, eye, speed);
    }

    if ( cKey )
    {
        if ( !last_cKey )
        {
            projection = !projection;
            Projection(projection);
        }
    }
    last_cKey = cKey;
}

void Camera::HandleKey( int key, int state, int x, int y )
{
    // toggle booleans to remember which key was pressed/released ready for the update function
    switch (key) {
        case 'A':
        case 'a':
            aKey = state;
            break;
        case 'D':
        case 'd':
            dKey = state;
            break;
        case 'W':
        case 'w':
            wKey = state;
            break;
        case 'S':
        case 's':
            sKey = state;
            break;
        case 'Q':
        case 'q':
            qKey = state;
            break;
        case 'E':
        case 'e':
            eKey = state;
            break;
        case 'C':
        case 'c':
            cKey = state;
            break;
        default:
            break;
    }
}

void Camera::HandleMouseClick( int button, int state, int x, int y )
{
    // when the mouse is clicked remember which button was pressed and where it was pressed
    currentButton = button;
    mouseX = x;
    mouseY = y;
}

void Camera::HandleMouseMotion( int x, int y )
{
    float rx, ry;
    float sensitivity = 0.01f; // speed of the camera moving

    // work out the difference between where the mouse was last used (mouseX, mouseY) to
    // position the view direction and the current position (x, y) the mouse is in
    rx =(float)(x-mouseX);
    ry =(float)(y-mouseY);

    // switch on which button was pressed and only do the update if the left button was pressed
    switch (currentButton) {
        case GLUT_LEFT_BUTTON:

            // add on the amount of change in to the left and right view direction of the camera
            if(rx>0)
                add(vd, right, vd, rx*sensitivity);
            else
                sub(vd, right, vd, rx*-sensitivity);
            // add on the amount of change in to the up and down view direction of the camera
            if(ry>0)
                sub(vd, up, vd, ry*sensitivity);
            else
                add(vd, up, vd, ry*-sensitivity);

            // normalise the view direction so it is length 1
            norm(vd);

            // use the view direction crossed with the up vector to obtain the corrected right vector
            cross(vd, up, right);

            // normalise the right vector
            norm(right);

/* As we want out camera to stay on the same plane at the same height (i.e. not move up and down the y axis) update a forward direction vector which can be used to move the camera. This forward vector moves the camera in the same direction as the view direction except it will not contain any y component so it cannot move off of its original height. This was we are free to look up and down however moving forward and back will not move us off of the camera plane. */

            forward[0] = vd[0];
            forward[2] = vd[2];
            norm(forward);
            break;
        case GLUT_RIGHT_BUTTON:
            break;
        case GLUT_MIDDLE_BUTTON:
            break;
        default:
            break;
    }

    mouseX = x;
    mouseY = y;
}

void Camera::HandlePassiveMouseMotion( int x, int y)
{
    // Copy the implementation of HandleMouseMotion into this function for a camera that does not require you to
    // click the button to move the view direction.
}
