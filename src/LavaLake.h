#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "RandFloat.h"

class LavaLake : public Object
{
public:
    LavaLake(float height,float size,float radius,float totalTime,int texture);
    ~LavaLake();

    void Draw();
    void Update(const float& dT);

private:
    float height;
    float size;

    float radius;
    float current_time;
    float total_time;

    int texture;
};
