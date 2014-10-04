#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_

#include <vector>
#include "math.hpp"

namespace NEWTON {

class IntegrableSystem {
public:
    virtual ~IntegrableSystem() { }
    virtual size_t size() const = 0;
    virtual void get_state( real_t* arr, real_t* time ) const = 0;
    virtual void set_state( const real_t* arr, const real_t time ) = 0;
    virtual void eval_deriv( real_t* deriv_result ) = 0;
};

class Integrator {
public:
    virtual ~Integrator() { }
    virtual void integrate( IntegrableSystem& sys, real_t dt ) const = 0;
    typedef std::vector< real_t > StateList;
};

class RungeKuttaIntegrator : public Integrator {
public:
    virtual ~RungeKuttaIntegrator() { }
    virtual void integrate( IntegrableSystem& sys, real_t dt ) const;
private:
	mutable StateList state;
    mutable StateList state2;
};

} // NEWTON

#endif