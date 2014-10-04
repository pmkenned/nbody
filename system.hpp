#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "SDL.h"
#include "SDL_opengl.h"

#include "vector.hpp"
#include "integrator.hpp"

namespace NEWTON {

extern const GLdouble G;

#define PARTICLE_SIZE 6

#define NP 10
#define SUN       0
#define MERCURY   1
#define VENUS     2
#define EARTH     3
#define MOON      4
#define MARS      5
#define JUPITER   6
#define SATURN    7
#define URANUS    8
#define NEPTUNE   9

struct Body
{ 
    // particle members, part of integrator state
    Vector3 position;
    Vector3 velocity;

    // other info, not part of integrator state
    Vector3 acc_accumulator;
    real_t mass;
	bool exerts_grav;
//	bool subject_to_grav;
	Vector3 thrust;
};

class System : public IntegrableSystem {
public:
    virtual ~System() { }
    bool initialize();
	void translate(Vector3 const & t);
	size_t add_body(real_t mass, Vector3 const & pos, Vector3 const & vel, bool exerts_grav = true);
    void set_body( size_t index, const Vector3& position, const Vector3& velocity, real_t mass );

    // integrable system interface
    virtual size_t size() const;
    virtual void get_state( real_t* arr, real_t* time ) const;
    virtual void set_state( const real_t* arr, const real_t time );
    virtual void eval_deriv( real_t* deriv_result );

//private:
    std::vector< Body > bodies;
    real_t time;
};

} // NEWTON

#endif