#include <glm/glm.hpp>
#include <vector>
#include "geometry/primitives/triangle.h"
#include "geometry/primitives/sphere.h"
#include "geometry/object.h"
#include "lights/point_light.h"
#include "lights/directional_light.h"
#include "lights/ambient_light.h"
#include "shaders/diffuse.h"
#include "shaders/blinn.h"
#include "shaders/mirror.h"
#include "shaders/mix.h"
#include "shaders/refraction.h"
#include "shaders/fresnel.h"
#include "shaders/glass.h"
#include "shaders/flat_color.h"
#include "shaders/smoke.h"
#include "shaders/projection.h"
#include "rendering/renderer.h"
#include "models/cornel_box.h"
#include "models/textured_test_model.h"
#include "models/saturn_model.h"
#include "models/star_model.h"
#include "models/sphere_model.h"
#include "models/volume_model.h"
#include <glm/glm.hpp>
#include <SDL.h>

#define SCREEN_WIDTH 500//320 * 1
#define SCREEN_HEIGHT 500//256 * 1
#define FULLSCREEN_MODE false
#define MAX_NUM_RAY_BOUNCES 5
#define NUM_SHADOW_RAYS 1
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

    const float move_delta = 0.025f;
    const float yaw_delta = 0.07f;

    if(scancodes[SDL_SCANCODE_DOWN])  camera.move_forward(-move_delta); // Backwards
    if(scancodes[SDL_SCANCODE_UP])    camera.move_forward(move_delta);  // Forwards
    if(scancodes[SDL_SCANCODE_A])     camera.move_right(-move_delta);   // Left
    if(scancodes[SDL_SCANCODE_D])     camera.move_right(move_delta);    // Right
    if(scancodes[SDL_SCANCODE_W])     camera.move_down(-move_delta);    // Up
    if(scancodes[SDL_SCANCODE_S])     camera.move_down(move_delta);     // Down
    if(scancodes[SDL_SCANCODE_LEFT])  camera.turn(-yaw_delta);          // Pan Left
    if(scancodes[SDL_SCANCODE_RIGHT]) camera.turn(yaw_delta);           // Pan Right
}

int main(int argc, char* argv[]) {
    //Scene scene = cornel_box();
    //Scene scene = textured_test_scene();
    //Scene scene = star_scene();
    //Scene scene = saturn_scene();
    //Scene scene = sphere_scene();
    Scene scene = volume_scene();
    Camera cam = Camera(vec4(0, 0, -2.3, 1), SCREEN_WIDTH / 2, MAX_NUM_RAY_BOUNCES);
    //Camera cam = Camera(vec4(0, 0, -1.5, 1), SCREEN_WIDTH / 2, MAX_NUM_RAY_BOUNCES);
    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    while (NoQuitMessageSDL()) {
        update(cam);
        render(scene, cam, screen, NUM_SAMPLES, NUM_SHADOW_RAYS);
        SDL_Renderframe(screen);
    }
}
