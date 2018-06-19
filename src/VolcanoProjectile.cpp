#include "VolcanoProjectile.h"

VolcanoProjectile::VolcanoProjectile()
    : VolcanoEruptionEffect()
{}

VolcanoProjectile::VolcanoProjectile(int texture)
    : VolcanoEruptionEffect()
    , size_min(5.f)
    , size_max(20.f)
    , h_speed_min(0.25f)
    , h_speed_max(0.4f)
    , v_speed_min(0.4f)
    , v_speed_max(0.8f)
    , delay_min(0.f)
    , delay_max(3000.f)
    , v_acceleration(-0.001f)
    , v_reset(-250.f)
    , texture(texture)
{
    Reset();
}

void VolcanoProjectile::Reset()
{
    // each projectile begins at the same place: the volcano's crater
    state[POSITION][X] = 0.f;
    state[POSITION][Y] = 0.f;
    state[POSITION][Z] = 0.f;

    // randomise the angle that the projectile travels at, it's size, and its speed both horizontally and vertically
    angle = 2*(float)M_PI * RandFloat();
    size = RandFloatR(size_min,size_max);
    h_speed = RandFloatR(h_speed_min,h_speed_max);
    v_speed = RandFloatR(v_speed_min,v_speed_max);
    delay  = RandFloatR(delay_min,delay_max);

    // projectiles begin reset
    isReset = true;
}

void VolcanoProjectile::Draw()
{
    // only draw if erupting is true, or projectile hasn't been reset
    // this allows projectile to finish their animations even if the volcano isn't erupting anymore
    // I.e. no new projectile are spawned, but currently active ones still persist until they're reset
    if (!erupting && isReset || delay > 0)
    {
        return;
    }

    // projectiles are simply textures, billboarded circles
    Billboard();
    float res = (2*(float)M_PI)/segments;

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);

    // draw and texture the circle
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_POLYGON);
        for (float a = 0.f ; a <= 2*M_PI ; a += res)
        {
            glTexCoord2f(cos(a),sin(a));
            glVertex3f(size*cos(a),size*sin(a),0.f);
        }
        glTexCoord2f(size,0.f);
        glVertex3f(size,0.f,0.f);
    glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void VolcanoProjectile::Update(const float& dT)
{
    // update erupting flag
    UpdateErupting();

    // as with Draw(), only update projectiles if the volcano is erupting, or they haven't yet finished their animation
    if (!erupting && isReset)
    {
        Reset();
        return;
    }

    // count down on the delay
    if (delay > 0)
    {
        delay -= dT;
        return;
    }

    // update Y position by projectile's vertical speed
    state[POSITION][Y] += dT * v_speed;

    // update vertical speed by vertical acceleration
    v_speed += dT * v_acceleration;

    // when projectiles go below a threshold on the Y axis, reset them
    if (state[POSITION][Y] < v_reset)
    {
        Reset();
        return;
    }

    // update X and Z positions by horizontal speed and the angle
    state[POSITION][X] += dT * h_speed * cos(angle);
    state[POSITION][Z] += dT * h_speed * sin(angle);

    // set flag saying this particle has been reset
    isReset = false;
}
