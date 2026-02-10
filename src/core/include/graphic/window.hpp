#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <string>
#include <memory>
#include <vector>

namespace gkit {

    class Window {
    public:
        Window(std::string title, int width, int height) noexcept;
        Window(std::string title, int width, int height, std::vector<SDL_WindowFlags> flags) noexcept;
        virtual ~Window() = default;
        
    public:
        void show() noexcept;
        void hide() noexcept;

    private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_ptr;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_ptr;
    }; // class Window

} // namespace gkit