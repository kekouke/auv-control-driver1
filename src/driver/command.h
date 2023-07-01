#pragma once

#include <functional>

#include "SDL2/SDL.h"

class Command {
public:
    Command(const std::function<void()>& action);

    void Execute() const;

private:
    std::function<void()> f;
};
