//
// Created by Juan S. Marquerie on 31/05/2021.
//

#include "material.h"
#include "gl3w.h"
#include "glcorearb.h"
#include "texture.h"

void sMaterial::add_shader(const char     *vertex_shader,
                                    const char     *fragment_shader) {
    shader.load_file_shaders(vertex_shader,
                             fragment_shader);
}

void sMaterial::add_texture(const char*           text_dir,
                            const eTextureType   text_type) {
    enabled_textures[text_type] = true;
    load_texture(&textures[text_type],
                 false,
                 false,
                 text_dir);
}

// TODO: this is kinda messy... refactor to sTexture??
void sMaterial::add_raw_texture(const char* raw_data,
                                const size_t width,
                                const size_t height,
                                const GLenum format,
                                const GLenum type,
                                const eTextureType text_type) {
    enabled_textures[text_type] = true;

    assert(raw_data != NULL && "Uploading empty texture to GPU");

    sTexture *curr_texture = &textures[text_type];

    glGenTextures(1, &curr_texture->texture_id);

    glBindTexture(GL_TEXTURE_2D, curr_texture->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width, height,
                 0,
                 format,
                 type,
                 raw_data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void sMaterial::add_cubemap_texture(const char   *text_dir) {
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
void sMaterial::enable() const {
    shader.activate();

    for (int texture = 0; texture < TEXTURE_TYPE_COUNT; texture++) {
        if (!enabled_textures[texture]) {
            continue;
        }
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, textures[texture].texture_id);

        shader.set_uniform_texture(texture_uniform_LUT[texture], texture);
    }
}

void sMaterial::disable() const {
    //shader.disable();
}
