#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gkit_Application Gkit_Application;

Gkit_Application* gkit_application_new();
void gkit_application_delete(Gkit_Application* app_ptr);

#ifdef __cplusplus
}
#endif