#pragma once

#include "SDL2/SDL.h"
#undef main

struct KeyBinding {
   SDL_GameControllerButton bindingKey;
};

struct AxisBinding {
    SDL_GameControllerAxis bindingAxis;
};


class Commands
{
public:
    static KeyBinding start_control;
    static KeyBinding stop_control;
    static KeyBinding set_min_speed;
    static KeyBinding set_slow_speed;
    static KeyBinding set_max_speed;
    static KeyBinding set_zero_speed;
    static AxisBinding move_forward;
    static AxisBinding move_right;
};
