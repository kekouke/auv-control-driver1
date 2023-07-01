#include "command.h"

Command::Command(const std::function<void()>& action)
{
    f = action;
}

void Command::Execute() const
{
    f();
};
