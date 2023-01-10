#ifndef MESH_H_
#define MESH_H_

#include <glm/glm.hpp>
#include "kv_storage.h"
#include <cstdint>

inline void get_key_of_vertex(const uint16_t vertex,
                              const uint16_t normal,
                              const uint16_t uv,
                                    char result[12]) {
    // Union to get a key of the vertex
    union {struct {uint16_t v;  uint16_t n; uint16_t _uv;};
           char key[12] = {'\0'};
    } union_key;

    union_key.v = vertex;
    union_key.n = normal;
    union_key._uv = uv;

    for(int i = 0; i < 12; i++) {
        result[i] = union_key.key[i]+1;
    }
    //memcpy(result, union_key.key, sizeof(char[12]));
}

struct sRawVertex {
    glm::vec3 vertex = {0.0f, 0.0f, 0.0f};
    float u = 0.0f;
    float v = 0.0f;
    glm::vec3 normal = {0.0f, 0.0f, 0.0f};
};

struct sMesh {
    uint16_t    vertex_count    = 0;
    uint16_t    face_count      = 0;
    uint16_t    indexing_count  = 0;

    uint16_t    *vertices_index = NULL;

    sRawVertex  *vertices     = NULL;

    glm::vec3    *face_normals   = NULL;

    glm::vec3    *face_middle = NULL;


    void load_OBJ_mesh(const char* mesh_dir);

    void clean();
};


#endif // MESH_H_
