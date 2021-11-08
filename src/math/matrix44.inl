#include "matrix44.h"
#include "quaternion.h"


inline void
    sMat44::set_identity() {
        sx1 = 1.0f; sy1 = 0.0f; sz1 = 0.0f; tmp1 = 0.0f;
        sx2 = 0.0f; sy2 = 1.0f; sz2 = 0.0f; tmp2 = 0.0f;
        sx3 = 0.0f; sy3 = 0.0f; sz3 = 1.0f; tmp3 = 0.0f;
        px = 0.0f; py = 0.0f; pz = 0.0f; tmp4 = 1.0f;
    }

    ///
    /// SIMPLE OPERATIONS
    ///

    inline void sMat44::set_position(const sVector3 &vec) {
        px = vec.x;
        py = vec.y;
        pz = vec.z;
    }

    inline void sMat44::add_position(const sVector3 &vec) {
        px += vec.x;
        py += vec.y;
        pz += vec.z;
    }

    inline void sMat44::set_scale(const sVector3 &vec) {
        sx1 = vec.x;
        sy2 = vec.y;
        sz3 = vec.z;
    }


    ///
    /// COMPLEX OPERATIONS
    ///

    // TODO: Wrap loops
    // TODO: Optimize via SIMD
    inline void
    sMat44::multiply(const sMat44   *B) {
      sMat44 result = {};
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                float tmp = 0.0f;
                for (int i = 0; i < 4; i++) {
                    tmp += mat_values[i][y] * B->mat_values[x][i];
                }

                result.mat_values[x][y] = tmp;
            }
        }

        memcpy(mat_values, result.mat_values, sizeof(float) * 16);
    }

    inline void sMat44::rotate(const sQuaternion4 &quat) {
      sMat44 tmp_mat = {}, tmp_mat2 = {};
      tmp_mat.convert_quaternion_to_matrix(quat);
        tmp_mat.invert(&tmp_mat2);
        multiply(&tmp_mat2);
    }

    inline void sMat44::scale(const sVector3 &vect) {
      sMat44 tmp_mat = {}, tmp_mat2 = {};
        tmp_mat.set_scale(vect);
        tmp_mat.invert(&tmp_mat2);
        multiply(&tmp_mat2);
    }

    inline sVector4 sMat44::multiply(const sVector4   &vect) const {
        sVector4 result {};
        for (int i = 0; i < 4; i++) {
            result.raw_values[i] = (vect.raw_values[0] * mat_values[i][0]) +
                                   (vect.raw_values[1] * mat_values[i][1]) +
                                   (vect.raw_values[2] * mat_values[i][2]) +
                                   (vect.raw_values[3] * mat_values[i][3]);
        }

        return result;
    }

    inline sVector3 sMat44::multiply(const sVector3   &vect) const{
        float x = sx1 * vect.x + (sx2 * vect.y + (sx3 * vect.z + px));
        float y = sy1 * vect.x + (sy2 * vect.y + (sy3 * vect.z + py));
        float z = sz1 * vect.x + (sz2 * vect.y + (sz3 * vect.z + pz));
        return sVector3{x, y, z};
    }

  inline sQuaternion4 sMat44::multiply(const sQuaternion4 &quat) const {
        sQuaternion4 result {};
        for (int i = 0; i < 4; i++) {
            result.raw_values[i] = (quat.raw_values[0] * mat_values[i][0]) +
                                   (quat.raw_values[1] * mat_values[i][1]) +
                                   (quat.raw_values[2] * mat_values[i][2]) +
                                   (quat.raw_values[3] * mat_values[i][3]);
        }

        return result;

  }

    inline void sMat44::transpose_to(sMat44* result) const {
      result->mat_values[0][0] = mat_values[0][0];
      result->mat_values[0][1] = mat_values[1][0];
      result->mat_values[0][2] = mat_values[2][0];
      result->mat_values[0][3] = mat_values[3][0];
      result->mat_values[1][0] = mat_values[0][1];
      result->mat_values[1][1] = mat_values[1][1];
      result->mat_values[1][2] = mat_values[2][1];
      result->mat_values[1][3] = mat_values[3][1];
      result->mat_values[2][0] = mat_values[0][2];
      result->mat_values[2][1] = mat_values[1][2];
      result->mat_values[2][2] = mat_values[2][2];
      result->mat_values[2][3] = mat_values[3][2];
      result->mat_values[3][0] = mat_values[0][3];
      result->mat_values[3][1] = mat_values[1][3];
      result->mat_values[3][2] = mat_values[2][3];
      result->mat_values[3][3] = mat_values[3][3];
    }

  inline void sMat44::transpose() {
    sMat44 tmp = {};
      tmp.mat_values[0][0] = mat_values[0][0];
      tmp.mat_values[0][1] = mat_values[1][0];
      tmp.mat_values[0][2] = mat_values[2][0];
      tmp.mat_values[0][3] = mat_values[3][0];
      tmp.mat_values[1][0] = mat_values[0][1];
      tmp.mat_values[1][1] = mat_values[1][1];
      tmp.mat_values[1][2] = mat_values[2][1];
      tmp.mat_values[1][3] = mat_values[3][1];
      tmp.mat_values[2][0] = mat_values[0][2];
      tmp.mat_values[2][1] = mat_values[1][2];
      tmp.mat_values[2][2] = mat_values[2][2];
      tmp.mat_values[2][3] = mat_values[3][2];
      tmp.mat_values[3][0] = mat_values[0][3];
      tmp.mat_values[3][1] = mat_values[1][3];
      tmp.mat_values[3][2] = mat_values[2][3];
      tmp.mat_values[3][3] = mat_values[3][3];
      memcpy(&mat_values, &tmp, sizeof(mat_values));
    }

inline void
sMat44::convert_quaternion_to_matrix(const sQuaternion4 &quat) {
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
  mat_values[3][3] = 1.0;
}


    // Yoinked from a stackoverlof that yoinked from the MESA implmentation
    // of GLU
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    // It uses Sarrus' rule
    // TODO: SIMD...?
    inline void sMat44::invert(sMat44 *result) const {
        float inv[16], det;
        int i;

        inv[0] = raw_values[5]  * raw_values[10] * raw_values[15] -
                 raw_values[5]  * raw_values[11] * raw_values[14] -
                 raw_values[9]  * raw_values[6]  * raw_values[15] +
                 raw_values[9]  * raw_values[7]  * raw_values[14] +
                 raw_values[13] * raw_values[6]  * raw_values[11] -
                 raw_values[13] * raw_values[7]  * raw_values[10];

        inv[4] = -raw_values[4]  * raw_values[10] * raw_values[15] +
                 raw_values[4]  * raw_values[11] * raw_values[14] +
                 raw_values[8]  * raw_values[6]  * raw_values[15] -
                 raw_values[8]  * raw_values[7]  * raw_values[14] -
                 raw_values[12] * raw_values[6]  * raw_values[11] +
                 raw_values[12] * raw_values[7]  * raw_values[10];

        inv[8] = raw_values[4]  * raw_values[9] * raw_values[15] -
                 raw_values[4]  * raw_values[11] * raw_values[13] -
                 raw_values[8]  * raw_values[5] * raw_values[15] +
                 raw_values[8]  * raw_values[7] * raw_values[13] +
                 raw_values[12] * raw_values[5] * raw_values[11] -
                 raw_values[12] * raw_values[7] * raw_values[9];

        inv[12] = -raw_values[4]  * raw_values[9] * raw_values[14] +
                  raw_values[4]  * raw_values[10] * raw_values[13] +
                  raw_values[8]  * raw_values[5] * raw_values[14] -
                  raw_values[8]  * raw_values[6] * raw_values[13] -
                  raw_values[12] * raw_values[5] * raw_values[10] +
                  raw_values[12] * raw_values[6] * raw_values[9];

        inv[1] = -raw_values[1]  * raw_values[10] * raw_values[15] +
                 raw_values[1]  * raw_values[11] * raw_values[14] +
                 raw_values[9]  * raw_values[2] * raw_values[15] -
                 raw_values[9]  * raw_values[3] * raw_values[14] -
                 raw_values[13] * raw_values[2] * raw_values[11] +
                 raw_values[13] * raw_values[3] * raw_values[10];

        inv[5] = raw_values[0]  * raw_values[10] * raw_values[15] -
                 raw_values[0]  * raw_values[11] * raw_values[14] -
                 raw_values[8]  * raw_values[2] * raw_values[15] +
                 raw_values[8]  * raw_values[3] * raw_values[14] +
                 raw_values[12] * raw_values[2] * raw_values[11] -
                 raw_values[12] * raw_values[3] * raw_values[10];

        inv[9] = -raw_values[0]  * raw_values[9] * raw_values[15] +
                 raw_values[0]  * raw_values[11] * raw_values[13] +
                 raw_values[8]  * raw_values[1] * raw_values[15] -
                 raw_values[8]  * raw_values[3] * raw_values[13] -
                 raw_values[12] * raw_values[1] * raw_values[11] +
                 raw_values[12] * raw_values[3] * raw_values[9];

        inv[13] = raw_values[0]  * raw_values[9] * raw_values[14] -
                  raw_values[0]  * raw_values[10] * raw_values[13] -
                  raw_values[8]  * raw_values[1] * raw_values[14] +
                  raw_values[8]  * raw_values[2] * raw_values[13] +
                  raw_values[12] * raw_values[1] * raw_values[10] -
                  raw_values[12] * raw_values[2] * raw_values[9];

        inv[2] = raw_values[1]  * raw_values[6] * raw_values[15] -
                 raw_values[1]  * raw_values[7] * raw_values[14] -
                 raw_values[5]  * raw_values[2] * raw_values[15] +
                 raw_values[5]  * raw_values[3] * raw_values[14] +
                 raw_values[13] * raw_values[2] * raw_values[7] -
                 raw_values[13] * raw_values[3] * raw_values[6];

        inv[6] = -raw_values[0]  * raw_values[6] * raw_values[15] +
                 raw_values[0]  * raw_values[7] * raw_values[14] +
                 raw_values[4]  * raw_values[2] * raw_values[15] -
                 raw_values[4]  * raw_values[3] * raw_values[14] -
                 raw_values[12] * raw_values[2] * raw_values[7] +
                 raw_values[12] * raw_values[3] * raw_values[6];

        inv[10] = raw_values[0]  * raw_values[5] * raw_values[15] -
                  raw_values[0]  * raw_values[7] * raw_values[13] -
                  raw_values[4]  * raw_values[1] * raw_values[15] +
                  raw_values[4]  * raw_values[3] * raw_values[13] +
                  raw_values[12] * raw_values[1] * raw_values[7] -
                  raw_values[12] * raw_values[3] * raw_values[5];

        inv[14] = -raw_values[0]  * raw_values[5] * raw_values[14] +
                  raw_values[0]  * raw_values[6] * raw_values[13] +
                  raw_values[4]  * raw_values[1] * raw_values[14] -
                  raw_values[4]  * raw_values[2] * raw_values[13] -
                  raw_values[12] * raw_values[1] * raw_values[6] +
                  raw_values[12] * raw_values[2] * raw_values[5];

        inv[3] = -raw_values[1] * raw_values[6] * raw_values[11] +
                 raw_values[1] * raw_values[7] * raw_values[10] +
                 raw_values[5] * raw_values[2] * raw_values[11] -
                 raw_values[5] * raw_values[3] * raw_values[10] -
                 raw_values[9] * raw_values[2] * raw_values[7] +
                 raw_values[9] * raw_values[3] * raw_values[6];

        inv[7] = raw_values[0] * raw_values[6] * raw_values[11] -
                 raw_values[0] * raw_values[7] * raw_values[10] -
                 raw_values[4] * raw_values[2] * raw_values[11] +
                 raw_values[4] * raw_values[3] * raw_values[10] +
                 raw_values[8] * raw_values[2] * raw_values[7] -
                 raw_values[8] * raw_values[3] * raw_values[6];

        inv[11] = -raw_values[0] * raw_values[5] * raw_values[11] +
                  raw_values[0] * raw_values[7] * raw_values[9] +
                  raw_values[4] * raw_values[1] * raw_values[11] -
                  raw_values[4] * raw_values[3] * raw_values[9] -
                  raw_values[8] * raw_values[1] * raw_values[7] +
                  raw_values[8] * raw_values[3] * raw_values[5];

        inv[15] = raw_values[0] * raw_values[5] * raw_values[10] -
                  raw_values[0] * raw_values[6] * raw_values[9] -
                  raw_values[4] * raw_values[1] * raw_values[10] +
                  raw_values[4] * raw_values[2] * raw_values[9] +
                  raw_values[8] * raw_values[1] * raw_values[6] -
                  raw_values[8] * raw_values[2] * raw_values[5];

        det = raw_values[0] * inv[0] + raw_values[1] * inv[4] + raw_values[2] * inv[8] + raw_values[3] * inv[12];

        assert(det != 0.0f && "Cannot inverse a matrix with 0 determinant");

        det = 1.0f / det;

        for (i = 0; i < 16; i++) {
            result->raw_values[i] = inv[i] * det;
        }

    }


#include <iostream>
inline void sMat44::print() const {
  std::cout << "  ======== " << std::endl;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      std::cout << mat_values[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "  ======== " << std::endl;
}
