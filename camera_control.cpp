#include "camera_control.hpp"

namespace NEWTON {

static const real_t TranslationSpeed = 1e9;
static const real_t RotationSpeed = 0.2;

CameraControl::CameraControl()
{
    rotation = RNONE;
    body_focus = 0;
}

void CameraControl::handle_event( const SDL_Event& event )
{
    int y;
    int key;
    int digit;
    Vector3 dir;

    switch ( event.type )
    {
    case SDL_KEYDOWN:
        key = event.key.keysym.sym;
        if( key >= SDLK_0 && key <= SDLK_9) {
            digit = key - SDLK_0;
            body_focus = digit;
        }
        break;
    case SDL_MOUSEWHEEL:
        y = event.wheel.y;
        if(y > 0) { // zoom in
            camera.position *= 0.9;
        } else if(y < 0) { // zoom out
            camera.position *= (1/0.9);
        }
        break;
 
    case SDL_MOUSEBUTTONDOWN:
        // enable rotation
        if ( event.button.button == SDL_BUTTON_RIGHT )
            rotation = RPITCHYAW;
        break;

    case SDL_MOUSEBUTTONUP:
        // disable rotation
        if ( event.button.button == SDL_BUTTON_RIGHT && rotation == RPITCHYAW )
            rotation = RNONE;
        break;

    case SDL_MOUSEMOTION:
        if ( rotation == RPITCHYAW ) {
            camera.phi += RotationSpeed * event.motion.xrel;
            camera.theta += RotationSpeed * event.motion.yrel;
        }
        break;

    default:
        break;
    }
}

void CameraControl::update(real_t dt) {
}

} // NEWTON