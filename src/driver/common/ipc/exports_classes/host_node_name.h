#pragma once
#include "common.h"

namespace ipc {

/**
 * @brief Класс для описания пары имени хоста и имени программы
 */
class CLASS_EXPORT HostNodeName {

public:

    /**
     * @brief Класс для описания имени параметра
     *
     * {hostname}@{nodename}#{message::name}/{parameters/parameter} - путь до параметра
     * {hostname}@{nodename}#{message::name}* - Все параметры этого сообщения
     *
     * {hostname}@{nodename}#{message::name}~age - Служебные команды (Возраст сообщения)
     */
    class CLASS_EXPORT URL {
    private:
        std::string _host_name;             ///< Имя хоста
        std::string _node_name;             ///< Имя модуля
        std::string _message_name;          ///< Имя сообщения
        std::string _parameter_name;        ///< Имя параметра
        bool        _age;                   ///< Флаг команды (age)
    public:
        URL()
            : _host_name("*")
            , _node_name("*")
            , _message_name("")
            , _parameter_name("")
            , _age(false)
        {}

        URL(const std::string& str) {parce(str);}

        /// Установить значения
        URL& set(const std::string& str) {parce(str); return *this;}
        /// Установить имя хоста
        URL& hostname(const std::string& name) {_host_name = name; return *this;}
        /// Установить имя модуля
        URL& nodename(const std::string& name) {_node_name = name; return *this;}
        /// Установить имя сообщения
        URL& message_name(const std::string& name) {_message_name = name; return *this;}
        /// Установить имя параметра
        URL& parameter_name(const std::string& name) {set_paramerter_name(name); return *this;}
        /// Установить имя параметра
        URL& age(bool b) {_age = b; _parameter_name = "~age"; return *this;}
        /// Получить имя хоста
        std::string hostname() const {return _host_name;}
        /// Получить имя модуля
        std::string nodename() const {return _node_name;}
        /// Получить имя сообщения
        std::string message_name() const {return _message_name;}
        /// Получить имя параметра
        std::string parameter_name() const {return _parameter_name;}
        /// Получить флаг Age
        bool    age() const {return _age;}
        /**
         * @brief Проверить подходит ли переданый путь, местному
         * @return  true - если хост, нод, мессаже и парамет подходят друг другу
         */
        bool    fits(const URL& url);
        /**
         * @brief Разрешены все хосты и модули
         * @return true - да
         */
        bool    is_all_host_node();
        /// Составить полный путь
        std::string full_path() const;
    private:
        /**
         * @brief Распарсить строку
         * @param str строка тип {hostname}@{nodename}#{message::name}/{parameters/parameter}
         */
        void    parce(const std::string& str);
        /**
         * @brief Разделить строку по разделителю на 2 подстроки
         * @param str   Строка
         * @param sep   Разделитель
         * @return Массив строк
         */
        std::vector<std::string> split_in_two_str(const std::string &str, char sep);
        bool    fits(std::string mask, std::string str);
        /**
         * @brief Установить имя параметра
         * @param name Имя параметра
         */
        void    set_paramerter_name(const std::string& name);
    };

public:
    struct item_t {
        std::string hostname;
        std::string nodename;
    };

    enum mode_t {
        All_Hosts_Nodes,        ///< Без фильтра. Все хосты и все модули
        Only_local_host,        ///< Только модули с локального хоста
        Only_allow,             ///< Только разрешенные модули
        Only_me                 ///< Только самому себе
    };

    HostNodeName(mode_t mode = All_Hosts_Nodes);
    /**
     * @brief HostNodeName Конструктор
     * @param host_node строка с именем хоста и нода (пример "hostname@nodename")
     */
    HostNodeName(std::string host_node);
    HostNodeName& add(std::string host_node);
    HostNodeName& add(std::string hostname, std::string nodename);

    std::list<item_t>   list() const;
    mode_t              mode() const;

    bool                is_empty() const;
private:
        std::list<item_t>   _list;
        mode_t              _mode;
};

}
