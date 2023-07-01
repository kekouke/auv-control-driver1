#pragma once

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QTimer>

#include <vector>
#include <queue>

#include "gamepad.h"
#include "command.h"

enum class SetupType {
    Button,
    Axis
};

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int buttonCommandsCount READ GetButtonCommandsCount)
    Q_PROPERTY(int axisCommandsCount READ GetAxisCommandsCount)
    Q_PROPERTY(int focusedElementIndex READ GetFocusedElementIndex WRITE SetFocusIndex)
public:
    const QString CONFIG_FILE_PATH = "load/Message--GamepadBindings.json";
    Controller(QObject* parent = nullptr);

    int GetButtonCommandsCount() const;

    int GetAxisCommandsCount() const;

    int GetFocusedElementIndex() const;

    void SetFocusIndex(int focus);

    bool processButtonsInput();
    bool processAxesInput();

    ~Controller();

public slots:
    void setup();
    void saveSettings();

    QString getButtonCommandName(int index) {
        return buttonCommands[index].title;
    }
    QString getAxisCommandName(int index) {
        return axisCommands[index].title;
    }
    QString getButtonName(int index) {
        return buttonCommands[index].buttonMapping;
    }
    QString getAxisName(int index) {
        return axisCommands[index].buttonMapping;
    }

    void clearInput() {
        gamepad->ClearKeyState();
        while (!events.empty()) {
            events.pop();
        }
    }

signals:
    void joystickConnected();
    void joystickDisconnected();
    void gamepadKeyPressed(const QString& buttonName);
    void gamepadAxisChanged(const QString& axisName);
    void commandMappingChanged(int index, const QString& buttonName, const QString& type);
    void settingsUpdated();

private slots:
    void slotTimerAlarm() {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_CONTROLLERDEVICEADDED:
                    OnJoystickConnected(event.cdevice.which);
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    OnJoystickDisconnected();
                    break;
                case SDL_CONTROLLERAXISMOTION:
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    if (focusIndex != -1) {
                        events.push(event);
                    }
                    break;
            }
        }
        if (events.size() > 0) {
            setup();
        }
    }

private:
    int focusIndex = -1;
    SetupType setupType = SetupType::Button;
    Gamepad* gamepad;
    QTimer* connectOrDisconnectJoystickTimer;
    SDL_Event event;
    std::vector<Command> buttonCommands;
    std::vector<Command> axisCommands;
    std::queue<SDL_Event> events;

    void OnJoystickConnected(int deviceIndex);
    void OnJoystickDisconnected();
    bool Connect(int deviceIndex);

    void LoadControlSettings(const QString& filename);

    void SaveControlSettings(const QString& filename);
};
