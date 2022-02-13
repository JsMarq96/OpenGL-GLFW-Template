//
// Created by Juan S. Marquerie on 31/05/2021.
//

#ifndef QUEST_DEMO_MATERIAL_H
#define QUEST_DEMO_MATERIAL_H

//#include <stdlib.h>

#include <GL/gl3w.h>
#include <cstddef>

#include "glcorearb.h"
#include "texture.h"
#include "shader.h"

#define TEXTURE_SIZE 3

enum eTextureType : int {
    COLOR_MAP = 0,
    NORMAL_MAP,
    SPECULAR_MAP,
    METALLIC_ROUGHNESS_MAP,
    TEXTURE_TYPE_COUNT
};

const char texture_uniform_LUT[TEXTURE_TYPE_COUNT][25] = {
   "u_albedo_map",
   "u_normal_map",
   "u_metallic_rough_map",
   "u_metallic_rough_map"
};

struct sMaterial {
    sTexture        textures[TEXTURE_TYPE_COUNT];
    bool            enabled_textures[TEXTURE_TYPE_COUNT] = {false};

    sShader         shader;

    void add_shader(const char     *vertex_shader,
                    const char     *fragment_shader);

    void add_texture(const char*          text_dir,
                     const eTextureType   text_type);

    void add_raw_texture(const char* raw_data,
                         const size_t width,
                         const size_t height,
                         const GLenum format,
                         const GLenum type,
                         const eTextureType text_type);

    void add_cubemap_texture(const char  *text_dir);

    uint8_t get_used_textures() const {
        uint8_t tmp = 0b0;

        if (enabled_textures[COLOR_MAP]) {
            tmp |= 0b1;
        }
        if (enabled_textures[NORMAL_MAP]) {
            tmp |= 0b10;
        }
        if (enabled_textures[SPECULAR_MAP]) {
            tmp |= 0b100;
        }
        if (enabled_textures[METALLIC_ROUGHNESS_MAP]) {
            tmp |= 0b1000;
        }

        return tmp;
    };

    /**
    * Binds the textures on Opengl
    *  COLOR - Texture 0
    *  NORMAL - Texture 1
    *  SPECULAR - TEXTURE 2
    * */
    void enable() const;

    void disable() const;
};

#endif //QUEST_DEMO_MATERIAL_H
