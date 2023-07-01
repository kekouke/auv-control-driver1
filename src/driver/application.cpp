#include "application.h"
#include "motion.h"
#include "commands.h"


Application::Application()
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

Application::~Application()
{
    delete core;
    delete controlSender;
    delete gamepad;
    delete gamepadStateSender;
    delete programStateSender;

    core = nullptr;
    controlSender = nullptr;
    gamepad = nullptr;
    gamepadStateSender = nullptr;
    programStateSender = nullptr;
}

void Application::Initialize(int argc, char *argv[], ipc::Core::Description description)
{
    core = new ipc::Core(argc, argv, description);
    gamepad = new Gamepad();

    programStateSender = new Sender<Message::State>(core);
    programStateSender->Initialize();

    const Message::State& programState = programStateSender->GetData();
    sendStateInterval = programState.settings.state_timer;
    sendDataInterval =programState.settings.send_timer;

    LoadGamepadBindings();

    controlSender = new Sender<motion::Control>(core);
    SetDefaultDataForControlCommandSender();

    CreateCommands();
}

void Application::LoadGamepadBindings() {
    const auto& gamepadBindings = programStateSender->GetData().bindings;
    Commands::start_control.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.start_control.binding.to_std_string().c_str()
    );
    Commands::stop_control.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.stop_control.binding.to_std_string().c_str()
    );

    Commands::set_min_speed.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.set_min_speed.binding.to_std_string().c_str()
    );
    Commands::set_slow_speed.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.set_slow_speed.binding.to_std_string().c_str()
    );
    Commands::set_max_speed.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.set_max_speed.binding.to_std_string().c_str()
    );
    Commands::set_zero_speed.bindingKey = SDL_GameControllerGetButtonFromString(
        gamepadBindings.button_commands.set_zero_speed.binding.to_std_string().c_str()
    );


    Commands::move_forward.bindingAxis = SDL_GameControllerGetAxisFromString(
        gamepadBindings.axis_commands.move_forward.binding.to_std_string().c_str()
    );
    Commands::move_right.bindingAxis = SDL_GameControllerGetAxisFromString(
        gamepadBindings.axis_commands.move_right.binding.to_std_string().c_str()
    );
}

void Application::SetDefaultDataForControlCommandSender() {
    motion::Control& controlData= controlSender->GetData();
    controlData.parameters[geo::Right]  .value = 0;
    controlData.parameters[geo::Right]  .type  = motion::ControlType::Force;
    controlData.parameters[geo::Right]  .frame = scene::Absent;

    controlData.parameters[geo::Forward].value = 0;
    controlData.parameters[geo::Forward].type  = motion::ControlType::Force;
    controlData.parameters[geo::Forward].frame = scene::Absent;

    controlData.parameters[geo::Up]     .value = 0;
    controlData.parameters[geo::Up]     .type  = motion::ControlType::Position;
    controlData.parameters[geo::Up]     .frame = scene::Zero;

    controlData.parameters[geo::Yaw]    .value = 0;
    controlData.parameters[geo::Yaw]    .type  = motion::ControlType::Force;
    controlData.parameters[geo::Yaw]    .frame = scene::Absent;

    controlData.parameters[geo::Pitch]  .value = 0;
    controlData.parameters[geo::Pitch]  .type  = motion::ControlType::Force;
    controlData.parameters[geo::Pitch]  .frame = scene::Absent;

    controlData.parameters[geo::Roll]   .value = 0;
    controlData.parameters[geo::Roll]   .type  = motion::ControlType::Force;
    controlData.parameters[geo::Roll]   .frame = scene::Absent;
}

void Application::Run()
{
    ipc::Timer sendTimer(*core);
    ipc::Timer tmr_state(*core);
    sendTimer.start(sendDataInterval);
    tmr_state.start(sendStateInterval);

    SDL_Event event;

    while (core->receive()) {
        if (tmr_state.received()) {
            programStateSender->Send();
            // Только одно событие в итерации цикла!
            continue;
        }

        PollEvents(event);

        if (sendTimer.received() && isGamepadAvailable) {
            gamepad->ProcessPendingKeyEvents();
            Message::GamepadState& gamepadState = gamepadStateSender->GetData();
            for (int i = 0; i < Gamepad::AxisCount; i++) {
                gamepadState.axesState[i].value = gamepad->GetValueForAxis(Axis(i));
            }
            for (int i = 0; i < Gamepad::ButtonCount; i++) {
                gamepadState.buttonStates[i].isPressed = gamepad->IsKeyPressed(i);
            }
            gamepadStateSender->Send();
            ProcessCommands();
            continue;
        }
    }
}

void Application::PollEvents(SDL_Event &event)
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_CONTROLLERDEVICEADDED:
                OnJoystickConnected(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                OnJoystickDisconnected();
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                gamepad->SetButtonState(SDL_GameControllerButton(event.cbutton.button), true);
                break;
            case SDL_CONTROLLERBUTTONUP:
                gamepad->SetButtonState(SDL_GameControllerButton(event.cbutton.button), false);
                break;
            case SDL_CONTROLLERAXISMOTION:
                gamepad->SetAxisValue((Axis)event.caxis.axis, event.caxis.value);
                break;
        }
    }
}

void Application::OnJoystickConnected(int deviceIndex)
{
    if (!SDL_IsGameController(deviceIndex)) {
        return;
    }
    if (!isGamepadAvailable) {
        gamepad->Open(deviceIndex);
        isGamepadAvailable = true;
        ToggleInputControl(false);
        std::cout << "Gamepad connected." << std::endl;
        if (gamepadStateSender == nullptr) {
            gamepadStateSender = new Sender<Message::GamepadState>(core);
            gamepadStateSender->Initialize();
        }
        core->log("Устройство подключено");
    }
}

void Application::ToggleInputControl(bool value)
{
    isControlEnable = value;
}

void Application::OnJoystickDisconnected()
{
    if (!gamepad->IsAtached()) {
        std::cout << "Gamepad disconnected." << std::endl;
        core->log("Устройство отключено");
        gamepad->Close();
        isGamepadAvailable = false;
        ToggleInputControl(false);
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (!SDL_IsGameController(i)) {
                continue;
            }
            if (gamepad->Open(i)) {
                isGamepadAvailable = true;
                return;
            }
        }
    }
}

void Application::ProcessCommands()
{
    commandsHandler.ExecuteCommands();
}

bool Application::IsControlEnable() const
{
    return isControlEnable;
}

void Application::CreateCommands() {
    commandsHandler.Add(
        [this]() {
        if (gamepad->WasKeyPressed(Commands::start_control.bindingKey)) {
            gamepad->ConsumeKey(Commands::start_control.bindingKey);
            ToggleInputControl(true);
            std::cout << "Control on" << std::endl;
            core->log("Управление включено");
        }
        return;
    });
    commandsHandler.Add(
        [this]() {
        if (gamepad->WasKeyPressed(Commands::stop_control.bindingKey)) {
            gamepad->ConsumeKey(Commands::stop_control.bindingKey);
            ToggleInputControl(false);
            std::cout << "Control off" << std::endl;
            core->log("Управление отключено");
        }
        return;
    });
    commandsHandler.Add(
        [this]() {
        if (!IsControlEnable()) {
            return;
        }

        bool hasChanges = false;

        if (gamepad->WasKeyPressed(Commands::set_zero_speed.bindingKey)) {
            hasChanges = true;
            SetDefaultDataForControlCommandSender();
            controlSender->Send();
            return;
        }

        double speed_coeff = 1;
        double force_up = 25;
        double force_pitch = 7.5;

        if (gamepad->WasKeyPressed(Commands::set_min_speed.bindingKey)) {
            speed_coeff = 0.2;
        }
        if (gamepad->WasKeyPressed(Commands::set_slow_speed.bindingKey)) {
            speed_coeff = 0.5;
        }
        if (gamepad->WasKeyPressed(Commands::set_max_speed.bindingKey)) {
            speed_coeff = 2;
        }

        motion::Control& controlData = controlSender->GetData();
        controlData.parameters[geo::Up].value = force_up;
        controlData.parameters[geo::Up].type  = motion::ControlType::Force;
        controlData.parameters[geo::Up].frame = scene::Absent;

        controlData.parameters[geo::Pitch].value = force_pitch;
        controlData.parameters[geo::Pitch].type  = motion::ControlType::Force;
        controlData.parameters[geo::Pitch].frame = scene::Absent;

        if (gamepad->HasValueForAxis((Axis)Commands::move_forward.bindingAxis)) {
            double velocity_forward = -gamepad->GetValueForAxis((Axis)Commands::move_forward.bindingAxis) * 5 * speed_coeff;

            controlData.parameters[geo::Forward].value = velocity_forward;
            controlData.parameters[geo::Forward].type  = motion::ControlType::Velocity;
            controlData.parameters[geo::Forward].frame = scene::Absent;

            hasChanges = true;
        }
        if (gamepad->HasValueForAxis((Axis)Commands::move_right.bindingAxis)) {
            double velocity_yaw = gamepad->GetValueForAxis((Axis)Commands::move_right.bindingAxis) * 5 * speed_coeff;

            controlData.parameters[geo::Yaw].value = velocity_yaw;
            controlData.parameters[geo::Yaw].type  = motion::ControlType::Velocity;
            controlData.parameters[geo::Yaw].frame = scene::Absent;

            hasChanges = true;
        }


        if (hasChanges) {
            controlSender->Send();
            SetDefaultDataForControlCommandSender();
        }

        return;
    });
}
