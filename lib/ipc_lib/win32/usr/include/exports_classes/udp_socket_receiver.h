#pragma once
#include "common.h"

namespace ipc {

class Core;
/// Класс для получения UDP датаграм средствами библиотеки
class CLASS_EXPORT UdpSocketReceiver {
public:
    /// Структура с принятой датаграммой
    struct UdpDatagram {
        std::vector<char>   data;           ///< Данные датаграммы
        std::string         source_ip;      ///< IP адрес отправителя
    };

    /// Структура с описанием мультикаст группы
    struct Multicast {
        std::string multicast_group;        ///< Мультикаст группа
        std::string multicast_interface;    ///< Интерфейс для подключения к мультикасту
        Multicast(std::string g, std::string i)
            : multicast_group(g)
            , multicast_interface(i)
        {}
    };

private:
    Core &core;                             ///< ссылка на ядро
    uint16_t    _port;                      ///< UDP порт
    UdpDatagram _last_datagram;             ///< Последняя полученная датаграмма
public:
    /**
     * @brief Конструктор без подключения к мультикаст группе
     * @param core          Ссылка на ядро системы обмена
     * @param port          UDP порт
     * @param queue_len     Размер максимальной очереди
     */
    UdpSocketReceiver(Core &core, uint16_t port, int queue_len = 50);
    /**
     * @brief Конструктор с подключением к мультикаст группе
     * @param core                  Ссылка на ядро системы обмена
     * @param port                  UDP порт
     * @param multicast_group       Мультикаст группа
     * @param multicast_interfes    Интерфейс для подключения к мультикасту
     * @param queue_len             Размер максимальной очереди
     */
    UdpSocketReceiver(Core &core, uint16_t port
                      , std::string multicast_group, std::string multicast_interfes = "0.0.0.0"
                      , int queue_len = 50);

    /**
     * @brief Проверить получение UDP датаграмы
     * @return true - если датаграма получена
     */
    bool    received();

    /**
     * @brief Получить последнюю полученную датаграмму
     * @return ссылка на датаграмму
     */
    UdpDatagram& datagram();
};

}
