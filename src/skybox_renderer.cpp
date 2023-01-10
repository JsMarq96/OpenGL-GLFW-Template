//
// Created by Juan S. Marquerie on 01/06/2021.
//

#include "skybox_renderer.h"
#include "glm/ext/matrix_transform.hpp"
#include <GL/gl3w.h>


void sSkyBoxRenderer::init(const char       *texture_dir) {
    float raw_skybox_vertex[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(raw_skybox_vertex),
                 raw_skybox_vertex,
                 GL_STATIC_DRAW);

    // Position parameter
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*) 0);

    glBindVertexArray(0);

#ifdef _WIN32
    skybox_material.shader.load_file_shaders("..\\resources\\shaders\\skybox.vs", "..\\resources\\shaders\\skybox.fs");
#else
    skybox_material.shader.load_file_shaders("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
#endif

    skybox_material.add_cubemap_texture(texture_dir);
}

void sSkyBoxRenderer::render(const glm::mat4x4        &viewproj_mat,
                             const sCamera       &camera) const {
    glBindVertexArray(VAO);
    //glDepthFunc(GL_LEQUAL);

    //shader.activate();// enable();

    skybox_material.enable();

    glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), camera.position);

    // TODO: This part is kinda yuck yuck bro...
    skybox_material.shader.set_uniform_matrix4("u_model_mat", model);
    skybox_material.shader.set_uniform_matrix4("u_viewproj_mat", viewproj_mat);
    skybox_material.shader.set_uniform("u_skybox", 0);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
    skybox_material.disable();

    //glDepthFunc(GL_LESS);

    glBindVertexArray(0);
}

void sSkyBoxRenderer::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Delete shader
}
