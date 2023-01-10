#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstring>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Camera functions
 * TODO: Implement zoom as a view matrix
 * */

//
struct sCurrentCameraInstance {
    glm::mat4x4     ortho_matrix;
    glm::vec3   position  = { 0.0f, 0.0f, 0.0f};
};

struct sCamera {
    glm::vec3     position {0.0f, 0.0f, 0.0f};
    float        zoom      = 0.059f;

    // TODO: initialize this and the view matrix code
    glm::vec3    u = {};
    glm::vec3    s = {};
    glm::vec3    f = {};

    glm::mat4x4 view_mat = {};

    // View Port size
    int        vp_height  = 0.0f;
    int        vp_width   = 0.0f;


    // Orthografic projection is the only one by default
    void
    get_ortho_projection_matrix(glm::mat4x4     *result) const {
        glm::mat4 ortho = glm::mat4(1.0f);

        float left = (position.y -vp_width) * zoom / 2.0f;
        float right = (position.y + vp_width) * zoom / 2.0f;
        float bottom = (position.x -vp_height) * zoom / 2.0f;
        float top = (position.x + vp_height) * zoom / 2.0f;

        ortho[0][0] = 2.0f / (right - left);
        ortho[1][1] = 2.0f / (top - bottom);
        ortho[2][2] = -1.0f;
        ortho[3][0] = -(right + left) / (right - left);
        ortho[3][1] = -(top + bottom) / (top - bottom);
    };

    void
    get_perspective_projection_matrix(const float FOV,
                                      const float far_plane,
                                      const float near_plane,
                                      const float aspect_ratio,
                                      glm::mat4x4 *result) const {
      *result = glm::perspective(FOV, aspect_ratio, far_plane, near_plane);
    }

    void
    look_at(const glm::vec3 &center) {
      f = glm::normalize(glm::vec3{center.x - position.x, center.y - position.y, center.z - position.z});
      s = glm::cross(f, glm::normalize(glm::vec3{0.f, 1.0f, 0.0f}));
      u = glm::cross(s, f);

      view_mat = glm::lookAt(f,
                             s,
                             u);
    }

  void compute_view_matrix() {
     s = glm::cross(f, glm::normalize(glm::vec3{0.f, 1.0f, 0.0f}));
      u = glm::cross(s, f);

      view_mat = glm::lookAt(f,
                             s,
                             u);
  }

    void
    get_perspective_viewprojection_matrix(const float FOV, 
                                          const float far_plane, 
                                          const float near_plane,
                                          const float aspect_ratio,
                                          glm::mat4x4  *result) const {
      get_perspective_projection_matrix(FOV, far_plane, near_plane, aspect_ratio, result);

      *result = view_mat * *result;
      //memcpy(result, &tmp, sizeof(sMat44));
    }

    void
    get_current_camera_instance(sCurrentCameraInstance  *new_instance) const {
        new_instance->position.x = position.x * zoom / 2.0f;
        new_instance->position.y = position.y * zoom / 2.0f;
        new_instance->position.z = position.z * zoom / 2.0f;

        //get_projection_matrix(&(new_instance->ortho_matrix));
    }

  void
  set_rotation(const float pitch,
               const float yaw) {
    /*sVector3 dir = {};
    dir.x = cos(radians(yaw)) * cos(radians(pitch));
    dir.y = radians(pitch);
    dir.z = sin(radians(yaw)) * cos(radians(pitch));

    f = dir.normalize(); */
  }
};

#endif // CAMERA_H_
