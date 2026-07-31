#include "gkit/graphic/Renderer.hpp"
#include "gkit/graphic/VertexBufferLayout.hpp"
#include "gkit/graphic/opengl/StateManager.hpp"
#include "gkit/graphic/opengl/Texture.hpp"

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
        auto& renderer      = gkit::graphic::Renderer::instance();
        auto& state_manager = gkit::graphic::opengl::StateManager::instance();
        renderer.init(); // default OpenGL backend

        auto& device = renderer.get_device();

#pragma region triangle
        // Colored triangle vertex data (position + color)
        float tri_vertices[] = {// positions            // colors
                                0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top: red
                                -0.4f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left: green
                                0.4f,  -0.25f, 0.0f, 0.0f, 0.0f, 1.0f}; // bottom-right: blue

        // index data
        unsigned int tri_indices[] = {0, 1, 2};

        auto tri_vao = device.create_vertex_array();
        auto tri_vbo = device.create_vertex_buffer(tri_vertices, sizeof(tri_vertices), false);
        auto tri_ibo = device.create_index_buffer(tri_indices, 3);

        gkit::graphic::VertexBufferLayout tri_layout;
        tri_layout.push<float>(3); // position
        tri_layout.push<float>(3); // color
        tri_vao->add_buffer(*tri_vbo, tri_layout);

        // load shader source
        auto tri_shader = device.create_shader((resource_base / "graphic" / "color_triangle.shader").string());

        // Full-screen quad vertex data (post-processing)
        float quad_vertices[] = {// positions                    // tex coords
                                 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                 1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};

        unsigned int quad_indices[] = {0, 1, 2, 2, 3, 0};

        auto quad_vao = device.create_vertex_array();
        auto quad_vb  = device.create_vertex_buffer(quad_vertices, sizeof(quad_vertices), false);
        auto quad_ib  = device.create_index_buffer(quad_indices, 6);

        gkit::graphic::VertexBufferLayout quad_layout;
        quad_layout.push<float>(3);
        quad_layout.push<float>(2);
        quad_vao->add_buffer(*quad_vb, quad_layout);

        // load post-processing shader
        auto post_shader = device.create_shader((resource_base / "graphic" / "post_process.shader").string());
#pragma endregion

#pragma region framebuffer
        auto fbo = device.create_frame_buffer(gkit::graphic::SCR_WIDTH, gkit::graphic::SCR_HEIGHT);
        gkit::graphic::opengl::Texture fbo_texture(" ", gkit::graphic::TextureType::TextureFramebuffer);
        auto rbo = device.create_render_buffer(gkit::graphic::SCR_WIDTH, gkit::graphic::SCR_HEIGHT);
        fbo->attach_color_texture(fbo_texture, 0);
        fbo->attach_depth_stencil(*rbo);
        fbo->check();
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

            fbo->bind();
            fbo->set_viewport(0, 0, screen_width, screen_height);
            renderer.clear(gkit::graphic::ClearFlags::All);
            // 1. Render to framebuffer
            tri_shader->bind();
            renderer.draw(*tri_vao, *tri_ibo, *tri_shader);

            // 2. Render to screen (post-processing)
            fbo->unbind();
            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width, screen_height);
            post_shader->bind();
            fbo_texture.bind(0);
            post_shader->set_uniform_1i("screenTexture", 0);
            renderer.draw(*quad_vao, *quad_ib, *post_shader);

            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width/2, screen_height/2);
            tri_shader->bind();
            renderer.draw(*tri_vao, *tri_ibo, *tri_shader);

            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width/4, screen_height/4);
            post_shader->bind();
            fbo_texture.bind(0);
            post_shader->set_uniform_1i("screenTexture", 0);
            renderer.draw(*quad_vao, *quad_ib, *post_shader);
            
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
