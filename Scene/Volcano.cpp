#include "Volcano.h"

Volcano::Volcano(float state[3][3],int rock_texture,int lava_texture)
    : Object(state)
    , rock_texture(rock_texture)
    , lava_texture(lava_texture)
{
    particles = new VolcanoParticle[particle_count];
    projectiles = new VolcanoProjectile[projectile_count];

    for (int i = 0 ; i < particle_count ; i++)
    {
        particles[i] = VolcanoParticle();
    }

    for (int i = 0 ; i < projectile_count ; i++)
    {
        projectiles[i] = VolcanoProjectile(lava_texture);
    }
}

Volcano::~Volcano()
{
    delete particles;
    delete projectiles;
}

// draw the volcano
void Volcano::Draw()
{
    // set material properties
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    float mat_ambient[] = { (float)200/255, (float)200/255, (float)50/255, 1.f };
    float mat_diffuse[] = { (float)255/255, (float)255/255, (float)100/255, 1.f };
    float mat_spec[]    = { 0.0f, 0.0f, 0.0f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);

    glPushMatrix();

    // draw the volcano's base
    glColor3ub(184,67,44);
    ConeFrustum(0.5f,0.6f,rock_texture);
    ConeFrustum(0.4f,0.55f,rock_texture);
    ConeFrustum(0.3f,0.5f,rock_texture);

    // draw the volcano's crater
    ConeFrustum(-0.04f,0.75f,rock_texture);
    glDisable(GL_LIGHTING); // disable lighting for the lava texture
    glColor3ub(255,255,255);
    ConeFrustum(0.0f,0.0f,lava_texture);
    glEnable(GL_LIGHTING);

    glPopMatrix();

    // draw all particles
    glTranslatef(0.f,1.3f,0.f);
    glScalef(0.005f,0.005f,0.005f);
    for (int i = 0 ; i < particle_count ; i++)
    {
        particles[i].Display();
    }

    // draw all projectiles
    glTranslatef(0.f,-40.f,0.f);
    for (int i = 0 ; i < projectile_count ; i++)
    {
        projectiles[i].Display();
    }

    glPopAttrib();
}

void Volcano::Update(const float& dT)
{
    // update all particles
    for (int i = 0 ; i < particle_count ; i++)
    {
        particles[i].Update(dT);
    }

    // update all projectiles
    for (int i = 0 ; i < projectile_count ; i++)
    {
        projectiles[i].Update(dT);
    }
}

// draws a textured truncated cone with bottom face radius of 1, and top face radius of ratio
void Volcano::ConeFrustum(float height,float ratio,int texture)
{
    const float res = (2*(float)M_PI)/segments;
 // const float norm_y = atan(height/(1-ratio)); // Using this as the Y component of the normals makes the lighting between other connected ConeFrustum()s clash!

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLE_STRIP);
        for (float a1 = 0.f, a2 = res/2 ; a1 <= 2*M_PI ; a1 += res, a2 += res)
        {

            // lower circle
            glTexCoord2f(cos(a1),sin(a1));
            glNormal3f(cos(a1),0.f,sin(a1));
            glVertex3f(cos(a1),0.f,sin(a1));

            // upper circle
            glTexCoord2f(ratio*cos(a2),ratio*sin(a2));
            glNormal3f(ratio*cos(a2),0,ratio*sin(a2));
            glVertex3f(ratio*cos(a2),height,ratio*sin(a2));
        }
        glTexCoord2f(1.f,0.f);
        glNormal3f(1.f,0.f,0.f);
        glVertex3f(1.f,0.f,0.f);

        glTexCoord2f(ratio*cos(res/2),ratio*sin(res/2));
        glNormal3f(ratio*cos(res/2),0.f,ratio*sin(res/2));
        glVertex3f(ratio*cos(res/2),height,ratio*sin(res/2));
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // transform to line up the bottom of the next Base() with the top of this one
    glTranslatef(0.f,height,0.f);
    glScalef(ratio,1.f,ratio);
    glRotatef(180.f/segments,0.f,1.f,0.f);
}

// if 'V' is pressed, inform the VolcanoEruptionEffects
void Volcano::HandleKey(int key,int state,int x,int y)
{
    if (key != 'v' && key != 'V')
    {
        return;
    }

    for (int i = 0 ; i < particle_count ; i++)
    {
        particles[i].ToggleEruption(state);
    }
    for (int i = 0 ; i < projectile_count ; i++)
    {
        projectiles[i].ToggleEruption(state);
    }
}
