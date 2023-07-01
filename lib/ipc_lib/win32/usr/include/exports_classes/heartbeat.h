#pragma once

#include "common.h"
#include "storage_path.h"

namespace ipc {

class CLASS_EXPORT Heartbeat
{
public:
    struct hash_size_t
    {
        uint64_t   hash;
        size_t     size;
    };

public:
    Heartbeat();
    Heartbeat(const Heartbeat &h);
    Heartbeat& operator =(const Heartbeat& h);
    ~Heartbeat();

    friend class   Core;

    std::string get_program_name();
    std::string get_program_hostname();

    std::vector<hash_size_t> get_use_message_list();

    /**
     * @brief Получить ХЭШ от JSON-схемы зарегистрированных сообщений в модуле
     * @return ХЭШ от схемы
     */
    uint64_t get_hash_schema();
    uint64_t get_program_pid();
    /**
     * @brief Получить номер TCP порта открытого модулем для WEB-socket
     * @return Номер порта
     */
    uint16_t get_port();
    /**
     * @brief Получить номер ядра в модуле
     * @return Номер ядра
     */
    uint8_t  get_ipc_id();
    /**
     * @brief Получить сырые статичные данные в формате JSON
     * @return JSON-строка
     */
    std::string get_raw_static_data();
    /**
     * @brief Получить сырые динамические данные в формате JSON
     * @return JSON-строка
     */
    std::string get_raw_dynamic_data();
    /**
     * @brief Получить IP адрес отправителя
     * @return IP адрес
     */
    std::string get_ip();
    /**
     * @brief Получить тип работы накопления
     * @return Тип накопления
     */
    ipc::Storage_path::Type get_storage_type();
    /**
     * @brief Получить директорию в которую идет накопление
     * @return Директория накопления
     */
    std::string get_storage_path();
    /// Получить время местное время отпралвения сообщения
    ipc::DateTime   get_pub_time();

private:
    ipc_basic_heartbeat_receive_t *basic;
};

}
