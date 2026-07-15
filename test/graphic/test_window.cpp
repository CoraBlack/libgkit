#include "gkit/graphic/Renderer.hpp"
#include "gkit/graphic/Shader.hpp"
#include "gkit/graphic/opengl/FrameBuffer.hpp"
#include "gkit/graphic/opengl/IndexBuffer.hpp"
#include "gkit/graphic/opengl/RenderBuffer.hpp"
#include "gkit/graphic/opengl/StateManager.hpp"
#include "gkit/graphic/opengl/VertexArray.hpp"

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
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    // Request OpenGL 4.6 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    int screen_width  = gkit::graphic::opengl::SCR_WIDTH;
    int screen_height = gkit::graphic::opengl::SCR_HEIGHT;

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
#pragma region square
        // Triangle vertex data
        float pic_vertices[] = {// positions        // tex coordsc
                                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

        // index data
        unsigned int pic_indices[] = {0, 1, 2, 2, 3, 0};

        gkit::graphic::opengl::VertexArray pic_vao;
        gkit::graphic::opengl::buffer::VertexBuffer pic_vbo(pic_vertices, sizeof(pic_vertices));
        gkit::graphic::opengl::buffer::IndexBuffer pic_ibo(pic_indices, 6);

        gkit::graphic::opengl::buffer::VertexBufferLayout pic_layout;
        pic_layout.push<float>(3);
        pic_layout.push<float>(2);
        pic_vao.add_buffer(pic_vbo, pic_layout);

        // load shader source
        gkit::graphic::Shader pic_shader((resource_base / "graphic" / "texture.shader").string());
        gkit::graphic::opengl::Texture main_texture((resource_base / "graphic" / "container2.png").string(),
                                                    gkit::graphic::opengl::TextureType::Texture2D);

        // Full-screen quad vertex data (post-processing)
        float quad_vertices[] = {// positions                    // tex coords
                                 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                 1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};

        unsigned int quad_indices[] = {0, 1, 2, 2, 3, 0};

        gkit::graphic::opengl::VertexArray quad_vao;
        gkit::graphic::opengl::buffer::VertexBuffer quad_vb(quad_vertices, sizeof(quad_vertices));
        gkit::graphic::opengl::buffer::IndexBuffer quad_ib(quad_indices, 6);

        gkit::graphic::opengl::buffer::VertexBufferLayout quad_layout;
        quad_layout.push<float>(3);
        quad_layout.push<float>(2);
        quad_vao.add_buffer(quad_vb, quad_layout);

        // load post-processing shader
        gkit::graphic::Shader post_shader((resource_base / "graphic" / "post_process.shader").string());
#pragma endregion

#pragma region framebuffer
        gkit::graphic::opengl::buffer::FrameBuffer fbo(gkit::graphic::opengl::SCR_WIDTH,
                                                       gkit::graphic::opengl::SCR_HEIGHT);
        gkit::graphic::opengl::Texture fbo_texture(" ", gkit::graphic::opengl::TextureType::TextureFramebuffer);
        gkit::graphic::opengl::buffer::RenderBuffer rbo(gkit::graphic::opengl::SCR_WIDTH,
                                                        gkit::graphic::opengl::SCR_HEIGHT);
        fbo.attach_color_texture(fbo_texture, 0);
        fbo.attach_depth_stencil(rbo);
        fbo.check();
#pragma endregion

        auto& renderer      = gkit::graphic::Renderer::instance();
        auto& state_manager = gkit::graphic::opengl::StateManager::instance();

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

            fbo.bind();
            fbo.set_viewport(0, 0, screen_width, screen_height);
            renderer.clear(gkit::graphic::opengl::ClearFlags::All);
            // 1. Render to framebuffer
            pic_shader.bind();
            main_texture.bind(0);
            renderer.draw(pic_vao, pic_ibo, pic_shader);

            // 2. Render to screen (post-processing)
            fbo.unbind();
            renderer.clear(gkit::graphic::opengl::ClearFlags::All);
            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width / 2, screen_height / 2);
            state_manager.set_stencil_test(true);
            state_manager.set_stencil(gkit::graphic::opengl::CompareFunc::Always, 1, 0xFF);
            state_manager.set_stencil_op(gkit::graphic::opengl::StencilOp::Keep,
                                         gkit::graphic::opengl::StencilOp::Keep,
                                         gkit::graphic::opengl::StencilOp::Replace);
            state_manager.apply();
            pic_shader.bind();
            main_texture.bind(0);
            renderer.draw(pic_vao, pic_ibo, pic_shader);

            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width, screen_height);
            state_manager.set_stencil(gkit::graphic::opengl::CompareFunc::Equal, 1, 0xFF);
            state_manager.set_stencil_op(gkit::graphic::opengl::StencilOp::Keep,
                                         gkit::graphic::opengl::StencilOp::Keep,
                                         gkit::graphic::opengl::StencilOp::Keep);
            state_manager.apply();
            post_shader.bind();
            fbo_texture.bind(0);
            post_shader.set_uniform_1i("screenTexture", 0);
            renderer.draw(quad_vao, quad_ib, post_shader);

            gkit::graphic::opengl::viewport::set_viewport(0, 0, screen_width / 4, screen_height / 4);
            post_shader.bind();
            fbo_texture.bind(0);
            post_shader.set_uniform_1i("screenTexture", 0);
            renderer.draw(quad_vao, quad_ib, post_shader);

            state_manager.set_stencil_test(false);
            state_manager.apply();

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
