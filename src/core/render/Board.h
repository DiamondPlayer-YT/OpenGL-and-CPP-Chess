#pragma once
#include <GL/glew.h>
#include "themes/ThemeBase.h"
#include "../util/Vector3.h"

class Board {
public:
    Board(ThemeBase* theme);
    ~Board();

    void Draw();

private:
    void DrawSquare(int row, int col, const Vec3& color);

    ThemeBase* theme;

    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;

    GLint uColorLocation;
    GLint uOffsetLocation;
    GLint uScaleLocation;
};
