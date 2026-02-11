#include "../include/window.h"
#include "core/include/graphic/window.hpp"
#include <SDL3/SDL_video.h>
#include <vector>

struct Gkit_Window {
    gkit::Window* instance;
};

Gkit_Window* gkit_window_new(const char* title, int width, int height) {
    auto window = new Gkit_Window;
    window->instance = new gkit::Window(title, width, height);
    return window;
}

Gkit_Window* gkit_window_new_with_flags(const char* title, int width, int height, SDL_WindowFlags flag) {
    auto window = new Gkit_Window;
    window->instance = new gkit::Window(title, width, height, {flag});
    return window;
}

void gkit_window_delete(Gkit_Window* window) {
    delete window->instance;
    delete window;
}

void gkit_window_show(Gkit_Window* window) {
    window->instance->show();
}

void gkit_window_hide(Gkit_Window* window) {
    window->instance->hide();
}

