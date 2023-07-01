#pragma once

#include "common.h"
#include "message.h"
#include "core.h"
#include "host_node_name.h"

namespace ipc {

#pragma pack(push,1)

/// Опции при регистрации сообщения на публикацию
class SenderOptions {
public:
    enum SendType {
        Simple,         ///< Обычная публикация
        Repeat          ///< Публикация с дублированием
    };

    ipc::HostNodeName   _target_HostNode;   ///< Список модулей для доставки
    SendType            _repeat;            ///< Использовать излишнее дублирование сообщений
    RegisterType        _register_type;     ///< Тип регистрации
    std::string         _log_suffix;        ///< Суффикс для файлов логирования
    std::string         _redefinition_node; ///< Переопределить имя модуля поставщика (Подменить свое имя), пустая строка - ничего не делать
    std::string         _redefinition_host; ///< Переопределить имя хоста поставщика (Подменить свое имя), пустая строка - ничего не делать

    bool                _store;             ///< Вести накопление данных
    bool                _send_net;          ///< Отправлять данные по сети (UDP)
    bool                _send_web;          ///< Отправлять данные в WEB-сокет

    SenderOptions()
        : _target_HostNode(HostNodeName::All_Hosts_Nodes)
        , _repeat(Simple)
        , _register_type(RegisterEnable)
        , _log_suffix("")
        , _redefinition_node("")
        , _redefinition_host("")
        , _store(true)
        , _send_net(true)
        , _send_web(true)
    {}

    SenderOptions& target_HostNode  (const ipc::HostNodeName t)   {_target_HostNode = t;   return *this;}
    SenderOptions& send_type        (const SendType t)            {_repeat = t;            return *this;}
    SenderOptions& register_type    (const RegisterType t)        {_register_type = t;     return *this;}
    SenderOptions& log_suffix       (const std::string s)         {_log_suffix = s;        return *this;}
    SenderOptions& redefinition_node(const std::string s)         {_redefinition_node = s; return *this;}
    SenderOptions& redefinition_host(const std::string s)         {_redefinition_host = s; return *this;}

    SenderOptions& no_store()       {_store    = false; return  *this;}
    SenderOptions& no_send_net()    {_send_net = false; return  *this;}
    SenderOptions& no_send_web()    {_send_web = false; return  *this;}
};

/// Класс для отправки сообщения
template <typename T>
class Sender : public Message
{
public:

private:
    Core &core;                                 ///< Ссылка на ядоро обмена
    Sender(const Sender<T> &s):core(s.core) {}  ///< Запретить конструктор копирования
    void operator =(const Sender<T>&) {}        ///< Запретить присвоение

    SenderOptions _options;                     ///< Опции публикации

    /// Инициализация
    void    init() {
        schema(_.schema());
        load(core.load_path());
        load(core.load_paths_addition());
        load((ipc::Stream() << core.load_path()<<"/"<<core.prog_name()).str());
        core.register_send(*this, _options);
    }
public:
    T _;
    /**
     * @brief Конструктор
     * @param core            Ядро
     * @param target_HostNode список модулей для доставки
     * @param repeat          Использовать излишнее дублирование сообщений
     * @param register_type   Тип регистрации
     */
    Sender(Core &core, ipc::HostNodeName target_HostNode, SenderOptions::SendType repeat = SenderOptions::Simple, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
    {
        _options._target_HostNode = target_HostNode;
        _options._repeat          = repeat;
        _options._register_type   = register_type;
        init();
    }
    /**
     * @brief Конструктор для публикции на все хосты
     * @param core           Ядро
     * @param repeat         Использовать излишнее дублирование сообщений
     * @param register_type  Тип регистрации
     */
    Sender(Core &core, SenderOptions::SendType repeat, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
    {
        _options._target_HostNode = HostNodeName::All_Hosts_Nodes;
        _options._repeat          = repeat;
        _options._register_type   = register_type;
        init();
    }
    /**
     * @brief Конструктор для публикации на все хосты и без излишнего дублирования при пуликации
     * @param core           Ядро
     * @param repeat         Использовать излишнее дублирование сообщений
     * @param register_type  Тип регистрации
     */
    Sender(Core &core, RegisterType register_type = RegisterEnable)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
    {
        _options._target_HostNode = HostNodeName::All_Hosts_Nodes;
        _options._repeat          = SenderOptions::Simple;
        _options._register_type   = register_type;
        init();
    }

    /**
     * @brief Конструктор с указанием опций
     * @param core          Ядро
     * @param options       Опции
     */
    Sender(Core &core, SenderOptions options)
        : Message(typeid_name_to_str(typeid(_).name()), &_, sizeof(_))
        , core(core)
        , _options(options)
    {
        init();
    }

    /// Отправить сообщение
    void send()
    {
        core.send(*this, _options);
    }
    /**
     * @brief Отправить сообщение на заданные модули
     * @param target_HostNode   Модули для отправки
     */
    void send(const ipc::HostNodeName target_HostNode)
    {
        SenderOptions op = _options;
        op.target_HostNode(target_HostNode);
        core.send(*this, op);
    }
    /**
     * @brief Присвоить данные и отравить сообщение
     * @param value данные
     */
    void send(T &value){_ = value; send();}
};
#pragma pack(pop)

}
