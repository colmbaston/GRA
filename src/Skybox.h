#pragma once

#include "Object.h"

class Skybox : public Object
{
public:
    Skybox(float size,int *textures);
    ~Skybox();
    void Draw();

private:
    float size;
    int *textures;
};
