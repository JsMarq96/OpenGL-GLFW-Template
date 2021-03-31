#ifndef _INPUT_LAYER_H_
#define _INPUT_LAYER_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

/**
 * Wrapper arround GLFW for easy access of periferall input
 * TODO: Controller, and more keys (if needed)
 * by Juan S. Marquerie
*/
#define KEY_NUM 9

enum eKeyMaps : char {
    W_KEY           = 0,
    A_KEY           = 1,
    S_KEY           = 2,
    D_KEY           = 3,
    SPACE_KEY       = 4,
    UP_KEY          = 5,
    LEFT_KEY        = 6,
    RIGHT_KEY       = 7,
    DOWN_KEY        = 8
};

enum eMouseClickMaps : char {
    RIGHT_CLICK     = 0,
    LEFT_CLICK      = 1,
    MIDDLE_CLICK    = 2
};

enum eKeyStates : bool {
    KEY_PRESSED     = true,
    KEY_RELEASED    = false
};

struct sInputLayer {
    // Keyboard state
    eKeyStates       keyboard[KEY_NUM];

    // Mouse state
    eKeyStates       mouse_clicks[3];
    bool             is_mouse_on_screen     = false;
    double           mouse_pos_x            = 0.0;
    double           mouse_pos_y            = 0.0;
    double           mouse_speed_x          = 0.0;
    double           mouse_speed_y          = 0.0;
};

static sInputLayer* get_game_input_instance() {
    static sInputLayer instance;
    return &instance;
};

#endif
