#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../render/themes/DefaultTheme.h"
#include "../render/Board.h"

// Create the window and initialize OpenGL
void CreateWindow(const char* title, int width, int height)
{
    if (!glfwInit()) {
        return;
    }

    // Modern OpenGL 3.3 Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // ---------- IMPORTANT: Initialize GLEW ----------
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        glfwTerminate();
        return;
    }
    // ------------------------------------------------

    // Optional: set viewport
    glViewport(0, 0, width, height);

    // Theme
    DefaultTheme theme;

    // Create board using modern OpenGL
    Board board(&theme);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // background = theme main color
        Vec3 bg = theme.GetBackgroundColor();
        glClearColor(bg.r, bg.g, bg.b, 1.0f);


        glClear(GL_COLOR_BUFFER_BIT);

        // ---------- draw the chessboard ----------
        board.Draw();
        // -----------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
