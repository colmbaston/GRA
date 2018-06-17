#include "Skybox.h"

Skybox::Skybox(float size,int* textures) : size(size), textures(textures)
{
    // bottom of the skybox starts at Y=0
    state[POSITION][Y] = size;
}

Skybox::~Skybox() {}

// draw skybox cube with textures
void Skybox::Draw()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);

    // front
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(+size,+size,-size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(-size,+size,-size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(-size,-size,-size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(+size,-size,-size);
    glEnd();
    // left
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(-size,+size,-size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(-size,+size,+size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(-size,-size,+size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(-size,-size,-size);
    glEnd();
    // right
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(+size,+size,+size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(+size,+size,-size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(+size,-size,-size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(+size,-size,+size);
    glEnd();
    // back
    glBindTexture(GL_TEXTURE_2D,textures[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(-size,+size,+size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(+size,+size,+size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(+size,-size,+size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(-size,-size,+size);
    glEnd();
    // top
    glBindTexture(GL_TEXTURE_2D,textures[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.f,1.f);
        glVertex3f(+size,+size,+size);
        glTexCoord2f(0.f,1.f);
        glVertex3f(-size,+size,+size);
        glTexCoord2f(0.f,0.f);
        glVertex3f(-size,+size,-size);
        glTexCoord2f(1.f,0.f);
        glVertex3f(+size,+size,-size);
    glEnd();
    // bottom (if there is one)
    if (textures[5] != NULL)
    {
        glBindTexture(GL_TEXTURE_2D,textures[5]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.f,1.f);
            glVertex3f(+size,-size,-size);
            glTexCoord2f(0.f,1.f);
            glVertex3f(-size,-size,-size);
            glTexCoord2f(0.f,0.f);
            glVertex3f(-size,-size,+size);
            glTexCoord2f(1.f,0.f);
            glVertex3f(+size,-size,+size);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
