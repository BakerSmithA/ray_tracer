#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "models/TestModelH.h"
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include "mass.h"
#include "ray.h"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

// returns: a 4D direction vector from the camera's origin to a pixel.
vec4 primary_ray(int x_pixelCoordinate, int y_pixelCoordinate, int width, int height, float focal_length, vec4 camera_pos) {
    float x_cameraCoordinate = x_pixelCoordinate - width/2;
    float y_cameraCoordinate = y_pixelCoordinate - height/2;
    vec4 dir = vec4(x_cameraCoordinate - camera_pos.x, y_cameraCoordinate - camera_pos.y, focal_length - camera_pos.z, 1);
    return glm::normalize(dir);
}

vec3 color(vec4 ray, vec4 mass_pos) {
    float dist = orthogonal_dist(mass_pos, ray);
    float x = (dist - 0.55) * 1.5;
    vec3 white(1,1,1);
    vec3 black(0,0,0);
    return glm::mix(white, black, x);
}

/*Place your drawing here*/
void draw(screen* screen, float focal_length, vec4 camera_pos, vec4 mass_pos) {
    /* Clear buffer */
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    for (int y=0; y<SCREEN_HEIGHT; y++) {
        for (int x=0; x<SCREEN_WIDTH; x++) {
            vec4 dir = primary_ray(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, focal_length, camera_pos);
            vec3 col = color(dir, mass_pos);
            PutPixelSDL(screen, x, y, col);
        }
    }
}

/*Place updates of parameters here*/
void update(vec4& mass_pos) {
    static int t = SDL_GetTicks();
    /* Compute frame time */
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;

    std::cout << "Render time: " << dt << " ms" << std::endl;

    //Translate Camera Position
    const uint8_t* scancodes = SDL_GetKeyboardState(NULL);
    if(scancodes[SDL_SCANCODE_DOWN]) mass_pos.z -= 0.01;
    else if(scancodes[SDL_SCANCODE_UP]) mass_pos.z += 0.01;
    else if(scancodes[SDL_SCANCODE_RIGHT]) mass_pos.x += 0.01;
    else if (scancodes[SDL_SCANCODE_LEFT]) mass_pos.x -= 0.01;
}

int main(int argc, const char* argv[]) {
    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    float focal_length = SCREEN_WIDTH/2;
    vec4 camera_pos(0, 0, -2.25, 1);

    vec4 mass_pos(0, 0, -0.5, 1.0);

    while(NoQuitMessageSDL()){
        update(mass_pos);
        draw(screen, focal_length, camera_pos, mass_pos);
        SDL_Renderframe(screen);
    }

    SDL_SaveImage(screen, "screenshot.bmp");

    KillSDL(screen);
    return 0;
}
