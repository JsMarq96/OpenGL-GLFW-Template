#ifndef RENDER_CONTEXT_H_
#define RENDER_CONTEXT_H_

#include "texture.h"
#include "render_quad.h"

#include "glcorearb.h"
#include <GL/gl3w.h>
#include <cstdint>
#include <string.h>
#include <cassert>
#include <stdlib.h>

#include <stb_image.h>
#include <stb_image_write.h>

#include <iostream>

struct sRenderContext {
    uint32_t  FBO = 0;

    uint32_t  color_attachment = 0;
    uint32_t  depth_attachment = 0;

    uint16_t  window_width = 0;
    uint16_t  window_height = 0;

    sQuadRenderer screen_quad = {};

    void init(const uint16_t width,
              const uint16_t height) {
        // Init Texture
        uint32_t text_ids[2] = {0, 0};
        glGenTextures(2, text_ids);

        color_attachment = text_ids[0];
        depth_attachment = text_ids[1];

        // Color texture
        glBindTexture(GL_TEXTURE_2D, color_attachment);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_BGRA, GL_FLOAT, NULL);

        // Depth texture
        glBindTexture(GL_TEXTURE_2D, depth_attachment);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        // Generate the FBO
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attachment, 0);

        uint32_t render_textures[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
        glDrawBuffers(2, render_textures);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            assert("Render context error");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        window_height = height;
        window_width = width;

        screen_quad.init();
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }

    void unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void store_attachments_to_CPU(const char* color_txt,
                                  const char* depth_txt) {
        uint32_t pixel_len = window_height * window_width;
        unsigned char *raw_color = (unsigned char*) malloc(sizeof(unsigned char) * pixel_len * 4);
        float *raw_depth = (float*) malloc(sizeof(float) * pixel_len * 1);

        glReadPixels(0,
                     0,
                     window_width,
                     window_height,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     raw_color);

        glReadPixels(0,
                     0,
                     window_width,
                     window_height,
                     GL_DEPTH_COMPONENT,
                     GL_FLOAT,
                     raw_depth);

        float t = -0.001f;
        for(uint32_t i = 0; i < t; i++) {
            t = MAX(t, raw_depth[i]);
        }
        /*glReadPixels(0,
                     0,
                     depth_texture.width,
                     depth_texture.height,
                     GL_DEPTH_COMPONENT,
                     GL_FLOAT,
                     depth_texture.raw_data);*/
        std::cout << raw_depth[0]  << " " << t << std::endl;

        stbi_write_png(color_txt,
                       window_width,
                       window_height,
                       4,
                       raw_color,
                       window_width * 4);

        stbi_write_png(depth_txt,
                       window_width,
                       window_height,
                       1,
                       raw_depth,
                       window_width * 1);


        free(raw_color);
        free(raw_depth);
    }

    void render_to_screen(const uint16_t width,
                          const uint16_t height) const {
        unbind();
        glViewport(0,0, width, height);

        // Clear render window
        glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screen_quad.render(color_attachment,
                           depth_attachment);
    }

    void clean() {
        uint32_t text_ids[2] = {color_attachment, depth_attachment};
        glDeleteTextures(2, text_ids);
        glDeleteFramebuffers(1, &FBO);
    }

};

#endif // RENDER_CONTEXT_H_
