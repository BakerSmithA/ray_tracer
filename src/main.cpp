#include "rendering/renderer.h"
#include "models/cornel_box.h"
#include "models/textured_test_model.h"
#include "models/star_model.h"
#include "models/saturn_model.h"
#include "models/sphere_model.h"
#include <glm/glm.hpp>
#include <SDL.h>

#define SCREEN_WIDTH 320 * 1
#define SCREEN_HEIGHT 256 * 1
#define FULLSCREEN_MODE false
#define MAX_NUM_RAY_BOUNCES 3
#define NUM_SHADOW_RAYS 0
#define NUM_SAMPLES 8

// /*Place updates of parameters here*/
void update(Camera &camera) {
    static int t = SDL_GetTicks();
    /* Compute frame time */
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;

    std::cout << "Render time: " << dt << " ms" << std::endl;

    // Translate Camera Position
    const uint8_t* scancodes = SDL_GetKeyboardState(NULL);
    if(scancodes[SDL_SCANCODE_DOWN]) camera.pos.z -= 0.01;
    if(scancodes[SDL_SCANCODE_UP]) camera.pos.z += 0.01;
    if(scancodes[SDL_SCANCODE_RIGHT]) camera.pos.x += 0.01;
    if (scancodes[SDL_SCANCODE_LEFT]) camera.pos.x -= 0.01;
    if(scancodes[SDL_SCANCODE_A]) camera.pos.y -= 0.01;
    if(scancodes[SDL_SCANCODE_D]) camera.pos.y += 0.01;
}

int main(int argc, char* argv[]) {
    //Scene scene = cornel_box();
    //Scene scene = textured_test_scene();
    //Scene scene = star_scene();
    //Scene scene = saturn_scene();
    Scene scene = sphere_scene();
    Camera cam = Camera(vec4(0, 0, -2.3, 1), SCREEN_WIDTH / 2, MAX_NUM_RAY_BOUNCES);
    //Camera cam = Camera(vec4(0, 0, -1.5, 1), SCREEN_WIDTH / 2, MAX_NUM_RAY_BOUNCES);
    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    while (NoQuitMessageSDL()) {
        update(cam);
        render(scene, cam, screen, NUM_SAMPLES, NUM_SHADOW_RAYS);
        SDL_Renderframe(screen);
    }
}
