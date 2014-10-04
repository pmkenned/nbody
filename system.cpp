#include "system.hpp"

namespace NEWTON {

const GLdouble G = 6.67384e-11;

bool System::initialize()
{
	return true;
}

void System::translate(Vector3 const & t) {
	size_t num_part = bodies.size();
	for ( size_t i = 0; i < num_part; ++i )
		bodies[i].position += t;
}

size_t System::add_body(real_t mass, Vector3 const & pos, Vector3 const & vel, bool exerts_grav /* = true */) {
	Body new_body = {pos, vel, Vector3::Zero, mass, exerts_grav, Vector3(0.0, 0.0, 0.0)};
	bodies.push_back(new_body);
	return bodies.size()-1;
}

void System::set_body( size_t index, const Vector3& position, const Vector3& velocity, real_t mass )
{
    assert( index < bodies.size() );
    Body& b = bodies[index];
    b.position = position;
    b.velocity = velocity;
    b.mass = mass;
}

size_t System::size() const
{
    return PARTICLE_SIZE * bodies.size();
}

void System::get_state( real_t* arr, real_t* time ) const
{
    assert( arr && time );
    size_t num_part = bodies.size();

    for ( size_t i = 0; i < num_part; ++i ) {
        size_t idx = PARTICLE_SIZE * i;
        *(Vector3*)(arr+idx+0) = bodies[i].position;
        *(Vector3*)(arr+idx+3) = bodies[i].velocity;
    }

    *time = this->time;
}

void System::set_state( const real_t* arr, const real_t time )
{
    assert( arr );
    size_t num_part = bodies.size();

    for ( size_t i = 0; i < num_part; ++i ) {
        size_t idx = PARTICLE_SIZE * i;
        bodies[i].position = *(Vector3*)(arr+idx+0);
        bodies[i].velocity = *(Vector3*)(arr+idx+3);
    }

    this->time = time;
}

void System::eval_deriv( real_t* deriv_result )
{
    assert( deriv_result );
    size_t num_bodies = bodies.size();

    // zero out acceleration
    for( size_t i = 0; i < num_bodies; i++ ) {
        bodies[i].acc_accumulator = Vector3::Zero;
    }

	// calculate acceleration due to gravity
	for(size_t i = 0; i < num_bodies; i++) {
        double m_i = bodies[i].mass;
        for(size_t j=0; j < num_bodies; j++) {
            if(i == j)
                continue;
			if(!bodies[j].exerts_grav)
				continue;
			Vector3 r_vec = bodies[j].position - bodies[i].position;
            double m_j = bodies[j].mass;
            double r_s = squared_length(r_vec);
			Vector3 r_hat = normalize(r_vec);
            bodies[i].acc_accumulator += r_hat * G*m_j/r_s;
        }
	}

	// calculate acceleration due to thrust
	for(size_t i = 0; i < num_bodies; i++) {
		Vector3 & t = bodies[i].thrust;
		double m = bodies[i].mass;
		bodies[i].acc_accumulator += t/m;
	}

    // compute derivative
    for ( size_t i = 0; i < num_bodies; ++i ) {
        size_t idx = PARTICLE_SIZE * i;
        const Body& p = bodies[i];
        *(Vector3*)(deriv_result+idx+0) = p.velocity;
        *(Vector3*)(deriv_result+idx+3) = p.acc_accumulator;
    }
}

} // NEWTON