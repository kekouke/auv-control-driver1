#include "sender.h"
#include "gamepad.h"

template<>
void Sender<Message::GamepadState>::Initialize() {
    for (int i = 0; i < Gamepad::ButtonCount; i++) {
        sender->_.buttonStates[i].name =
            SDL_GameControllerGetStringForButton(SDL_GameControllerButton(i));
    }
    for (int i = 0; i < Gamepad::AxisCount; i++) {
        sender->_.axesState[i].name =
            SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(i));
    }
}

template<>
void Sender<Message::State>::Initialize() {
    ipc::Loader<Message::Init> init(*core);
    ipc::Loader<Message::GamepadBindings> gamepadBindings(*core);

    sender->_.settings = init._;
    sender->_.bindings = gamepadBindings._;
    sender->_.send_regime = true;
}

template<>
void Sender<motion::Control>::Initialize() {
    sender->_.is_compensation = false;
    sender->_.priority = motion::Priority::Highest;
    sender->_.time_limit = 2;
}
