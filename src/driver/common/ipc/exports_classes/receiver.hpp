#pragma once

#include "common.h"
#include "core.h"
#include "host_node_name.h"
#include "message.h"

namespace ipc {

#pragma pack(push,1)

/**
 * @brief Класс получения сообщений по событию
 */
template <typename T>
class Receiver : public Message
{
private:
    Core &core;
    ipc::HostNodeName _target_host_node;
    Receiver(const Receiver<T> &r):core(r.core) {}
    void operator =(const Receiver<T>&) {}

    /**
     * @brief Инициализация
     */
    void init(int queue_len, RegisterType register_type)
    {
        schema(_.schema());
        load(core.load_path());
        load(core.load_paths_addition());
        load((ipc::Stream() << core.load_path()<<"/"<<core.prog_name()).str());
        core.register_receive(*this, queue_len, register_type);
    }
public:
    T _;
    /**
     * @brief Конструктор
     * @param core            Ядро
     * @param target_HostNode список модулей от кого принять сообщение
     * @param queue_len       Размер очереди
     * @param register_type   Тип регистрации
     */
    Receiver(Core &core, ipc::HostNodeName target_HostNode, int queue_len = 50, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
        , _target_host_node(target_HostNode)
    {
        init(queue_len, register_type);
    }
    /**
     * @brief Конструктор для любых хостов
     * @param core            Ядро
     * @param queue_len       Размер очереди
     * @param target_HostNode список модулей от кого принять сообщение
     * @param register_type   Тип регистрации
     */
    Receiver(Core &core, int queue_len, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
        , _target_host_node(HostNodeName::All_Hosts_Nodes)
    {
        init(queue_len, register_type);
    }
    /**
     * @brief Конструктор для любых хостов и очередью по-умолчанию
     * @param core            Ядро
     * @param queue_len       Размер очереди
     * @param target_HostNode список модулей от кого принять сообщение
     * @param register_type   Тип регистрации
     */
    Receiver(Core &core, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
        , _target_host_node(HostNodeName::All_Hosts_Nodes)
    {
        init(50, register_type);
    }

    /**
     * @brief Проверить получено ли сообщение
     * @return true если получено сообщение
     */
    bool received()
    {return core.received(*this, _target_host_node);}

    /**
     * @brief Проверить получено ли сообщение
     * @param target_HostNode список модулей от кого принять сообщение
     * @return true если получено сообщение
     */
    bool received(ipc::HostNodeName target_HostNode)
    {return core.received(*this, target_HostNode);}

    void    storage_write()
    {core.storage_write(*this, ipc::StorageTypeReceive);}
};

#pragma pack(pop)
}
