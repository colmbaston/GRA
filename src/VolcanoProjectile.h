#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "VolcanoEruptionEffect.h"
#include "RandFloat.h"

class VolcanoProjectile : public VolcanoEruptionEffect
{
public:
    VolcanoProjectile();
    VolcanoProjectile(int texture);

    void Reset();

    void Draw();
    void Update(const float& dT);

private:
    static const int segments = 100;
    int texture;

    float angle;
    float size,size_min,size_max;
    float h_speed,h_speed_min,h_speed_max;
    float v_speed,v_speed_min,v_speed_max;
    float delay,delay_min,delay_max;
    float v_acceleration;
    float v_reset;
    bool  isReset;
};
