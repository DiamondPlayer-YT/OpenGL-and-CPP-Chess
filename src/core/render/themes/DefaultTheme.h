#pragma once
#include "ThemeBase.h"

class DefaultTheme : public ThemeBase {
public:
    Vec3 GetLightSquareColor() const override { return { 0.9f, 0.9f, 0.9f }; }
    Vec3 GetDarkSquareColor()  const override { return { 0.2f, 0.2f, 0.2f }; }
    Vec3 GetBackgroundColor()  const override { return { 0.1f, 0.1f, 0.1f }; }
};
