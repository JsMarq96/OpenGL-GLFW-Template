//
// Created by Juan S. Marquerie on 31/05/2021.
//

#include "material.h"

void material_add_shader(sMaterial      *mat,
                         const char     *vertex_shader,
                         const char     *fragment_shader) {
    mat->shader.load_shaders(vertex_shader,
                             fragment_shader);
}

void material_add_texture(sMaterial            *mat,
                          const char*           text_dir,
                          const eTextureType   text_type) {
    mat->enabled_textures[text_type] = true;
    load_texture(&mat->textures[text_type],
                 false,
                 false,
                 text_dir);
}

void material_add_cubemap_texture(sMaterial   *mat,
                                  const char   *text_dir) {
    mat->enabled_textures[COLOR_MAP] = true;
    load_texture(&mat->textures[COLOR_MAP],
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
void material_enable(const sMaterial   *mat) {
    for (int texture = 0; texture < TEXTURE_TYPE_COUNT; texture++) {
        if (!mat->enabled_textures[texture]) {
            continue;
        }
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, mat->textures[texture].texture_id);
    }

    mat->shader.enable();
}

void material_disable(const sMaterial   *mat) {
    mat->shader.disable();
}