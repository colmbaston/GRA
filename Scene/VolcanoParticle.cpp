#include "VolcanoParticle.h"

VolcanoParticle::VolcanoParticle()
    : VolcanoEruptionEffect()
    , radius(30.f)
    , v_speed_min(0.03f)
    , v_speed_max(0.06f)
    , fade_min(0.0015f)
    , fade_max(0.003f)
{
    Reset();
}

void VolcanoParticle::Reset()
{
    // place each particle at random X and Z coords within a circle
    float rnd1 = RandFloat();
    float rnd2 = RandFloat();
    state[POSITION][X] = rnd1*radius*cos(rnd2*2*(float)M_PI);
    state[POSITION][Z] = rnd1*radius*sin(rnd2*2*(float)M_PI);

    // place each particle at a random Y coord between + and - radius
    // the particles are thus initially placed within a cylinder with height of 2*radius
    state[POSITION][Y] = radius * (2 * RandFloat() - 1);

    // give each particle a random v_speed and a random fade value
    v_speed = RandFloatR(v_speed_min,v_speed_max);
    fade    = RandFloatR(fade_min,fade_max);

    // give each particle a random colour
    switch (rand() % 3)
    {
    case 0:
        // red
        colour[0] = 255/(float)255;
        colour[1] = 0  /(float)255;
        colour[2] = 0  /(float)255;
        break;
    case 1:
        // dark orange
        colour[0] = 255/(float)255;
        colour[1] = 60 /(float)255;
        colour[2] = 0  /(float)255;
        break;
    case 2:
        // light orange
        colour[0] = 255/(float)255;
        colour[1] = 120/(float)255;
        colour[2] = 0  /(float)255;
        break;
    }

    // each particle begins fully opaque
    colour[3] = 1.f;

    // particles begin reset
    isReset = true;
}

// draws particles
void VolcanoParticle::Draw()
{
    // only draw if erupting is true, or particle hasn't been reset
    // this allows particles to finish their animations even if the volcano isn't erupting anymore
    // I.e. no new particles are spawned, but currently active ones still persist until they're reset
    if (!erupting && isReset)
    {
        return;
    }

    // these particles are simply coloured, billboarded squares
    Billboard();
    glColor4f(colour[0],colour[1],colour[2],colour[3]);
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
        glVertex3f(+1.f,+1.f,0.f);
        glVertex3f(-1.f,+1.f,0.f);
        glVertex3f(-1.f,-1.f,0.f);
        glVertex3f(+1.f,-1.f,0.f);
    glEnd();
    glEnable(GL_LIGHTING);
}

void VolcanoParticle::Update(const float& dT)
{
    // update erupting flag
    UpdateErupting();

    // as with Draw(), only update particles if the volcano is erupting, or they haven't finished their animation
    if (!erupting && isReset)
    {
        return;
    }

    // fade particle by its fade value, increasing its transparency
    colour[3] -= dT * fade;

    // if fully transparent, reset the particle
    if (colour[3] <= 0)
    {
        Reset();
        return;
    }

    // move particle upwards by its v_speed value
    state[POSITION][Y] += dT * v_speed;

    // set flag that this particle is active
    isReset = false;
}
