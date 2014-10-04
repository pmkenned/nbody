#ifndef _MATH_HPP_
#define _MATH_HPP_

#include <algorithm>
#include <cmath>

namespace NEWTON {

typedef double real_t; // floating point precision set by this typedef

//class Color3;

#define PI 3.141592653589793238

template<typename T>
inline T clamp( T val, T min, T max )
{
    return std::min( max, std::max( min, val ) );
}

} /* NEWTON */

#endif /* _MATH_HPP_ */