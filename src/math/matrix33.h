#ifndef MATRIX33_H_
#define MATRIX33_H_

#include <string.h>
#include <assert.h>
#include <cmath>
#include <iostream>

#include "vector.h"
#include "quaternion.h"

union sMat33 {
    float raw_values[9];
    float mat_values[3][3];
    struct {
        float sx1 = 1.0f; float sy1 = 0.0f; float tmp1 = 0.0f;
        float sx2 = 0.0f; float sy2 = 1.0f; float tmp2 = 0.0f;
        float px = 0.0f; float py = 0.0f; float tmp3 = 1.0;
    };

    void set_identity() {
        sx1 = 1.0f;  sy1 = 0.0f;  px = 0.0f;
        sx2 = 0.0f;  sy2 = 1.0f;  py = 0.0f;
        tmp1 = 0.0f; tmp2 = 0.0f; tmp3 = 1.0;
    }

    inline sVector2 multiply(const sVector2   *vect) {
        return sVector2{ (vect->x * sx1) + (vect->y * sy1) + px, (vect->x * sx2) + (vect->y * sy2) + py };
    }

    inline sVector3 multiply(const sVector3   &vect) const {
        sVector3 result = {};
        for (int i = 0; i < 3; i++) {
            result.raw_values[i] = (vect.raw_values[0] * mat_values[i][0]) +
                                   (vect.raw_values[1] * mat_values[i][1]) +
                                   (vect.raw_values[2] * mat_values[i][2]);
        }

        return result;
    }


    inline void multiply(const sMat33   *B) {
        sMat33 result = {};
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                float tmp = 0.0f;
                for (int i = 0; i < 3; i++) {
                    tmp += mat_values[x][i] * B->mat_values[i][y];
                }

                result.mat_values[x][y] = tmp;
            }
        }

        memcpy(mat_values, result.mat_values, sizeof(float) * 9);
    }

    inline void multiply_to(const sMat33   *B,
                      sMat33   *result) {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                float tmp = 0.0f;
                for (int i = 0; i < 3; i++) {
                    tmp += mat_values[x][i] * B->mat_values[i][y];
                }

                result->mat_values[x][y] = tmp;
            }
        }
    }

    inline void transponse_to(sMat33 *b) const {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                b->mat_values[y][x] = mat_values[x][y];
            }
        }
    }

    inline void set_position(const sVector2    vec) {
        px = vec.x;
        py = vec.y;
    }

    inline void set_scale(const sVector2   vec) {
        sx1 = vec.x;
        sy2 = vec.y;
    }

    inline void invert(sMat33 *inverse) const {
        // Using sarrus rule
        float determinant = (mat_values[0][0] * (mat_values[1][1] * mat_values[2][2] - mat_values[2][1] * mat_values[1][2])
                            - mat_values[1][0] * (mat_values[0][1] * mat_values[2][2] - mat_values[2][1] * mat_values[0][2])
                            + mat_values[2][0] * (mat_values[0][1] * mat_values[1][2] - mat_values[1][1] * mat_values[0][2]));
        assert(determinant != 0 && "Determinant cannto be 0 on inv_matrix 3");

        determinant = 1.0f / determinant;

        inverse->mat_values[0][0] = + (mat_values[1][1] * mat_values[2][2] - mat_values[2][1] * mat_values[1][2]) * determinant;
        inverse->mat_values[1][0] = - (mat_values[1][0] * mat_values[2][2] - mat_values[2][0] * mat_values[1][2]) * determinant;
        inverse->mat_values[2][0] = + (mat_values[1][0] * mat_values[2][1] - mat_values[2][0] * mat_values[1][1]) * determinant;
        inverse->mat_values[0][1] = - (mat_values[0][1] * mat_values[2][2] - mat_values[2][1] * mat_values[0][2]) * determinant;
        inverse->mat_values[1][1] = + (mat_values[0][0] * mat_values[2][2] - mat_values[2][0] * mat_values[0][2]) * determinant;
        inverse->mat_values[2][1] = - (mat_values[0][0] * mat_values[2][1] - mat_values[2][0] * mat_values[0][1]) * determinant;
        inverse->mat_values[0][2] = + (mat_values[0][1] * mat_values[1][2] - mat_values[1][1] * mat_values[0][2]) * determinant;
        inverse->mat_values[1][2] = - (mat_values[0][0] * mat_values[1][2] - mat_values[1][0] * mat_values[0][2]) * determinant;
        inverse->mat_values[2][2] = + (mat_values[0][0] * mat_values[1][1] - mat_values[1][0] * mat_values[0][1]) * determinant;
      }

    inline void convert_quaternion_to_matrix(const sQuaternion4 &quat) {
        float qx2 = quat.x + quat.x;
        float qy2 = quat.y + quat.y;
        float qz2 = quat.z + quat.z;
        float qxqx2 = quat.x * qx2;
        float qxqy2 = quat.x * qy2;
        float qxqz2 = quat.x * qz2;
        float qxqw2 = quat.w * qx2;

        float qyqy2 = quat.y * qy2;
        float qyqz2 = quat.y * qz2;
        float qyqw2 = quat.w * qy2;

        float qzqz2 = quat.z * qz2;
        float qzqw2 = quat.w * qz2;

        set_identity();
        mat_values[0][0] = 1.0f - qyqy2 - qzqz2;
        mat_values[0][1] = qxqy2 + qzqw2;
        mat_values[0][2] = qxqz2 - qyqw2;

        mat_values[1][0] = qxqy2 - qzqw2;
        mat_values[1][1] = 1.0f - qxqx2 - qzqz2;
        mat_values[1][2] = qyqz2 + qxqw2;

        mat_values[2][0] = qxqz2 + qyqw2;
        mat_values[2][1] = qyqz2 - qxqw2;
        mat_values[2][2] = 1.0f - qxqx2 - qyqy2;
    }

    inline void print() const {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                std::cout << mat_values[x][y] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "----------" << std::endl;
    }

};

#endif // MATRIX33_H_
