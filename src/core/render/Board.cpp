#include "Board.h"
#include <GL/glew.h>
#include <iostream>

static GLuint CompileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
    }
    return shader;
}

static GLuint CreateShaderProgram(const char* vertSrc, const char* fragSrc) {
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader linking error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

Board::Board(ThemeBase* theme) : theme(theme) {
    // ----- Vertex data for unit square -----
    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ----- Embedded shaders -----
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        uniform vec2 uOffset;
        uniform vec2 uScale;
        void main() {
            vec2 pos = aPos * uScale + uOffset;
            gl_Position = vec4(pos, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )";

    shaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);

    uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
    uOffsetLocation = glGetUniformLocation(shaderProgram, "uOffset");
    uScaleLocation = glGetUniformLocation(shaderProgram, "uScale");
}

Board::~Board() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
}

void Board::Draw() {
    glUseProgram(shaderProgram);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            bool isLight = (row + col) % 2 == 0;
            Vec3 color = isLight ? theme->GetLightSquareColor() : theme->GetDarkSquareColor();
            DrawSquare(row, col, color);
        }
    }

    glUseProgram(0);
}

void Board::DrawSquare(int row, int col, const Vec3& color) {
    float squareSize = 2.0f / 8.0f;

    float offsetX = -1.0f + col * squareSize;
    float offsetY = 1.0f - (row + 1) * squareSize;

    glUniform3f(uColorLocation, color.r, color.g, color.b);
    glUniform2f(uOffsetLocation, offsetX, offsetY);
    glUniform2f(uScaleLocation, squareSize, squareSize);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
