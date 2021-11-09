//
// Created by jsmar on 21/05/2021.
//

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H
#include <GL/gl3w.h>

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "math.h"
#include "raw_shaders.h"

struct sMeshRenderer {
    unsigned int    VAO;
    unsigned int    VBO;
    unsigned int    EBO;

    const sMesh     *origin_mesh;
    int             indices_count = 0;

    sShader         shader;

    void render_init(const sMesh    *raw_mesh,
                 const bool      is_static);

    void render_init_cube(const bool is_static);

    void render_mesh(const sMat44        &model_mat,
                     const sMat44        &viewproj_mat,
                     const sCamera       &camera,
                     const bool          show_wireframe) const;

    void render_destroy();
};


struct sInstancedMeshRenderer {
    unsigned int    VAO;
    unsigned int    mesh_VBO;
    unsigned int    mesh_EBO;

    unsigned int    instanced_VBO;

    const sMesh     *origin_mesh;
    int             indices_count = 0;

    void init(const sMesh *raw_mesh, const bool is_static);
    void destroy();

    void render_meshes(const sMat44    *model_mats,
                       const int        model_count,
                       const sCamera   &camera,
                       const bool       show_wireframe);
};

#endif //MESH_RENDERER_H
