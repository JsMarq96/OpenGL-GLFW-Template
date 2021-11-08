#ifndef DYNAMIC_MATRIX_H_
#define DYNAMIC_MATRIX_H_

#include <cstdint>
#include <cmath>
#include <cstring>
#include <cassert>
#include <iostream>

#include "vector.h"
#include "matrix33.h"

/**
 * A dynamic matrix format
 * This is only for using on the physics solver, and MAYBE some other place
 * */

struct sDynMatrix {
    uint8_t   width   = 0;
    uint8_t   height  = 0;

    float    *values  = NULL;

    void init(const uint8_t   i_width,
              const uint8_t   i_height,
              const float     default_value) {

        if (values != NULL) {
            free(values);
        }

        values = (float*) malloc(sizeof(float) * i_width * i_height);
        memset(values, default_value, sizeof(float) * i_width * i_height);

        width = i_width;
        height = i_height;
    }

    void clean() {
        free(values);
    }

    inline float get(const uint8_t   x,
                     const uint8_t   y) const {
        return (x < width && y < height) ? values[x + (y * width)] : 0.0f;
    }

    inline void  set(const uint8_t   x,
                     const uint8_t   y,
                     const float     value) {
        if (x >= width && y >= height) {
            return;
        }

        values[x + (y * width)] = value;
    }

    void multiply(const sDynMatrix   &a,
                  const sDynMatrix   &b) {
        assert(a.width == b.height && "You cannot multiply different sized matrices idk");

        if (values != NULL) {
            free(values);
        }
        init(b.width, a.height, 0.0f);

        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                float tmp = 0.0f;

                for(int i = 0; i < width; i++) {
                    tmp += a.get(y, i) * b.get(x, i);
                }

                set(x, y, tmp);
            }
        }
    }

    void transpose_to(sDynMatrix *a) const {
        a->init(width, height, 0.0f);

        for(int x = 0; x < height; x++) {
            for(int y = 0; y < width; y++) {
                a->set(y,x, get(x, y));
            }
        }
    }

    void copy_matrix_inside(const uint8_t    origin_x,
                            const uint8_t    origin_y,
                            const sMat33     &a) {
        assert(origin_x+3 < width && origin_y+3 < height && "Cannot copy insde matrix since it does not fit");

        for(int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                set(x + origin_x, y + origin_y, a.mat_values[x][y]);
            }
        }
    }

    void copy_vector3_inside(const uint8_t    origin_x,
                             const uint8_t    origin_y,
                             const sVector3   &a) {
        assert(origin_x+3 < width && origin_y < height && "Cannot copy insde vector since it does not fit");

        for (int x = 0; x < 3; x++) {
            set(x + origin_x, origin_y, a.raw_values[x]);
        }
    }

    void print() const {
        for(int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++) {
                std::cout << get(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif // DYNAMIC_MATRIX_H_
