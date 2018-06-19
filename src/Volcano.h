#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "VolcanoParticle.h"
#include "VolcanoProjectile.h"

class Volcano : public Object
{
public:
    Volcano(float state[3][3],int rock_texture,int lava_texture);
    ~Volcano();

    void Draw();
    void Update(const float& dT);

    void HandleKey(int key,int state,int x,int y);

private:
    void ConeFrustum(float height,float ratio,int texture);

    static const int segments = 20;
    int rock_texture,lava_texture;

    static const int projectile_count = 500;
    VolcanoProjectile* projectiles;

    static const int particle_count = 2000;
    VolcanoParticle* particles;
};
