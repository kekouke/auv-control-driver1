#include "gamepad.h"

Gamepad::Gamepad()
{
    keys = std::vector<ButtonState>(SDL_CONTROLLER_BUTTON_MAX);
    axes = std::vector<double>(SDL_CONTROLLER_AXIS_MAX);

    InitializeKeys();
}

void Gamepad::InitializeKeys()
{
    int buttonIndex = 0;

    for (auto& key : keys) {
        key.Button = SDL_GameControllerButton(buttonIndex++);
    }
}

Gamepad::~Gamepad()
{
    Close();
}

void Gamepad::Close()
{
    if (gameController != nullptr) {
        SDL_GameControllerClose(gameController);
        gameController = nullptr;
    }
}

bool Gamepad::Open(int deviceIndex)
{
    gameController = SDL_GameControllerOpen(deviceIndex);
    return gameController != nullptr;
}

bool Gamepad::IsAtached()
{
    if (gameController == nullptr) return false;
    return SDL_GameControllerGetAttached(gameController);
}

void Gamepad::ClearKeyState() {
      for (int i = 0; i < ButtonCount; i++) {
          keys[i].CurrentState = false;
      }
      buttonEventQueue.clear();
  }

void Gamepad::SetButtonState(SDL_GameControllerButton button, bool value)
{
    buttonEventQueue.push_back({button, value});
}

void Gamepad::SetAxisValue(Axis axis, int value) {
    int index = static_cast<int>(axis);
    if (index < 0 || index >= axes.size()) {
        return;
    }

    double newValue = static_cast<float>(value) / 32767.f;
    if (newValue <= -1.0f) {
        newValue = -1.0f;
    }
    else if (newValue >= 1.0f) {
        newValue = 1.0f;
    }

    axes[index] = abs(newValue) >= DEADZONE ? newValue : 0.0;
}

void Gamepad::ProcessPendingKeyEvents()
{
   std::vector<ButtonEvent> processedQueue;
   std::vector<bool> used(Gamepad::ButtonCount);
   for (const auto& key : buttonEventQueue) {
       if (used[key.Button]) {
           processedQueue.push_back(key);
           continue;
       }
       SetKeyState(key.Button, key.State);
       used[key.Button] = true;
   }
   buttonEventQueue = processedQueue;
}

void Gamepad::SetKeyState(int i, bool state)
{
    keys[i].PreviousState = keys[i].CurrentState;
    keys[i].CurrentState = state;
}

bool Gamepad::WasKeyPressed(int i) const
{
    return keys[i].CurrentState && !keys[i].PreviousState;
}

bool Gamepad::IsKeyPressed(int i) const {
    return keys[i].CurrentState;
}

void Gamepad::ConsumeKey(int i)
{
    keys[i].PreviousState = keys[i].CurrentState;
}

bool Gamepad::HasValueForAxis(Axis axis) {
    return abs(GetValueForAxis(axis)) >= DEADZONE;
}

double Gamepad::GetValueForAxis(Axis axis)
{
    int index = static_cast<int>(axis);
    if (index < 0 || index >= axes.size()) {
        return 0;
    }
    return axes[index];
}

const std::vector<ButtonState>& Gamepad::GetKeys()
{
    return keys;
}

const std::vector<double>& Gamepad::GetAxes()
{
    return axes;
}
