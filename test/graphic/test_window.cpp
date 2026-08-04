#include "gkit/graphic/VertexBufferLayout.hpp"
#include "gkit/graphic/render/Renderer.hpp"
#include "graphic/backend/opengl/Texture.hpp"

#include <filesystem>
#include <iostream>

#include "SDL3/SDL.h"
#include <glad/gl.h>

int main(int argc, char* argv[]) {
    // Get executable directory for resource paths
    std::filesystem::path exe_path = argv[0];
    // exe at bin/.../test/test_window.exe, go up 4 levels to reach project root
    std::filesystem::path resource_base = exe_path.parent_path().parent_path().parent_path().parent_path() / "test";

#pragma region Init
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    // Request OpenGL 4.6 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    int screen_width  = gkit::graphic::SCR_WIDTH;
    int screen_height = gkit::graphic::SCR_HEIGHT;

    SDL_Window* window = SDL_CreateWindow("OpenGL Window", screen_width, screen_height, SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize GLAD
    if (!gladLoadGL(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << '\n';
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';

#pragma endregion

    {
        auto& renderer = gkit::graphic::Renderer::instance();
        renderer.init(); // default OpenGL backend

        auto& device = renderer.get_device();

#pragma region triangle
        // Colored triangle vertex data (position + color)
        std::vector<float> tri_vertices = {// positions            // colors
                                           0.0f,
                                           0.5f,
                                           0.0f,
                                           1.0f,
                                           0.0f,
                                           0.0f, // top: red
                                           -0.4f,
                                           -0.25f,
                                           0.0f,
                                           0.0f,
                                           1.0f,
                                           0.0f, // bottom-left: green
                                           0.4f,
                                           -0.25f,
                                           0.0f,
                                           0.0f,
                                           0.0f,
                                           1.0f}; // bottom-right: blue

        std::vector<uint32_t> tri_indices = {0, 1, 2};

        gkit::graphic::VertexBufferLayout tri_layout;
        tri_layout.push<float>(3); // position
        tri_layout.push<float>(3); // color

        // load shader source
        auto tri_shader = device.create_shader((resource_base / "graphic" / "color_triangle.shader").string());

        // Full-screen quad vertex data (post-processing)
        std::vector<float> quad_vertices = {// positions                    // tex coords
                                            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                            1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};

        std::vector<uint32_t> quad_indices = {0, 1, 2, 2, 3, 0};

        gkit::graphic::VertexBufferLayout quad_layout;
        quad_layout.push<float>(3);
        quad_layout.push<float>(2);

        // load post-processing shader
        auto post_shader = device.create_shader((resource_base / "graphic" / "post_process.shader").string());
#pragma endregion

#pragma region framebuffer
        // FBO is half the window size.
        const int fbo_width  = screen_width;
        const int fbo_height = screen_height;
        auto fbo             = device.create_frame_buffer(fbo_width, fbo_height);
        gkit::graphic::opengl::Texture fbo_texture(" ", gkit::graphic::TextureType::TextureFramebuffer);
        auto rbo = device.create_render_buffer(fbo_width, fbo_height);
        fbo->attach_color_texture(fbo_texture, 0);
        fbo->attach_depth_stencil(*rbo);
        fbo->check();
#pragma endregion

#pragma region render_objects
        // Reusable draw units: user provides data arrays + material; VAO/VBO/IBO hidden.

        // Triangle material
        gkit::graphic::Material tri_material;
        tri_material.shader = tri_shader.get();

        // Post-processing quad material (samples the FBO texture)
        gkit::graphic::Material post_material;
        post_material.shader        = post_shader.get();
        post_material.textures[0]   = &fbo_texture;
        post_material.texture_count = 1;
        post_material.uniforms.values.push_back({"screenTexture", 0});

        gkit::graphic::RenderObject triangle_obj(tri_vertices, tri_indices, tri_layout, tri_material);
        triangle_obj.clear = true; // clear the FBO color/depth before drawing

        gkit::graphic::RenderObject quad_obj(quad_vertices, quad_indices, quad_layout, post_material);
#pragma endregion

        // Main loop
        bool quit = false;
        SDL_Event event;
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.key == SDLK_ESCAPE) {
                        quit = true;
                    }
                }
            }

            // Submit reusable render objects; Renderer enqueues them and flush() executes.
            // Draw 1: triangle to the FBO (target = fbo)
            renderer.draw(triangle_obj, fbo.get(), {0, 0, fbo_width, fbo_height});
            // Draw 2: post-processing quad to screen (samples fbo texture)
            renderer.draw(quad_obj, nullptr, {0, 0, screen_width, screen_height});

            renderer.flush();

            // Swap buffers
            SDL_GL_SwapWindow(window);
        }
    }

    // Cleanup
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Window closed successfully" << '\n';
    return 0;
}
