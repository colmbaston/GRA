#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "VolcanoEruptionEffect.h"
#include "RandFloat.h"

class VolcanoParticle : public VolcanoEruptionEffect
{
public:
    VolcanoParticle();

    void Reset();

    void Draw();
    void Update(const float& dT);

private:
    float radius;
    float colour[4];

    float v_speed,v_speed_min,v_speed_max;
    float fade,fade_min,fade_max;
};
