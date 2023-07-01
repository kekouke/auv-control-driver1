#pragma once

#include "common.h"
#include "core.h"
#include "message.h"

namespace ipc {

#pragma pack(push,1)

/**
 * @brief Класс для загрузки параметров
 */
template <typename T>
class Loader : public Message
{
private:
    Core &core;
    Loader(const Loader<T> &l):core(l.core) {}
    void operator =(const Loader<T>&) {}
public:
    T _;
    /**
     * @brief Конструктор
     * @param core            Ядро
     * @param register_type   Тип регистрации
     */
    Loader(Core &core, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
    {
        schema(_.schema());
        reload();
        core.register_load(*this, register_type);
    }

    void    storage_write()
    {core.storage_write(*this, ipc::StorageTypeLoad);}

    void    reload() {
        load(core.load_path());
        load(core.load_paths_addition());
        load((ipc::Stream() << core.load_path()<<"/"<<core.prog_name()).str());
    }

};

#pragma pack(pop)
}
