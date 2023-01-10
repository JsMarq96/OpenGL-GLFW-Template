#ifndef MESH_RENDERER_H_
#define MESH_RENDERER_H_

#include <glm/glm.hpp>

#include "gl3w.h"
#include "glcorearb.h"
#include "shader.h"
#include "raw_shaders.h"
#include "mesh.h"
#include "material.h"
#include "camera.h"

struct sMeshRenderer {
    unsigned int  VAO = 0;
    unsigned int  VBO = 0;
    unsigned int  EBO = 0;

    uint16_t indices_count = 0;

    sMaterial material;

    void create_from_mesh(const sMesh *mesh) {
        indices_count = mesh->indexing_count;

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(sRawVertex), mesh->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexing_count * sizeof(uint16_t), mesh->vertices_index, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Load vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);

        // UV coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (sizeof(float) * 3));

        // Vertex normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (sizeof(float) * 5));

        // Load vertex indexing
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void render(const glm::mat4x4 *models,
                const int count,
                const glm::mat4x4 &view_proj,
                const bool show_wireframe,
                const sCamera &camera) const {

        glBindVertexArray(VAO);

        if (show_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        material.enable();

        for(int i = 0; i < count; i++) {
            material.shader.set_uniform_matrix4("u_model_mat", models[i]);
            material.shader.set_uniform_matrix4("u_viewproj_mat", view_proj);
            material.shader.set_uniform_vector("u_camera_position", camera.position);
            glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_SHORT, 0);
        }

        material.disable();
    }

    void clean() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};


#endif // Mmaterial.ESH_RENmaterial.DERER_H_material.
