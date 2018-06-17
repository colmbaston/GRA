#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "VectorMaths.h"
#include "RandFloat.h"

class Bat : public Object
{
public:
    Bat(float state[3][3],float r,float g,float b,float radius,float total_time,bool direction);
    ~Bat();

    void Draw();
    void Update(const float& dT);

private:
    float x,z;
    float r,g,b;

    void Wing();
    void Limbs();
    void Body();

    void Triangle(const float* a,const float* b,const float* c); // for ease of drawing the many triangles of the wings
    void Cylinder(float h,float r,int segments); // for drawing limbs and the body

    // arrays to store wing vertices
    static const int vertex_count = 25;
    float wing_vertices_top[vertex_count+4][3];
    float wing_vertices_bottom[vertex_count][3];

    float wing_angle,wing_angle_min,wing_angle_max;
    float flap_speed,flap_speed_min,flap_speed_max;

    bool direction;
    float radius;
    float current_time,total_time;
};
