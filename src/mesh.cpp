#include "mesh.h"
#include <stdio.h>

#ifdef _WIN32
/**
 * Define the getline function on windows
 * Source https://gist.github.com/jstaursky/84cf1ddf91716d31558d6f0b5afc3feb
*/
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>

typedef ptrdiff_t ssize_t;

ssize_t
getline(char** lineptr, /* input/output - storage address for input stream */
    size_t* n,       /* input/output - string size in bytes */
    FILE* stream) /* input - text stream */
{
    char* buffer_tmp, * position;
    size_t   block, offset;
    int      c;

    if (!stream || !lineptr || !n) {
        errno = EINVAL;
        return -1;
        /* Minimum length for strings is 2 bytes. */
    }
    else if (*lineptr == NULL || *n <= 1) {
        *n = 128;
        if (!(*lineptr = (char*) malloc(*n))) {
            errno = ENOMEM;
            return -1;
        }
    }
    block = *n;
    position = *lineptr;

    while ((c = fgetc(stream)) != EOF \
        && (*position++ = c) != '\n') {
        offset = position - *lineptr; /* realloc doesn't guarantee it can keep '*lineptr' in same memory spot.
                                       * so needs keep track of offset. */
        if (offset >= *n) {
            buffer_tmp = (char*) realloc(*lineptr, *n += block);
            if (!buffer_tmp) { /* Do not free. Return *lineptr. */
                errno = ENOMEM;
                return -1;
            }
            *lineptr = buffer_tmp;
            position = *lineptr + offset;
        }
    }
    *position = '\0';
    return (position - *lineptr - 1);
}
#endif

void sMesh::load_OBJ_mesh(const char* mesh_dir) {
    FILE *mesh_file;
    int mesh_file_size;
    char *raw_mesh;

    mesh_file = fopen(mesh_dir, "r");

    assert(mesh_file != NULL && "Failed to open the mesh file");

    // First coutn the faces & vertices
    int read_chars;
    char *line_buffer = NULL;
    size_t len = 0;

    int uv_total_count = 0;
    int normal_total_count = 0;
    int vertex_total_count = 0;

    while((read_chars = getline(&line_buffer, &len, mesh_file)) != -1) {
        if (line_buffer[0] == 'v' && line_buffer[1] == ' ') {
            vertex_total_count++;
        } else if (line_buffer[0] == 'f') {
            face_count++;
        } else if (line_buffer[0] == 'v' && line_buffer[1] == 't') {
            uv_total_count++;
        }else if (line_buffer[0] == 'v' && line_buffer[1] == 'n') {
            normal_total_count++;
        }
    }

    vertices_index = (uint16_t*) malloc(face_count * 3 * sizeof(uint16_t));
    face_normals = (glm::vec3*) malloc(face_count * sizeof(glm::vec3));

    struct sUV_Wrapper {
        float u;
        float v;
    };

    glm::vec3 *temp_vertices = (glm::vec3*) malloc(vertex_total_count * sizeof(glm::vec3));
    glm::vec3 *temp_normals = (glm::vec3*) malloc(normal_total_count * sizeof(glm::vec3));
    sUV_Wrapper *tmp_uvs = (sUV_Wrapper*) malloc(sizeof(sUV_Wrapper) * (uv_total_count));


    vertices = (sRawVertex*) malloc(face_count * 3 * sizeof(sRawVertex));

    rewind(mesh_file);

    int raw_vertex_index = 0;
    int vertex_index = 0;
    int normal_count = 0;
    int half_edge_index = 0;
    int uv_count = 0;
    int face_index = 0;
    int index_count = 0;

    sKVStorage vertex_map;
    vertex_map.init();

    char tmp_edge_id[12] = {""};

     while((read_chars = getline(&line_buffer, &len, mesh_file)) != -1) {
        if (line_buffer[0] == 'v' && line_buffer[1] == ' ') {
            float x, y, z;
            sscanf(line_buffer,"v %f %f %f\n",&x, &y, &z);
            temp_vertices[vertex_index].x = x;
            temp_vertices[vertex_index].y = y;
            temp_vertices[vertex_index].z = z;
            vertex_index++;
        } else if (line_buffer[0] == 'v' && line_buffer[1] == 't') {
            float u,v;
            sscanf(line_buffer, "vt %f %f\n", &u, &v);

            // Store the UVs by tuples
            tmp_uvs[uv_count].u = u;
            tmp_uvs[uv_count].v = 1.0f - v;
            uv_count++;
        } else if (line_buffer[0] == 'v' && line_buffer[1] == 'n') {
            glm::vec3 *curr_normal = &temp_normals[normal_count];
            sscanf(line_buffer, "vn %f %f %f\n", &curr_normal->x, &curr_normal->y, &curr_normal->z);

            normal_count++;
        } else if (line_buffer[0] == 'f') {
            int index1, index2, index3, normal1, normal2, normal3, uv1, uv2, uv3;
            sscanf(line_buffer,
                   "f %i/%i/%i %i/%i/%i %i/%i/%i\n",
                   &index1,
                   &uv1,
                   &normal1,
                   &index2,
                   &uv2,
                   &normal2,
                   &index3,
                   &uv3,
                   &normal3);

            index1 -= 1;
            index2 -= 1;
            index3 -= 1;
            uv1    -= 1;
            uv2    -= 1;
            uv3    -= 1;
            normal1 -=1;
            normal2 -=1;
            normal3 -=1;

            // Vertex 1
            memset(tmp_edge_id, 0, 12);
            get_key_of_vertex(index1, normal1, uv1, tmp_edge_id);
            int vertex_id = vertex_map.get_int(tmp_edge_id, 12);
            if (vertex_id == -1) {
                vertices[vertex_count].vertex = temp_vertices[index1];
                vertices[vertex_count].normal = temp_normals[normal1];
                vertices[vertex_count].u = tmp_uvs[uv1].u;
                vertices[vertex_count].v = tmp_uvs[uv1].v;
                vertex_id = vertex_count;
                vertex_map.add(tmp_edge_id, 12, vertex_count);
                vertex_count++;
            }
            vertices_index[indexing_count++] = vertex_id;

            // Vertex 2
            memset(tmp_edge_id, 0, 12);
            get_key_of_vertex(index2, normal2, uv2, tmp_edge_id);
            vertex_id = vertex_map.get_int(tmp_edge_id, 12);
            if (vertex_id == -1) {
                vertices[vertex_count].vertex = temp_vertices[index2];
                vertices[vertex_count].normal = temp_normals[normal2];
                vertices[vertex_count].u = tmp_uvs[uv2].u;
                vertices[vertex_count].v = tmp_uvs[uv2].v;
                vertex_id = vertex_count;
                vertex_map.add(tmp_edge_id, 12, vertex_count);
                vertex_count++;
            }
            vertices_index[indexing_count++] = vertex_id;

              // Vertex 3
            memset(tmp_edge_id, 0, 12);
            get_key_of_vertex(index3, normal3, uv3, tmp_edge_id);
            vertex_id = vertex_map.get_int(tmp_edge_id, 12);
            if (vertex_id == -1) {
                vertices[vertex_count].vertex = temp_vertices[index3];
                vertices[vertex_count].normal = temp_normals[normal3];
                vertices[vertex_count].u = tmp_uvs[uv3].u;
                vertices[vertex_count].v = tmp_uvs[uv3].v;
                vertex_id = vertex_count;
                vertex_map.add(tmp_edge_id, 12, vertex_count);
                vertex_count++;
            }
            vertices_index[indexing_count++] = vertex_id;

            // Compute the face normal
            //face_normals[face_index].x = (temp_normals[normal1].x + temp_normals[normal2].x + temp_normals[normal3].x) / 3.0f;
            //face_normals[face_index].y = (temp_normals[normal1].y + temp_normals[normal2].y + temp_normals[normal3].y) / 3.0f;
            //face_normals[face_index].z = (temp_normals[normal1].z + temp_normals[normal2].z + temp_normals[normal3].z) / 3.0f;

            face_index++;
        }
    }

    free(temp_vertices);
    free(line_buffer);
    free(tmp_uvs);
    free(temp_normals);

    fclose(mesh_file);
    vertex_map.clean();
}


void sMesh::clean() {
    free(vertices_index);
    free(vertices);
    free(face_normals);
}
