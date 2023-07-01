#include "controller.h"

#include<iostream>

Controller::Controller(QObject *parent)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    gamepad = new Gamepad();

    LoadControlSettings(CONFIG_FILE_PATH);

    connectOrDisconnectJoystickTimer = new QTimer();
    connect(connectOrDisconnectJoystickTimer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    connectOrDisconnectJoystickTimer->start(100);
}

Controller::~Controller()
{
   connectOrDisconnectJoystickTimer->stop();
   gamepad->Close();
   delete gamepad;
   delete  connectOrDisconnectJoystickTimer;
   gamepad = nullptr;
   connectOrDisconnectJoystickTimer = nullptr;
}

void Controller::setup() {
    bool success = false;
    if (setupType == SetupType::Button) {
        while (!events.empty()) {
            SDL_Event event = events.front();
            events.pop();
            switch (event.type) {
                case SDL_CONTROLLERBUTTONDOWN:
                    gamepad->SetButtonState(SDL_GameControllerButton(event.cbutton.button), true);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                    gamepad->SetButtonState(SDL_GameControllerButton(event.cbutton.button), false);
                    break;
            }
        }
        success = processButtonsInput();
    }
    else if (setupType == SetupType::Axis) {
        while (!events.empty()) {
            SDL_Event event = events.front();
            events.pop();
            switch (event.type) {
                case SDL_CONTROLLERAXISMOTION:
                    gamepad->SetAxisValue((Axis) event.caxis.axis, event.caxis.value);
                    break;
            }
        }
        success = processAxesInput();
    }

    if (success) {
        emit settingsUpdated();
    }
}

bool Controller::processButtonsInput()
{
    gamepad->ProcessPendingKeyEvents();
    for (int i = 0; i < Gamepad::ButtonCount; i++) {
        if (gamepad->IsKeyPressed(i)) {

            for (size_t j = 0; j < buttonCommands.size(); j++) {
                if (buttonCommands[j].buttonMapping == SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i)) {
                    buttonCommands[j].buttonMapping = "";
                    emit commandMappingChanged(j, "", "button");
                }
            }

            buttonCommands[focusIndex].buttonMapping = SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i);
            emit gamepadKeyPressed(SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i));
            gamepad->ClearKeyState();
            return true;
        }
    }
    return false;
}

bool Controller::processAxesInput()
{
    for (int i = 0; i < Gamepad::AxisCount; i++) {
        if (abs(gamepad->GetValueForAxis((Axis)i)) == 1) {
            for (size_t j = 0; j < axisCommands.size(); j++) {
                if (axisCommands[j].buttonMapping == SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)i)) {
                    axisCommands[j].buttonMapping = "";
                    emit commandMappingChanged(j, "", "axis");
                }
            }

            axisCommands[focusIndex].buttonMapping = SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)i);
            emit gamepadAxisChanged(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)i));
            for (int j = 0; j < Gamepad::AxisCount; j++) {
                gamepad->SetAxisValue((Axis) j, 0);
            }
            return true;
        }
    }
    return false;
}

void Controller::saveSettings()
{
    SaveControlSettings(CONFIG_FILE_PATH);
}

int Controller::GetButtonCommandsCount() const
{
    return buttonCommands.size();
}

int Controller::GetAxisCommandsCount() const
{
    return axisCommands.size();
}

int Controller::GetFocusedElementIndex() const
{
    return focusIndex;
}

void Controller::SetFocusIndex(int focus)
{
    if (focus == -1) {
        focusIndex = focus;
    }
    else if (focus < buttonCommands.size()) {
        setupType = SetupType::Button;
        focusIndex = focus;
    }
    else {
        setupType = SetupType::Axis;
        focusIndex = focus % buttonCommands.size();
    }

    //emit focusChanged();
}

void Controller::OnJoystickConnected(int deviceIndex)
{
    if (!SDL_IsGameController(deviceIndex)) {
        return;
    }
    if (!gamepad->IsAtached()) {
        Connect(deviceIndex);
    }
}

bool Controller::Connect(int deviceIndex)
{
    bool opened = gamepad->Open(deviceIndex);
    if (opened) {
        emit joystickConnected();
    }
    return opened;
}

void Controller::OnJoystickDisconnected()
{
    if (!gamepad->IsAtached()) {
        gamepad->Close();
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (!SDL_IsGameController(i)) {
                continue;
            }
            if (Connect(i)) {
                return;
            }
        }
        emit joystickDisconnected();
    }
}

void Controller::LoadControlSettings(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл.";
        return;
    }

    QByteArray bytes = file.readAll();
    // Создание JSON-документа из JSON-данных
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    // Проверка на успешное чтение JSON-документа
    if (document.isNull()) {
       // qDebug() << "Не удалось прочитать JSON-документ.";
        return;
    }
    // Получение корневого JSON-объекта из JSON-документа
    QJsonObject objectJSON = document.object();

    file.close();

    const QJsonObject& buttonCommandsJSON = objectJSON["button_commands"].toObject();
    for (const auto& key : buttonCommandsJSON.keys()) {
        const QJsonObject& commandJSON = buttonCommandsJSON[key].toObject();
        QString title = commandJSON["title"].toString();
        QString binding = commandJSON["binding"].toString();
        buttonCommands.push_back({title, key, binding});
    }

    const auto& axesCommandsJSON = objectJSON["axis_commands"].toObject();
    for (const auto& key : axesCommandsJSON.keys()) {
        const auto& commandJSON = axesCommandsJSON[key].toObject();
        QString title = commandJSON["title"].toString();
        QString binding = commandJSON["binding"].toString();
        axisCommands.push_back({title, key, binding});
    }
}

void Controller::SaveControlSettings(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл.";
        return;
    }

    QByteArray bytes = file.readAll();
    // Создание JSON-документа из JSON-данных
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    // Проверка на успешное чтение JSON-документа
    if (document.isNull()) {
        qDebug() << "Не удалось прочитать JSON-документ.";
        return;
    }
    // Получение корневого JSON-объекта из JSON-документа
    QJsonObject objectJSON = document.object();

    QJsonObject buttonCommandsJSON = objectJSON["button_commands"].toObject();
    for (const auto& command : buttonCommands) {
        QJsonObject commandJSON = buttonCommandsJSON[command.commandName].toObject();
        commandJSON["binding"] = command.buttonMapping;
        buttonCommandsJSON[command.commandName] = commandJSON;
    }
    objectJSON["button_commands"] = buttonCommandsJSON;

    QJsonObject axesCommandsJSON = objectJSON["axis_commands"].toObject();
    for (const auto& command : axisCommands) {
        QJsonObject commandJSON = axesCommandsJSON[command.commandName].toObject();
        commandJSON["binding"] = command.buttonMapping;
        axesCommandsJSON[command.commandName] = commandJSON;
    }
    objectJSON["axis_commands"] = axesCommandsJSON;

    document.setObject(objectJSON);

    file.seek(0);
    file.write(document.toJson());
    file.resize(file.pos());
    file.close();
}
