#pragma once

#include "common.h"
#include "core.h"
#include "message.h"

namespace ipc {

#pragma pack(push,1)

/**
 * @brief Класс для получения текущих значений
 */
template <typename T>
class Reader : public Message
{
private:
    Core &core;
    ipc::HostNodeName _target_nodes;
    Reader(const Reader<T> &r):core(r.core) {}
    void operator =(const Reader<T>&) {}
    void init () {
        schema(_.schema());
        load(core.load_path());
        load(core.load_paths_addition());
        load((ipc::Stream() << core.load_path()<<"/"<<core.prog_name()).str());
    }
public:
    T _;
    /**
     * @brief Конструктор
     * @param core            Ядро
     * @param register_type   Тип регистрации
     */
    Reader(Core &core, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
    {
        init();
        core.register_read(*this, register_type);
    }
    /**
     * @brief Конструктор класса со списком поставщиков
     * @param core          Ядро
     * @param target_nodes  Список поставщиков
     * @param register_type Тип регистрации
     */
    Reader(Core &core, ipc::HostNodeName target_nodes, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
        , _target_nodes(target_nodes)
    {
        init();
        core.register_read(*this, _target_nodes, register_type);
    }
    /**
     * @brief Получить текущее значение
     * @return ссылка на данные
     */
    T& read()
    {
        if(_target_nodes.is_empty())
            core.read(*this);
        else
            core.read(*this, _target_nodes);
        return _;
    }

    void    storage_write()
    {core.storage_write(*this, ipc::StorageTypeRead);}
};

#pragma pack(pop)
}
