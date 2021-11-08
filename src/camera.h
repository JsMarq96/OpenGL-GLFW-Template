#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstring>
#include <cmath>
#include "math.h"

/**
 * Camera functions
 * TODO: Implement zoom as a view matrix
 * */

//
struct sCurrentCameraInstance {
    sMat44     ortho_matrix;
    sVector3   position  = { 0.0f, 0.0f, 0.0f};
};

struct sCamera {
    sVector3     position {0.0f, 0.0f, 0.0f};
    float        zoom      = 0.059f;

    // TODO: initialize this and the view matrix code
    sVector3    u;
    sVector3    s;
    sVector3    f;

    sMat44 view_mat;

    // View Port size
    int        vp_height  = 0.0f;
    int        vp_width   = 0.0f;

    static sCamera*
    shared_instance() {
        static sCamera instance;
        return &instance;
    };

    // Orthografic projection is the only one by default
    void
    get_ortho_projection_matrix(sMat44     *result) const {
        result->set_identity();

        float left = (position.y -vp_width) * zoom / 2.0f;
        float right = (position.y + vp_width) * zoom / 2.0f;
        float bottom = (position.x -vp_height) * zoom / 2.0f;
        float top = (position.x + vp_height) * zoom / 2.0f;

        result->mat_values[0][0] = 2.0f / (right - left);
        result->mat_values[1][1] = 2.0f / (top - bottom);
        result->mat_values[2][2] = -1.0f;
        result->mat_values[3][0] = -(right + left) / (right - left);
        result->mat_values[3][1] = -(top + bottom) / (top - bottom);
    };

    void
    get_perspective_projection_matrix(const float FOV,
                                      const float far,
                                      const float near,
                                      const float aspect_ratio,
                                      sMat44  *result) const {
        /*float scale = 1.0f / tan((FOV / 2.0f) * (M_PI / 180.f));
        
        result->set_identity();
        result->sx1 = scale;
        result->sy2 = scale;
        result->sz3 = -far / (far-near);
        result->tmp4 = 0.0f;
        result->pz = -1.0f;
        result->tmp3 = -(far * near)/(far-near);*/
      float tan_half_FOV = tan(to_radians(FOV) / 2.0f);
      result->set_identity();
      result->mat_values[0][0] = 1.0f / (aspect_ratio * tan_half_FOV);
      result->mat_values[1][1] = 1.0f / tan_half_FOV;
      result->mat_values[2][2] = -(far + near) / (far - near);
      result->mat_values[2][3] = -1.0f;
      result->mat_values[3][2] = -(2.0f * far * near) / (far - near);
      result->mat_values[3][3] = 0.0f; 
    }

    void
    look_at(const sVector3 center) {
      f = sVector3{center.x - position.x, center.y - position.y, center.z - position.z}.normalize();
      s = cross_prod(f, sVector3{0.f, 1.0f, 0.0f}).normalize();
      u = cross_prod(s, f);

      view_mat.set_identity();
      view_mat.mat_values[0][0] = s.x;
      view_mat.mat_values[1][0] = s.y;
      view_mat.mat_values[2][0] = s.z;
      view_mat.mat_values[0][1] = u.x;
      view_mat.mat_values[1][1] = u.y;
      view_mat.mat_values[2][1] = u.z;
      view_mat.mat_values[0][2] = -f.x;
      view_mat.mat_values[1][2] = -f.y;
      view_mat.mat_values[2][2] = -f.z;
      view_mat.mat_values[3][0] = -dot_prod(s, position);
      view_mat.mat_values[3][1] = -dot_prod(u, position);
      view_mat.mat_values[3][2] = dot_prod(f, position); 
    }

    void
    get_perspective_viewprojection_matrix(const float FOV, 
                                          const float far, 
                                          const float near,
                                          const float aspect_ratio,
                                          sMat44  *result) const {
      sMat44 tmp = {};
      memcpy(&tmp, &view_mat, sizeof(sMat44));
      get_perspective_projection_matrix(FOV, far, near, aspect_ratio, result);

      result->multiply(&tmp);

      //memcpy(result, &tmp, sizeof(sMat44));
    }

    void
    get_current_camera_instance(sCurrentCameraInstance  *new_instance) const {
        new_instance->position.x = position.x * zoom / 2.0f;
        new_instance->position.y = position.y * zoom / 2.0f;
        new_instance->position.z = position.z * zoom / 2.0f;

        //get_projection_matrix(&(new_instance->ortho_matrix));
    }
};

#endif // CAMERA_H_
