#pragma once

#include <iostream>

#include "gamepad.h"
#include "sender.h"
#include "messages.h"
#include "motion.h"
#include "commandshandler.h"

#include "SDL2/SDL.h"
#undef main

class Application
{
public:
    Application();
    ~Application();

    void Run();
    void Initialize(int argc, char *argv[], ipc::Core::Description description);

    bool IsControlEnable() const;

private:
    void OnJoystickConnected(int deviceIndex);
    void OnJoystickDisconnected();

    void PollEvents(SDL_Event& event);
    void ProcessCommands();

    void SetDefaultDataForControlCommandSender();
    void ToggleInputControl(bool value);

    void LoadGamepadBindings();
    void CreateCommands();

    Gamepad* gamepad;
    ipc::Core* core;
    Sender<motion::Control>* controlSender = nullptr;
    Sender<Message::State>* programStateSender = nullptr;
    Sender<Message::GamepadState>* gamepadStateSender = nullptr;

    double sendStateInterval;
    double sendDataInterval;

    bool isControlEnable = false;
    bool isGamepadAvailable = false;

    CommandsHandler commandsHandler;
};
