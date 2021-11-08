#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>

union sQuaternion4;

union sVector2 {
    struct { float x = 0.0f; float y = 0.0f;};
    float raw_values[2];
};


union sVector3 {
    struct { float x = 0.0f; float y = 0.0f; float z = 0.0f; };
    float raw_values[3];

    inline void multiply(const sVector3 &vect) {
      x *= vect.x;
      y *= vect.y;
      z *= vect.z;
    }

    inline sVector3 sum(const sVector3 &vect) const {
      return {x + vect.x, y + vect.y, z + vect.z};
    }

    inline sVector3 subs(const sVector3 &vect) const {
      return {x - vect.x, y - vect.y, z - vect.z};
    }

    inline sVector3 mult(const float num) const {
      return {x * num, y * num, z * num};
    }

    inline sVector3 mult(const sVector3 &vect) const {
        return {x * vect.x, y * vect.y, z * vect.z};
    }

    inline sVector3 rotate(const sQuaternion4 &quat) const;

    inline sVector3 invert() const {
      return sVector3{-x, -y, -z};
    };

  // TODO: std??
  inline sVector3 abs() const {
    return {std::abs(x), std::abs(y), std::abs(z)};
  }

    inline sVector3 normalize() const {
      float magnitude = sqrt((x*x)+ (y*y) + (z*z));
      return sVector3{x / magnitude, y / magnitude, z / magnitude};
    }

    inline float magnitude() const {
      return sqrt( (x*x) + (y*y) + (z*z) );
    }

    inline sQuaternion4 get_pure_quaternion() const;

};

union sVector4 {
    struct { float x = 0.0f; float y = 0.0f; float z = 0.0f; float w = 0.0f; };
    float raw_values[4];
};


#endif // VECTOR_H_
