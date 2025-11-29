#pragma once
#include "../util/Vector3.h"

class ThemeBase {
public:
    virtual Vec3 GetLightSquareColor() const = 0;
    virtual Vec3 GetDarkSquareColor() const = 0;
    virtual Vec3 GetBackgroundColor() const { return { 0.1f, 0.1f, 0.1f }; }
};
