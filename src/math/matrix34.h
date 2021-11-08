#ifndef MATRIX34_H_
#define MATRIX34_H_

#include <string.h>
#include <assert.h>
#include <cmath>


//TODO: fill values by rows & multiplication

union sMat1212 {
    float raw_values[144];
    float mat_values[12][12];

    void set_indentity() {
        memset(raw_values, 0, sizeof(raw_values));
        for(int i = 0; i < 12; i++) {
            mat_values[i][i] = 1.0f;
        }
    }
};

#endif // MATRIX34_H_
