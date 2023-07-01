#pragma once

#include <SDL2/SDL.h>
#include <vector>

struct ButtonEvent {
    SDL_GameControllerButton Button;
    bool State;
};

struct ButtonState {
    bool PreviousState;
    bool CurrentState;
    SDL_GameControllerButton Button;
};

// Adapted from SDL - see SDL_GameControllerAxis(https://wiki.libsdl.org/SDL2/SDL_GameControllerAxis)
enum class Axis
{
    LeftStickHorizontal,
    LeftStickVertical,
    RightStickHorizontal,
    RightStickVertical,
    Count
};

class Gamepad
{
public:

    static const int ButtonCount = SDL_CONTROLLER_BUTTON_MAX;
    static const int AxisCount = 4;

    bool Open(int deviceIndex);
    void Close();
    Gamepad();
    ~Gamepad();

    const std::vector<ButtonState>& GetKeys();
    const std::vector<double>& GetAxes();

    double GetValueForAxis(Axis axis);

    bool HasValueForAxis(Axis i);

    void SetButtonState(SDL_GameControllerButton button, bool value);
    void SetAxisValue(Axis axis, int value);
    bool WasKeyPressed(int i) const;
    bool IsKeyPressed(int i) const;
    void ConsumeKey(int i);
    void ProcessPendingKeyEvents();
    bool IsAtached();
    void ClearKeyState();

private:
    const int HOLD_THRESHOLD_MS = 300;
    const float DEADZONE = 0.2f;

    void InitializeKeys();
    void SetKeyState(int i, bool state);

    SDL_GameController* gameController = nullptr;
    std::vector<ButtonState> keys;
    std::vector<double> axes;
    std::vector<ButtonEvent> buttonEventQueue;
};
