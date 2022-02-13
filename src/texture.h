//
// Created by Juan S. Marquerie on 29/05/2021.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/gl3w.h>
#include <string.h>
#include <cassert>

#define DEFAULT_TEXT_FIDELITY 0

struct sTexture {
    bool             store_on_RAM = false;
    bool             is_cube_map  = false;

    // Raw data
    int             width     = 0;
    int             height    = 0;
    int             layers    = 0;
    unsigned char   *raw_data = NULL;

    // OpenGL id
    unsigned int     texture_id;
};

void load_texture(sTexture  *text,
                  const bool is_cube_map,
                  const bool store_on_RAM,
                  const char *texture_name);

void store_texture(const sTexture *text,
                   const char *name);

void delete_texture(sTexture  *text);

#endif //_TEXTURE_H_
