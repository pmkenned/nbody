#include "game.hpp"

namespace NEWTON {

void Game::initialize() {

	engines_on = false;
	engine_thrust = 1e6;

	camera_control.camera.position = Vector3(0.0, 0.0, 20*8.16520800e11);

//	sys.initialize();

	Vector3 earth_pos = Vector3(1.5210e11, 0.0, 0.0);
	Vector3 earth_vel = Vector3(0.0, 2.9300e4, 0.0);

	// all x-coordinates are aphelions
	// orbital speeds are speeds at aphelions

	size_t body_num;

	// body 0 is spaceship
	body_num = sys.add_body(30.3e3, earth_pos + Vector3(6556e3, 0.0, 0.0), earth_vel + Vector3(0.0, 7.796e3, 0.0), false);
	objects.push_back(GameObject(true, body_num, 5));

	body_num = sys.add_body(1.989e30,   Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0)); // SUN
	objects.push_back(GameObject(true, body_num, 696342e3));
	body_num = sys.add_body(3.3022e23,  Vector3(6.9817e10, 0.0, 0.0), Vector3(0.0, 3.886e4, 0.0)); // MERCURY
	objects.push_back(GameObject(true, body_num, 2439.7e3));
	body_num = sys.add_body(4.8676e24,  Vector3(1.0894e11, 0.0, 0.0), Vector3(0.0, 3.479e4, 0.0)); // VENUS
	objects.push_back(GameObject(true, body_num, 6051.8e3));
	body_num = sys.add_body(5.97219e24, earth_pos, earth_vel); // EARTH
	objects.push_back(GameObject(true, body_num, 6371.0e3));
	body_num = sys.add_body(7.3477e22,  earth_pos + Vector3(4.054e8, 0.0, 0.0), earth_vel + Vector3(0.0, 9.64e2, 0.0)); // MOON
	objects.push_back(GameObject(true, body_num, 1737.10e3));
	body_num = sys.add_body(6.4185e23,  Vector3(2.492e11, 0.0, 0.0), Vector3(0.0, 2.1977e4, 0.0)); // MARS
	objects.push_back(GameObject(true, body_num, 3389.5e3));
	body_num = sys.add_body(1.89813e27, Vector3(8.1652e11, 0.0, 0.0), Vector3(0.0, 1.2435e4, 0.0)); // JUPITER
	objects.push_back(GameObject(true, body_num, 69911e3));
	body_num = sys.add_body(5.6846e26,  Vector3(1.513e12, 0.0, 0.0), Vector3(0.0, 9.101e3, 0.0)); // SATURN
	objects.push_back(GameObject(true, body_num, 58232e3));
	body_num = sys.add_body(8.68e25,    Vector3(3.006e12, 0.0, 0.0), Vector3(0.0, 6.486e3, 0.0)); // URANUS
	objects.push_back(GameObject(true, body_num, 25362e3));
	body_num = sys.add_body(1.0243e26,  Vector3(4.538e12, 0.0, 0.0), Vector3(0.0, 5.385e3, 0.0)); // NEPTUNE
	objects.push_back(GameObject(true, body_num, 24622e3));

	sys.translate(-earth_pos);

	for(int j=0; j<3; j++) {
		for(size_t i=0; i < objects.size(); i++)
			objects[i].subdivide_mesh();
		for(size_t i=0; i < objects.size(); i++)
			objects[i].make_spherical();
	}

}

void Game::update(real_t dt) {
	if(engines_on)
		sys.bodies[0].thrust = engine_thrust*normalize(sys.bodies[0].velocity - sys.bodies[4].velocity);
	runge_kutta_integrator.integrate(sys, dt);
//	camera_control.update(dt);
}

void Game::handle_event(SDL_Event event) {

	int key;
	int digit;
//	Camera & cam = camera_control.camera;
//	int body_focus = camera_control.body_focus;
//	Vector3 & target = sys.bodies[body_focus].position;
	Vector3 sum;
	
	switch(event.type) {
		case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
		    if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) 
		    {
		        if( event.jaxis.axis == 0) {
					std::cout << "yaw: " << event.jaxis.value << std::endl;
		        }
		        if( event.jaxis.axis == 1) {
					std::cout << "pitch: " << event.jaxis.value << std::endl;
				}
		        if( event.jaxis.axis == 2) {
					std::cout << "roll: " << event.jaxis.value << std::endl;
		        }
			}
			break;
		case SDL_KEYDOWN:
			key = event.key.keysym.sym;
			if(key == SDLK_SPACE) {
				sum = sys.bodies[0].velocity - sys.bodies[4].velocity;
				std::cout << "Velocity: " << length(sum) << std::endl;
				if(!engines_on) {
					std::cout << "Engines at " << engine_thrust << " newtons of thrust along prograde vector." << std::endl;
					engines_on = true;
				}
			}
			break;
		case SDL_KEYUP:
			key = event.key.keysym.sym;
			if(key == SDLK_SPACE) {
				std::cout << "Engines off." << std::endl;
				sys.bodies[0].thrust = Vector3::Zero;
				engines_on = false;
			}
			break;
		default:
			break;
	}

	camera_control.handle_event(event);
}

void Game::render()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Set the background black
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear The Screen And The Depth Buffer

	Camera & cam = camera_control.camera;
	Vector3 right = Vector3(1.0, 0.0, 0.0);

	glLoadIdentity();
	glTranslated(-1*cam.position.x, -1*cam.position.y, -1*cam.position.z);
	glRotated(cam.theta, 1.0, 0.0, 0.0);
	glRotated(cam.phi, 0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	int body_focus = camera_control.body_focus;
	Vector3 & target = sys.bodies[body_focus].position;
	glTranslated(-target.x, -target.y, -target.z);

	glBegin(GL_POINTS);
	for(size_t i=0; i < sys.bodies.size(); i++) {
		Vector3 pos = sys.bodies[i].position;
		glVertex3d(pos.x, pos.y, pos.z);
	}
	glEnd();

	for(size_t i = 0; i < objects.size(); i++) {
		glPushMatrix();
			if(objects[i].is_body()) {
				size_t body_num = objects[i].get_body_num();
				Vector3 const & p = sys.bodies[body_num].position;
				glTranslated(p.x, p.y, p.z);
			}
			else {
				Vector3 const & p = objects[i].get_position();
				glTranslated(p.x, p.y, p.z);
			}
			objects[i].render();
		glPopMatrix();
	}
}

} // NEWTON
