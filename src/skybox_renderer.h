//
// Created by Juan S. Marquerie on 01/06/2021.
//

#ifndef QUEST_DEMO_SKYBOX_RENDERER_H
#define QUEST_DEMO_SKYBOX_RENDERER_H

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "material.h"

struct sSkyBoxRenderer {
    unsigned int    VAO;
    unsigned int    VBO;

    sShader         shader;
    sTexture        texture;
    sMaterial       skybox_material;

    void init(const char       *texture_dir);

    void render(       const glm::mat4x4        &viewproj_mat,
                       const sCamera       &camera) const;

    void destroy();

};

#endif //QUEST_DEMO_SKYBOX_RENDERER_H
