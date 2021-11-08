#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <cmath>

union sVector3;

union sQuaternion4 {
    float raw_values[4];
    struct {
        float q0 = 1.0f;
        float q1 = 0.0f;
        float q2 = 0.0f;
        float q3 = 0.0f;
    };
    struct {
        float w;
        float x;
        float y;
        float z;
    };

    inline sQuaternion4 conjugate() const {
        return sQuaternion4{w, -x, -y, -z};
    }

    inline sQuaternion4 inverse() const {
        float norm = sqrt(w*w + x*x + y*y + z*z);
        return sQuaternion4{w / norm, -x / norm, -y / norm, -z / norm};
    }

    inline sQuaternion4 normalize() const {
        float norm = sqrt(w*w + x*x + y*y + z*z);
        return sQuaternion4{w / norm, x / norm, y / norm, z / norm};
    }

    inline sQuaternion4 multiply(const sQuaternion4 &q) const {
      return {(w * q.w) - (x * q.x) - (y * q.y) - (z * q.z),
              (w * q.x) + (x * q.w) - (y * q.z) + (z * q.y),
              (w * q.y) + (x * q.z) + (y * q.w) - (z * q.x),
              (w * q.z) - (x * q.y) + (y * q.x) + (z * q.w)
              };
    };

    inline sQuaternion4 multiply(const float num) const {
      return sQuaternion4{w * num, x * num, y * num, z * num};
    }

    inline sQuaternion4 sum(const sQuaternion4 &quat) const {
      return sQuaternion4{w + quat.w, x + quat.x, y + quat.y, z + quat.z};
    }

    inline sVector3 get_vector() const;
};


#endif // QUATERNION_H_
