#include "LavaLake.h"


LavaLake::LavaLake(float height,float size,float radius,float totalTime,int texture)
    : size((float)M_SQRT2*size+radius) // ensures the lake stays within bounds of the skybox during animation
    , radius(radius)
    , total_time(totalTime)
    , texture(texture)
{
    state[POSITION][Y] = height;
    current_time = total_time * RandFloat();
}

LavaLake::~LavaLake() {}

// simply draws a large textured square
void LavaLake::Draw()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3ub(232,150,50);

    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(+size,0.f,-size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(-size,0.f,-size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(-size,0.f,+size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(+size,0.f,+size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

// translates along the XZ plane, and rotates about the Y axis to give impression of flowing lava
void LavaLake::Update(const float& dT)
{
    current_time = fmod(dT+current_time,total_time);

    float t = current_time/total_time;
    float angle = t*2*(float)M_PI;
    state[POSITION][X] = radius*cos(angle);
    state[POSITION][Z] = radius*sin(angle);
    state[ROTATION][Y] = t*360;
}
