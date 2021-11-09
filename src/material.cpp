//
// Created by Juan S. Marquerie on 31/05/2021.
//

#include "material.h"

void sMaterial::material_add_shader(const char     *vertex_shader,
                                    const char     *fragment_shader) {
    shader.load_shaders(vertex_shader,
                             fragment_shader);
}

void sMaterial::material_add_texture(const char*           text_dir,
                                     const eTextureType   text_type) {
    enabled_textures[text_type] = true;
    load_texture(&textures[text_type],
                 false,
                 false,
                 text_dir);
}

void sMaterial::material_add_cubemap_texture(const char   *text_dir) {
    enabled_textures[COLOR_MAP] = true;
    load_texture(&textures[COLOR_MAP],
                 true,
                 false,
                 text_dir);
}

/**
 * Binds the textures on Opengl
 *  COLOR - Texture 0
 *  NORMAL - Texture 1
 *  SPECULAR - TEXTURE 2
 * */
void sMaterial::material_enable() {
    for (int texture = 0; texture < TEXTURE_TYPE_COUNT; texture++) {
        if (!enabled_textures[texture]) {
            continue;
        }
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, textures[texture].texture_id);
    }

    //shader.enable();
}

void sMaterial::material_disable() {
    //shader.disable();
}
