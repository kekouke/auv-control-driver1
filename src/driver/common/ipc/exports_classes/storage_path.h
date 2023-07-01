#pragma once

#include "common.h"
#include "core.h"

namespace ipc {

/// Класс событие при изменении директории для накопления
class CLASS_EXPORT Storage_path
{
public:
    Storage_path(Core& core)
        : core(core)
    {core.register_storage_path();}

    enum Type {
        NoStorage,      ///< Нет накопления
        NoMisson,       ///< Накопление без миссии
        Mission         ///< Накопление в миссии
    };

    struct Data {
        std::string             mission_path;       ///< Путь для накопления общий (имя миссии)
        std::string             path;               ///< Путь для накопления для модуля (имя миссии/хост/модуль)
        Type                    type;               ///< Тип работы накопителя
        ipc::DateTimeRealTime   start_time;         ///< Время запуска накопления
    };

    /// Проверить находится ли в очереди это событие
    bool received()
    {return core.received(*this);}

    Data _;

private:
    Core& core;
};

}
