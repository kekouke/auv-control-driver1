#pragma once

#define IPC_VERSION "11.0.061"
#define IPC_VER      11000061

#include <typeinfo>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <stdint.h>
#include <sstream>

#ifdef IPC_STATIC
#   define CLASS_EXPORT
#   define FUNC_CPP_EXPORT
#else
#   ifdef WIN32
#       if defined(IPC_LIBRARY)                                    // сборка библиотеки
#           define CLASS_EXPORT        __declspec(dllexport)
#           define FUNC_CPP_EXPORT     __declspec (dllexport)
#       else
#           if defined(USE_IPC)                                          // использовать библиотеку
#               define CLASS_EXPORT      __declspec(dllimport)
#               define FUNC_CPP_EXPORT   __declspec(dllimport)
#           else                                                         // статично
#               define CLASS_EXPORT
#               define FUNC_CPP_EXPORT
#           endif
#       endif
#   else                                                           // для QNX
#       define CLASS_EXPORT
#       define FUNC_CPP_EXPORT
#   endif
#endif

class ipc_basic_t;
class ipc_basic_message_t;
class ipc_basic_timer_t;
class ipc_basic_blackbox_t;
class ipc_basic_heartbeat_receive_t;
class ipc_udp_socket_t;

namespace json_t {
    class object_t;
    class array_t;
    class basic_t;
}


/**
 * namespace ipc
 * @brief Интрефейс взаимодейтсвия процессов
 */
namespace ipc {

class Message;

/**
 * @brief Состояние
 */
enum State
{
    On,         ///< Включено
    Off,        ///< Выключено
    Ok,         ///< В норме
    Warning,    ///< Предупреждение
    Error,      ///< Ошибка
    Debug       ///< Отладка
};

/**
 * @brief Тип сообщения
 */
enum MessageType {
    Filtered,       ///< Использовать фильт при публикации в сеть
    NotFiltered     ///< Не использовать фильт при публикации в сеть
};

/**
 * @brief Тип записи в накопитель
 */
enum StorageWriteType {
    StorageTypeSend,        ///< Как публикуемое сообщение
    StorageTypeRead,        ///< Как получаемое сообщение
    StorageTypeReceive,     ///< Как получаемое событие
    StorageTypeLoad         ///< Как настроки
};

/**
 * @brief Тип регистрации сообщений
 */
enum RegisterType {
    RegisterEnable,         ///< Обычная регистрация
    RegisterExpert,         ///< Регистрация для режима Эксперт во вьювере.
    RegisterDisable,        ///< Без регистрации
    RegisterUnvisible       ///< Регистрация без создания схемы
};

/**
 * @brief Получение имени сообщения
 * @param name строка получения функцией typeid
 * @return форматированая строка
 */
FUNC_CPP_EXPORT std::string typeid_name_to_str(const char* name);


/**
 * @brief Преобразование состояние в строку
 * @param state состояние
 * @return текст
 */
FUNC_CPP_EXPORT std::string state_to_str(const State state);
/**
 * @brief Преобразование строки в состояние
 * @param state тест
 * @return состояние
 */
FUNC_CPP_EXPORT State       str_to_state(std::string state);
/**
 * @brief Преобразование типа сообщения в строку
 * @param type Тип
 * @return текст
 */
FUNC_CPP_EXPORT std::string message_type_to_str(const MessageType type);
/**
 * @brief Преобразование строки в тип сообщения
 * @param type Тип
 * @return текст
 */
FUNC_CPP_EXPORT MessageType str_to_message_type(std::string type);

FUNC_CPP_EXPORT uint64_t calculate_hash(const std::string text);

FUNC_CPP_EXPORT uint32_t calculate_hash_32(const std::string text);

/**
 * @brief Создать путь дирректорий с правами доступа 0777
 * @param path - путь создаваемых дирректорий
 */
FUNC_CPP_EXPORT void mk_path(std::string path);

/**
 * @brief Структура с динамически созданным сообщением из загруженых схем
 */
struct Message_and_reg_type
{
    Message* message;       ///< Указатель на сообщение. После использования необходимо вызвать delete
    bool    read;           ///< Сообщение было зарегистрировано как Read
    bool    receive;        ///< Сообщение было зарегистрировано как Receive
    bool    send;           ///< Сообщение было зарегистрировано как Send
    bool    load;           ///< Сообщение было зарегистрировано как Load

    std::string node_name;  ///< Имя модуля в котором оно зарегистрировано
    std::string node_title; ///< Имя хоста в котором оно зарегистрировано
};

/**
 * @brief Создать набор сообщений из описаных схем в строке
 * @param str JSON-схема
 * @return Список сообщений
 */
FUNC_CPP_EXPORT std::vector<Message_and_reg_type> load_from_str(std::string str);

}
