#ifndef _CAMERA_CONTROL_HPP_
#define _CAMERA_CONTROL_HPP_

#include "SDL.h"
#include "camera.hpp"

namespace NEWTON {

class CameraControl
{
public:
    CameraControl();
	CameraControl::~CameraControl() {}
	void update(real_t dt);
    void handle_event( const SDL_Event& event );
    Camera camera;
	int body_focus;

private:
    enum Direction { DZERO=0, DPOS=1, DNEG=2 };
    enum Rotation { RNONE, RPITCHYAW, RROLL };

    Direction direction[3];
    Rotation rotation;
};

} /* NEWTON */

#endif /* _CAMERA_CONTROL_HPP_ */