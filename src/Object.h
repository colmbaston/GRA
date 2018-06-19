
/*
	This is the class you will use to add objects to your scene.
	First create a new class for the object that you want to draw.
	Then make your new class inherit the 'Object' class.
	This way your new class will be able to override all the functions that an object may want to do.
	If you want to draw the new object you have created then you will need to override the Draw() function.
	If you want to update the object every frame you need to override the Update( const double& deltaTime ) function.
	If you want your object to recieve keyboard or mouse commands then override the Handle...() functions.
	Once you have done this you can add your new object to the scene with AddObjectToScene() in the Init function of MyScene.
*/

#pragma once

#include "Scene.h"

#define POSITION 0
#define SCALE    1
#define ROTATION 2

#define X        0
#define Y        1
#define Z        2

class Object
{
public:
    Object()
    {
        state[POSITION][X] = 0.f;
        state[POSITION][Y] = 0.f;
        state[POSITION][Z] = 0.f;

        state[SCALE][X] = 1.f;
        state[SCALE][Y] = 1.f;
        state[SCALE][Z] = 1.f;

        state[ROTATION][X] = 0.f;
        state[ROTATION][Y] = 0.f;
        state[ROTATION][Z] = 0.f;
    }

    Object(const float state[3][3])
    {
        for (int i = POSITION ; i <= ROTATION ; i++)
        {
            for (int j = X ; j <= Z ; j++)
            {
                this->state[i][j] = state[i][j];
                this->state[i][j] = state[i][j];
                this->state[i][j] = state[i][j];
            }
        }
    }

    // Transforms from object space to world space
    void Transform()
    {
        glTranslatef(state[POSITION][X],
                     state[POSITION][Y],
                     state[POSITION][Z]);

        glScalef(state[SCALE][X],
                 state[SCALE][Y],
                 state[SCALE][Z]);

        glRotatef(state[ROTATION][X],1.f,0.f,0.f);
        glRotatef(state[ROTATION][Y],0.f,1.f,0.f);
        glRotatef(state[ROTATION][Z],0.f,0.f,1.f);
    }

    void Display()
    {
        glPushMatrix();
        Transform();
		Draw();
        glPopMatrix();
    }

    void Billboard()
    {
        float modelview[16];
	    glGetFloatv(GL_MODELVIEW_MATRIX,modelview);

	    // undo all rotations (and scalings!)
	    for(int i = 0 ; i < 3 ; i++)
        {
	        for(int j = 0 ; j < 3 ; j++)
            {
		        if (i == j)
                {
		            modelview[i*4+j] = 1.0f;
                }
		        else
                {
		            modelview[i*4+j] = 0.0f;
                }
	        }
        }

	    // set the modelview with no rotations
	    glLoadMatrixf(modelview);
    }

    virtual ~Object(void) {};

    // Called each frame after updates are done to draw it to the screen
    virtual void Draw(){};

    // Called each frame to update.
    virtual void Update( const float& dT ){};

    // Called when keyboard input is received - x and y are the mouse coordinates at the time when the key was pressed
    virtual void HandleKey(int key, int state, int x, int y ){}
    // Called when a mouse button is pressed, or released.  x and y are the mouse position
    virtual void HandleMouseClick( int button, int state, int x, int y ){}
    // Called when the mouse moves if a button is pressed, x and y are the mouse position
    virtual void HandleMouseMotion( int x, int y ){}
    // Called every time the mouse moves, x and y are the mouse position.
    virtual void HandlePassiveMouseMotion( int x, int y ){}

    // State of the object in the world space
    float state[3][3];
};
