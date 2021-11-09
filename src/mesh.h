//
// Created by jsmar on 21/05/2021.
//

#ifndef QUEST_DEMO_MESH_H
#define QUEST_DEMO_MESH_H

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "math.h"

struct sGlVertex {
    float x  = 0.0f;
    float y  = 0.0f;
    float z  = 0.0f;

    float u  = 0.0f;
    float v  = 0.0f;
};

struct sMesh {
    union {
        float          *raw_vertex_list = nullptr;
        sGlVertex      *vertex_list;
    };
    unsigned int    vertex_count = 0;
    unsigned int    raw_vertex_size = 0;
    unsigned int    uv_count = 0;

    unsigned int   *faces_index = nullptr;
    unsigned int    indices_cout = 0;
};

void
load_mesh(sMesh *result,
          const char* mesh_dir);

void
load_cube_mesh(sMesh *result);

void
mesh_destroy(sMesh *to_dispose);

sVector3
get_bounding_box_dimensions(const sMesh *mesh,
                            const sVector3 scale);
#endif //QUEST_DEMO_MESH_H
