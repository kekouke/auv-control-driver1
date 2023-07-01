#pragma once

#include "common.h"
#include "schema.h"
#include "parameter.h"
#include "datetime.h"

namespace ipc {

/**
 * @brief Базовый класс IPC сообщения
 */
class CLASS_EXPORT Message {
protected:
    ipc_basic_message_t *basic;
    friend class Core;
    Message();

public:
    /**
     * @brief Конструктор
     * @param message_name          имя сообщения
     * @param message_data_pointer  указатель на данные сообщения
     * @param message_data_size     размер данных в сообщении
     * @param use_net_filter        использовать сетевой фильтр сообщений
     */
    Message(std::string message_name,
            void * message_data_pointer,
            size_t message_data_size,
            bool  use_net_filter = true);

    /**
     * @brief Конструктор
     * @param message_name          имя сообщения
     * @param message_data_size     размер данных в сообщении
     * @param use_net_filter        использовать сетевой фильтр сообщений
     */
    Message(std::string message_name,             // имя сообщения
            size_t message_data_size,             // размер данных в сообщении
            bool  use_net_filter = true);         // использовать сетевой фильтр сообщений


    /// контруктор для копирования сообщений
    Message(const Message &message);

    ~Message();

    Message& operator=(Message &message);
    /**
     * @brief Получить указатель на данные
     * @return указатель
     */
    void *data_pointer();

    /**
     * @brief Получить имя сообщения
     * @return
     */
    std::string     name() const;
    /**
     * @brief Русское имя сообщения
     * @return
     */
    std::string     title() const;
    /**
     * @brief Получить возраст собщения
     * @return
     */
    double          age();
    /**
     * @brief Получить реальный возраст собщения
     * @return
     */
    double          age_real();
    /**
     * @brief Устарели ли данные (true age > max_age)
     */
    bool            old();
    /**
     * @brief Получить флаг отсутствия возраста
     */
    bool            no_age();
    /**
     * @brief Получить время приема сообщения
     * @return
     */
    DateTime          time();
    /**
     * @brief Получить размер данных сообщения
     * @return
     */
    size_t          size();
    /**
     * @brief Получить имя программы поставшее сообщение
     * @return
     */
    std::string     progname();
    /**
     * @brief Получить имя хоста программы поставшее сообщение
     * @return
     */
    std::string     hostname();
    /**
     * @brief IP адрес отправителя
     * @return ip адрес
     */
    std::string     ip_adress();
    /**
     * @brief Получить идентификатор сообщения
     * @return
     */
    uint64_t hash();
    /**
     * @brief Получить смещение порта для сообщения
     * @return Номер порта
     */
    uint16_t offset_port();

    /**
     * @brief установить значиния по JSON строке
     * @param строка в формате json
     */
    void            value_from_json(std::string json_string);
    /**
     * @brief получить значения параметров в формате JSON
     * @param enum_to_str   false - JSON без изменений, true - заменить все enum на строковые алиасы
     * @return              Строка в формате JSON
     */
    std::string     value_in_json(bool enum_to_str = false);
    /**
     * @brief получить значения параметров в формате JSON-объекта
     * @param enum_to_str   false - JSON без изменений, true - заменить все enum на строковые алиасы
     * @return              JSON объект
     */
    json_t::object_t value_in_json_object(bool enum_to_str = false);

    /**
     * @brief Загрузить значение из файла
     * @param file_name Имя файла
     * @return true при удачной загрузке значений
     */
    bool            load_from_file(std::string file_name);
    /**
     * @brief Загрузить значение из файла (имя файла формируется от имени сообщения)
     * @param path      Путь к директории где находится файл
     * @return          true при удачной загрузке значений
     */
    bool            load(std::string path);

    /**
     * @brief Загрузить значение из файла (имя файла формируется от имени сообщения)
     * @param paths список дирректорий с файлами
     */
    void            load(std::vector<std::string> paths);

    /**
     * @brief Загрузить значения по умолчанию
     */
    void            load_from_default();
    /**
     * @brief загружены ли значения по умолчанию
     * @return true при успешно загруженых значений
     */
    bool            loaded();

    /**
     * @brief Загрузка описания сообщения
     * @param message_schema Json-схема
     */
    void            schema(Schema message_schema);
    /**
     * @brief Загрузка описания сообщения
     * @param message_schema строка в формате Json-схема
     */
    void            schema(std::string message_schema);
    /**
     * @brief Загрузка описания сообщения
     */
    void            schema(json_t::object_t obj);
    /**
     * @brief Получить описание сообщения
     * @return строка в фортмате json-схемы
     */
    std::string     schema();
    /**
     * @brief Получить описание сообщения
     * @return Json объект
     */
    json_t::object_t schema_obj() const;
    /**
     * @brief Получить максимальный возраст сообщения
     * @return максимальный возраст сообщения в секундах
     */
    double          max_age();
    /**
     * @brief Получить тип сообщения
     * @return Тип сообщения
     */
    MessageType     type();
    /**
     * @brief Номер отправленного сообщения (каждый модуль начинает с 1)
     * @return Номер отправки
     */
    uint64_t        counter_send();
    /// Получить список всех описанных параметров
    std::vector<ipc::Parameter> parameters();

    /// Проверка откуда отправлено сообщение (true - из WEB-Viewer)
    bool    is_webviewer();
};

}
