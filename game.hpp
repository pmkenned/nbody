#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <limits>

#include "camera_control.hpp"
#include "system.hpp"
#include "integrator.hpp"
#include "vector.hpp"
#include "mesh.hpp"
#include "matrix.hpp"

namespace NEWTON {

typedef std::numeric_limits< double > dbl;

class GameObject {
public:

	GameObject(bool is_body, size_t body_num, real_t r) : _is_body(is_body), body_num(body_num), radius(r) {
		mesh.construct_sphere(r);
	}

	void subdivide_mesh() {
		mesh.subdivide();
	}
	void render() {
		mesh.render();
	}
	void make_spherical() {
		mesh.make_spherical(radius);
	}
	bool is_body() { return _is_body; }
	Vector3 const & get_position() { return position; }
	size_t get_body_num() { return body_num; }
private:
	Mesh mesh;
	Vector3 position;
	bool _is_body;
	size_t body_num;
	real_t radius;
};

class Game {
public:
	void initialize();
	void update(real_t dt);
	void render();
	void handle_event(SDL_Event event);
private:
	System sys;
	RungeKuttaIntegrator runge_kutta_integrator;
	CameraControl camera_control;
	std::vector<GameObject> objects;

	bool engines_on;
	real_t engine_thrust;
};

}

#endif