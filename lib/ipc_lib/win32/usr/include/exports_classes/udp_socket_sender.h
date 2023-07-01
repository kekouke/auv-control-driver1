#pragma once
#include "common.h"
#include "core.h"

namespace ipc {

/**
 * @brief Класс для отправки UDP датаграм средствами библиотеки
 */
class CLASS_EXPORT UdpSocketSender {
private:
    Core &core;
    ipc_udp_socket_t* _socket;
    UdpSocketSender(const UdpSocketSender &u): core(u.core)  {}
    UdpSocketSender operator =(const UdpSocketSender &) {return *this;}
public:
    UdpSocketSender(Core &core);
    ~UdpSocketSender();

    /**
     * @brief Выставить опции сокета для broadcast
     */
    void    setsockopt_broadcast();

    /**
     * @brief Отправить датаграмму
     * @param data данные
     * @param len длина данных
     * @param host адрес отправки
     * @param port порт отправки
     * @return количество отправленных байт
     */
    int64_t sendto(const char *data, int64_t len, const std::string host, uint16_t port);
};

}
