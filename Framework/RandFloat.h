// Functions to generate random floats

#pragma once

// returns a random float between 0.f and 1.f
inline float RandFloat()
{
    return (float)rand()/RAND_MAX;
}

// returns a random float in the range of min to max
inline float RandFloatR(float min,float max)
{
    return min + (max - min) * RandFloat();
}
