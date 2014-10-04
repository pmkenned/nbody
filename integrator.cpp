#include "integrator.hpp"

namespace NEWTON {

void RungeKuttaIntegrator::integrate(IntegrableSystem& sys, real_t dt) const {
	real_t time;
	size_t size = sys.size();

	if(size == 0)
		return;
	state.resize(size);
	state2.resize(size);

    // get the current state (pos, t)
	sys.get_state(&state[0], &time);

	real_t * k1 = new real_t[size];
	real_t * k2 = new real_t[size];
	real_t * k3 = new real_t[size];
	real_t * k4 = new real_t[size];

	sys.eval_deriv(k1);

	for(size_t i=0; i<size; i++)
		state2[i] = state[i] + 0.5*dt*k1[i];
	sys.set_state(&state2[0], time + 0.5*dt);
	sys.eval_deriv(k2);

	for(size_t i=0; i<size; i++)
		state2[i] = state[i] + 0.5*dt*k2[i];
	sys.set_state(&state2[0], time + 0.5*dt);
	sys.eval_deriv(k3);

	for(size_t i=0; i<size; i++)
		state2[i] = state[i] + dt*k3[i];
	sys.set_state(&state2[0], time + dt);
	sys.eval_deriv(k4);

	for(size_t i=0; i<size; i++)
		state[i] += (1.0/6)*dt*(k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);

	sys.set_state(&state[0], time + dt);
}

} // NEWTON