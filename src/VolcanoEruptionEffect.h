/*  abstract class for eruption effects to inherit from

    defines the mechanism by which eruption is toggled by key pre
*/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"

class VolcanoEruptionEffect : public Object
{
public:
    VolcanoEruptionEffect()
        : keyPressed(0)
        , last_keyPressed(0)
        , erupting(false) {}

    ~VolcanoEruptionEffect() {}

    virtual void Reset() {};

    // updates eruption state by setting flag
    void ToggleEruption(int state)
    {
        keyPressed = state;
    }

    // checks keyPressed flag
    // if true this call, and false last call, flip eruption boolean
    void UpdateErupting()
    {
        if (keyPressed)
        {
            if (!last_keyPressed)
            {
                erupting = !erupting;
            }
        }
        last_keyPressed = keyPressed;
    }

protected:
    int keyPressed,last_keyPressed;
    bool erupting;
};
