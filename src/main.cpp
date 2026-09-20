#include <glad/gl.h>

#include "core/window.hpp"
#include "core/debug.hpp"
#include "core/logger.hpp"
#include "renderer/shader.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/buffer_layout.hpp"

int main() {
    Log::setLevel(Log::Level::Debug);

    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL 4.3");

    KINFO("OpenGL initialized");

    GLErrorHandler::init();

    glViewport(0, 0, window.getWidth(), window.getHeight());

    // ----------------
    // Rectangle vertices
    // ----------------
    constexpr float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};

    constexpr unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    // ----------
    // VBO
    // ----------
    VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);

    // ----------
    // Layout
    // ----------
    BufferLayout layout = {{"aPos", GL_FLOAT, 2}};

    // ----------
    // VAO
    // ----------
    VertexArray vao;

    vao.addVertexBuffer(vbo, layout);

    // ----------
    // IBO
    // ----------
    IndexBuffer ibo(indices, 6);

    // ----------
    // Shader
    // ----------
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    // ----------
    // Render loop
    // ----------
    while (!window.shouldClose()) {
        // Clear first
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader
        shader.use();

        shader.setVec4("myColor", 0.5f, 0.0f, 0.0f, 1.0f);

        // Bind geometry
        vao.bind();
        ibo.bind();

        // Draw
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}