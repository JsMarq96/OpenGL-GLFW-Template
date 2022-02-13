#ifndef RENDER_QUAD_H_
#define RENDER_QUAD_H_

#include <GL/gl3w.h>
#include <cstdint>
#include <string.h>
#include <cassert>
#include <stdlib.h>

#include "glcorearb.h"
#include "texture.h"
#include "material.h"


static const float quad_geometry[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};


// TODO: Add a postprocessing element?
struct sQuadRenderer {
    uint32_t VAO = 0;
    uint32_t VBO = 0;

    sMaterial quad_material;

    void init() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_geometry), quad_geometry, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

        glBindVertexArray(0);

        // TODO: Config shader & material
        quad_material.shader.load_file_shaders("resources/shaders/quad.vs",
                                               "resources/shaders/quad.fs");
    }

    void render(const uint32_t color_texture,
                const uint32_t depth_texture) const {
        glBindVertexArray(VAO);
        quad_material.enable();

        // Bind FBO color / texture color
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, color_texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depth_texture);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        quad_material.disable();
        glBindVertexArray(0);

    }
};

#endif // RENDER_QUAD_H_
