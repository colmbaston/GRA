/*
The starting point for your scene. It overides the Scene class. */

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <time.h>
#include "Scene.h"
#include "Skybox.h"
#include "Volcano.h"
#include "LavaLake.h"
#include "Bat.h"
#include "RandFloat.h"

class MyScene : public Scene
{
public:

    static const int skybox_size = 3000;

	// Constructor is called when the program starts
    MyScene( int argc, char **argv, const char *title, const int windowWidth, const int windowHeight );
	// Destructor is called when the program ends
    ~MyScene(){};

private:
	void Init();
    Bat* RandomBat();
};
