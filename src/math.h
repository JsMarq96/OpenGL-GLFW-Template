#ifndef __MATH_H_
#define __MATH_H_

/**
 * Math library with utlities and structures
 * by Juan S. Marquerie
 * */

struct sVector2 {
    union {
        struct { float x; float y;};
        float raw_values[2];
    };
};

struct sRect2 {
    sVector2    point;
    float       width;
    float       height;
};

struct sMat33 {
    union {
        float raw_values[9];
        struct {
            float sx1 = 1.0f; float sy1 = 0.0f; float px = 0.0f;
            float sx2 = 0.0f; float sy2 = 1.0f; float py = 0.0f;
            float tmp1 = 0.0f; float tmp2 = 0.0f; float tmp3 = 1.0;
        };
    };

    sMat33() {
        sx1 = 1.0f;  sy1 = 0.0f;  px = 0.0f;
        sx2 = 0.0f;  sy2 = 1.0f;  py = 0.0f;
        tmp1 = 0.0f; tmp2 = 0.0f; tmp3 = 1.0;
    }

    inline sVector2 multiply(const sVector2   *vect) {
        return sVector2{ (vect->x * sx1) + (vect->y * sy1) + px, (vect->x * sx2) + (vect->y * sy2) + py };
    }

    inline void set_position(const sVector2    vec) {
        px = vec.x;
        py = vec.y;
    }
};

#endif // __MATH_H_
