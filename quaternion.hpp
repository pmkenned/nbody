#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_

#include "math.hpp"
#include "vector.hpp"

namespace NEWTON {

class Matrix3;
class Matrix4;

class Quaternion
{
public:
    static const Quaternion Zero;
    static const Quaternion Identity;

    real_t w, x, y, z;

    Quaternion() {}
    Quaternion( real_t w, real_t x, real_t y, real_t z )
        : w( w ), x( x ), y( y ), z( z ) { }

    // Constructs a quaternion representing a rotation about the given axis by the given angle.
    Quaternion( const Vector3& axis, real_t radians );

    // Constructs a quaternion from a rotation matrix.
    explicit Quaternion( const Matrix4& mat );

    Quaternion operator*( const Quaternion& rhs ) const;

    // Rotate a vector by this quaternion.
    Vector3 operator*( const Vector3& rhs ) const;

    Quaternion operator*( real_t s ) const {
        return Quaternion( w * s, x * s, y * s, z * s );
    }

    Quaternion& operator*=( real_t s ) {
        w *= s;
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    bool operator==( const Quaternion& rhs ) const {
        return rhs.x == x && rhs.y == y &&
               rhs.z == z && rhs.w == w;
    }

    bool operator!=( const Quaternion& rhs ) const {
        return !operator==( rhs );
    }

    // Convert this quaternion into an angle and axis. Returns the rotation in radians about an axis.
    void to_axis_angle( Vector3* axis, real_t* angle ) const;

    // Converts this quaternion to a 3x3 matrix.
    void to_matrix( Matrix3* mat ) const;

    // Converts this quaternion to a 4x4 matrix.
    void to_matrix( Matrix4* mat ) const;

    // Returns the X,Y,Z axes rotated by this quaternion.
    void to_axes( Vector3 axes[3] ) const;
};

inline real_t norm( const Quaternion& q ) {
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

inline Quaternion operator*( real_t s, const Quaternion& rhs ) {
    return rhs * s;
}

Quaternion normalize( const Quaternion& q );

Quaternion conjugate( const Quaternion& q );

std::ostream& operator <<( std::ostream& o, const Quaternion& q );

} /* NEWTON */

#endif /* _QUATERNION_HPP_ */