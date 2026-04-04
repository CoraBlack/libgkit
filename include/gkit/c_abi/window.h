#pragma once

#include <SDL3/SDL_video.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gkit_Window Gkit_Window;

Gkit_Window* gkit_window_new(const char* title, int width, int height);
Gkit_Window* gkit_window_new_with_flags(const char* title, int width, int height, SDL_WindowFlags flag);
void gkit_window_delete(Gkit_Window* window);

void gkit_window_show(Gkit_Window* window);
void gkit_window_hide(Gkit_Window* window);

#ifdef __cplusplus
}
#endif