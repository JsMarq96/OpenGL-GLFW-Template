//
// Created by jsmar on 21/05/2021.
//

#include "mesh_renderer.h"

// =============================
//  BASIC MESH RENDERING
// =============================
void sMeshRenderer::render_init(const sMesh    *raw_mesh,
                                const bool      is_static) {
    origin_mesh = raw_mesh;
    indices_count = origin_mesh->indices_cout;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 raw_mesh->raw_vertex_size * sizeof(float),
                 raw_mesh->raw_vertex_list,
                 (is_static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 raw_mesh->indices_cout * sizeof(unsigned int),
                 raw_mesh->faces_index,
                 (is_static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    // Position parameter
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void*) 0);

    // UV coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void*) (sizeof(float) * 3));

    glBindVertexArray(0);
}

void sMeshRenderer::render_init_cube(const bool is_static) {
    float tmp_vertex[] = { 0.0f,  1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                           1.0f,  1.0f, 0.0f,
                           0.0f,  1.0f, 0.0f,

                           0.0f, 0.0f,  1.0f,
                           0.0f, 0.0f, 0.0f,
                           0.0f,  1.0f, 0.0f,
                           0.0f,  1.0f, 0.0f,
                           0.0f,  1.0f,  1.0f,
                           0.0f, 0.0f,  1.0f,

                           1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,

                           0.0f, 0.0f,  1.0f,
                           0.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f, 0.0f,  1.0f,
                           0.0f, 0.0f,  1.0f,

                           0.0f,  1.0f, 0.0f,
                           1.0f,  1.0f, 0.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           0.0f,  1.0f,  1.0f,
                           0.0f,  1.0f, 0.0f,

                           0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f,  1.0f,
                           1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f,  1.0f,
                           1.0f, 0.0f,  1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(tmp_vertex),
                 tmp_vertex,
                 (is_static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);


    // Position parameter
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*) 0);


    glBindVertexArray(0);

    indices_count = 36;

    shader.load_shaders(borring_vertex_shader, borring_frag_shader);
}



void sMeshRenderer::render_mesh(const sMat44        &model_mat,
                                const sMat44        &viewproj_mat,
                                const sCamera       &camera,
                                const bool          show_wireframe) const {

    glBindVertexArray(VAO);

    shader.activate();

    shader.set_uniform_matrix4("u_model_mat", model_mat);
    shader.set_uniform_matrix4("u_viewproj_mat", viewproj_mat);

    // This does not really work...
    glDrawArrays((show_wireframe) ? GL_LINES : GL_TRIANGLES, 0, indices_count);

    std::cout << "Rendered mesh " << indices_count << " indexes" << std::endl;

    shader.deactivate();

    glBindVertexArray(0);
}

void sMeshRenderer::render_destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // TODO: Delete shader
}


// ================================
// INSTANCED MESH RENDERER
// ===============================
