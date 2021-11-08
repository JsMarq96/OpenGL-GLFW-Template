//
// Created by Juan S. Marquerie on 31/05/2021.
//

#ifndef QUEST_DEMO_MATERIAL_H
#define QUEST_DEMO_MATERIAL_H

//#include <stdlib.h>

#include <GL/gl3w.h>

#include "texture.h"
#include "shader.h"

#define TEXTURE_SIZE 3

enum eTextureType : int {
    COLOR_MAP = 0,
    NORMAL_MAP,
    SPECULAR_MAP,
    TEXTURE_TYPE_COUNT
};

struct sMaterial {
    sTexture        textures[TEXTURE_TYPE_COUNT];
    bool            enabled_textures[TEXTURE_TYPE_COUNT] = {false};

    sShader         shader;

    void material_add_shader(const char     *vertex_shader,
                             const char     *fragment_shader);

    void material_add_texture(const char*          text_dir,
                              const eTextureType   text_type);

    void material_add_cubemap_texture(const char  *text_dir);

    /**
    * Binds the textures on Opengl
    *  COLOR - Texture 0
    *  NORMAL - Texture 1
    *  SPECULAR - TEXTURE 2
    * */
    void material_enable();

    void material_disable();
};

#endif //QUEST_DEMO_MATERIAL_H
