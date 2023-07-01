#pragma once                  // Только один раз подключаем заголовочник
#include "ipc.h"
#pragma pack(push,1)          // Выставляем однобайтовое выравнивание!!!

namespace Message {
    struct ButtonState {
        ipc::String<15> name;
        bool isPressed;
        ipc::Schema schema() {
            return ipc::Schema(this).title("Состояние кнопки")
               .add(IPC_STRING(name)
                    .title("Имя кнопки")
                    .default_("Unknown"))
               .add(IPC_BOOL(isPressed).title("Состояние: ")
                   .false_(ipc::Off, "Отпущена")
                   .true_(ipc::On, "Нажата")
                   .default_(false))
               ;
        }
    };

    struct AxisState {
        ipc::String<15> name;
        double value;
        ipc::Schema schema() {
            return ipc::Schema(this).title("Состояние оси")
               .add(IPC_STRING(name).title("Наименование оси").default_("Unknown"))
               .add(IPC_REAL(value).title("Значение").default_(0.0))
               ;
        }
    };

    struct GamepadState {
        ButtonState buttonStates[21];
        AxisState axesState[4];
        ipc::Schema schema() {
            return ipc::Schema(this).title("Состояние геймпада")
               .add(IPC_STRUCTS(buttonStates).title("Состояние кнопок")
                    .element_title("Состояние кнопки"))
               .add(IPC_STRUCTS(axesState).title("Состояние осей")
                    .element_title("Состояние оси"))
               ;
        }
    };

    struct Init {
        double  state_timer;
        double  read_timer;
        double  send_timer;

        ipc::Schema schema() {
            return ipc::Schema(this).title("Настройки")
                .add(IPC_INT(state_timer).title("Период состояния")
                     .unit("c").default_(1.0))
                .add(IPC_INT(read_timer).title("Период чтения данных")
                     .unit("c").default_(1.0))
                .add(IPC_INT(send_timer).title("Период посылки данных")
                     .unit("c").default_(0.5));
        }
    };

    struct CommandBinding {
        ipc::String<80> title;
        ipc::String<15> binding;

        ipc::Schema schema() {
            return ipc::Schema(this).title("Команда")
                .add(IPC_STRING(title).title("Описание"))
                .add(IPC_STRING(binding).title("Привязка"));
        }
    };

    struct ButtonBindings {
        CommandBinding start_control;
        CommandBinding stop_control;
        CommandBinding set_min_speed;
        CommandBinding set_slow_speed;
        CommandBinding set_max_speed;
        CommandBinding set_zero_speed;

        ipc::Schema schema() {
            return ipc::Schema(this).title("Команды кнопок")
                .add(IPC_STRUCT(start_control))
                .add(IPC_STRUCT(stop_control))
                .add(IPC_STRUCT(set_min_speed))
                .add(IPC_STRUCT(set_slow_speed))
                .add(IPC_STRUCT(set_max_speed))
                .add(IPC_STRUCT(set_zero_speed))
                ;
        }
    };

    struct AxisBindings {
        CommandBinding move_forward;
        CommandBinding move_right;

        ipc::Schema schema() {
            return ipc::Schema(this).title("Команды осей")
                .add(IPC_STRUCT(move_forward))
                .add(IPC_STRUCT(move_right))
                ;
        }
    };

    struct GamepadBindings {
        AxisBindings axis_commands;
        ButtonBindings button_commands;

        ipc::Schema schema() {
            return ipc::Schema(this).title("")
                .add(IPC_STRUCT(axis_commands).title("Команды осей"))
                .add(IPC_STRUCT(button_commands).title("Команды кнопок"))
                ;
        }
    };

    // Состояние программы //
    struct State {
        bool send_regime;
        Init settings;
        GamepadBindings bindings;
        ipc::Schema schema() {
            return ipc::Schema(this).title("Состояние")
                .add(IPC_BOOL(send_regime).title("Режим работы")
                    .false_(ipc::Ok, "Получатель").true_(ipc::On, "Отправитель"))
                .add(IPC_STRUCT(settings)
                    .title("Настройки"))
                .add(IPC_STRUCT(bindings)
                    .title("Текущие настройки управления"))
                ;
        }
    };
}
#pragma pack(pop)
