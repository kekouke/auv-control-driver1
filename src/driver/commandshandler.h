#pragma once
#include <vector>
#include <functional>

#include "command.h"

class CommandsHandler
{
public:
    CommandsHandler();
    void Add(const std::function<void()>& action) {
        commands.push_back({action});
    }
    void ExecuteCommands() {
        for (const auto& command : commands){
            command.Execute();
        }
    }

private:
    std::vector<Command> commands;
};

