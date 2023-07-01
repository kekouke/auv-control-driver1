#pragma once

#include "common.h"
#include "out.h"
#include "stream.h"
#include "udp_socket_receiver.h"
#include "host_node_name.h"
#include "datetime.h"

namespace ipc {

class Message;
class Timer;
class Serial;
class Blackbox;
class Heartbeat;
class Storage_path;
class SenderOptions;

/**
 * @brief Ядро системы взаимодействия процессов
 */
class CLASS_EXPORT Core {
private:
    Core(const Core &);
    void operator =(const Core&) {}
protected:
    ipc_basic_t* basic;
public:
    /// Данные о программе
    struct Description {
        std::string _title;          ///< Заголовок прогаммы (title для JSON-схемы и для WebViewer)
        std::string _version;        ///< Версия программы (выводится по ключу --ipc_version и --ipc_help, публикуется в heartbeat )
        std::string _description;    ///< Описание программы
        std::string _help;           ///< Хелп по использованию (выводится по ключу --ipc_help)
        std::string _key_help;       ///< Ключ argv по которуму нужно выводить хелп (пустая строка - хелп не будет выводится)
        std::string _conf_file;      ///< Путь до файла с настройками ipc (пустая строка - пути по умолчанию)

        Description() {}
        /// Конструктор с Названием програмы
        Description(std::string title) {_title = title;}
    };

    /**
     * @brief регистрация на чтение (look)
     * @param message       Регистрируемое сообщение
     * @param register_type Тип регистрации
     * @return true при удачной регистриции
     */
    bool    register_read(Message &message, RegisterType register_type = RegisterEnable);
    /**
     * @brief регистрация на чтение (look)
     * @param message       Регистрируемое сообщение
     * @param target_nodes  Список поставщиков (принимать данные только от тех модулей которые в списке)
     * @param register_type Тип регистрации
     * @return true при удачной регистриции
     */
    bool    register_read(Message &message, HostNodeName target_nodes, RegisterType register_type = RegisterEnable);
    /**
     * @brief регистрация на отправку (pub)
     * @param message       Регистрируемое сообщение
     * @param register_type Тип регистрации
     * @param log_suffix    Суфикс для файлов накопления
     * @return true при удачной регистриции
     */
    bool    register_send(Message &message, RegisterType register_type = RegisterEnable, std::string log_suffix = "");
    bool    register_send(Message &message, const SenderOptions options);
    /**
     * @brief регистрация на загрузку значений (load)
     * @param message       Регистрируемое сообщение
     * @param register_type Тип регистрации
     * @return true при удачной регистриции
     */
    bool    register_load(Message &message, RegisterType register_type = RegisterEnable);
    /**
     * @brief регистрация на ожидание (event)
     * @param message       Регистрируемое сообщение
     * @param queue_len     Размер очереди
     * @param register_type Тип регистрации
     * @return true при удачной регистриции
     */
    bool    register_receive(Message &message, int queue_len, RegisterType register_type = RegisterEnable);
    /**
     * @brief регистрация таймера
     * @param timer     Регистрируемый таймер
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_timer(Timer &timer, int queue_len);
    /**
     * @brief Регистрация получения сообщений бортового журнала
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_blackbox(int queue_len);
    /**
     * @brief Регистрация получения сообщений Heartbeat
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_heartbeat(int queue_len);
    /**
     * @brief Регистрация на получение события при смене директории накопления
     * @return true при удачной регистриции
     */
    bool    register_storage_path(int queue_len = 1000);
    /**
     * @brief Регистрация на получение события приема UDP датаграммы
     * @param port      Регистритуемый порт
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_receive_udp_datagram(uint16_t port, int queue_len);
    /**
     * @brief Регистрация на получение события приема UDP датаграммы
     * @param port      Регистритуемый порт
     * @param multicast Мультикаст группа
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_receive_udp_datagram(uint16_t port, std::vector<ipc::UdpSocketReceiver::Multicast> multicast,  int queue_len);
    /**
     * @brief Регистрация на получение события приема данных на серийный порт
     * @param serial    Ссылка на ipc::Serial
     * @param queue_len Размер очереди
     * @return true при удачной регистриции
     */
    bool    register_receive_serial(Serial& serial, int queue_len);
    /**
     * @brief Закрыть серийный порт и отписаться от события
     * @param serial    Ссылка на ipc::Serial
     * @return true при удачной отписки
     */
    bool    unregister_receive_serial(Serial& serial);
    /**
     * @brief Принято сообщение бортового журнала
     * @param blackbox  Объект бортового журнала
     * @return true если принято это сообщение
     */
    bool    received(Blackbox &blackbox);
    /**
     * @brief Принято сообщение Heartbeat
     * @param heartbeat Объект heartbeat
     * @return true если принято это сообщение
     */
    bool    received(Heartbeat &heartbeat);

    /**
     * @brief Принято UDP датаграмма
     * @param port      UDP Порт
     * @param datagram  UDP датаграмма
     * @return true если принято датаграмма от этого порта
     */
    bool    received(uint16_t port, UdpSocketReceiver::UdpDatagram& datagram);
    /**
     * @brief Приняты данные на серийный порт
     * @param serial Объект ipc::Serial
     * @return true если принято датаграмма от этого порта
     */
    bool    received_serial(Serial& serial);
    /**
     * @brief Получить путь загрузки сообщений
     * @return Путь
     */
    std::string load_path();

    /**
     * @brief Получить дополнительные пути загрузки сообщений
     * @return массив путей
     */
    std::vector<std::string> load_paths_addition();

    /**
     * @brief послать команду для начала накопления всеми модулями
     * @param path Путь для накопления
     */
    void    storage_start(std::string path);
    /**
     * @brief послать команду на остановку накопления всеми модулями
     */
    void    storage_stop();
    /**
     * @brief Завершить программу
     * @param prog_name имя программы (пусто - все программы)
     * @param host_name хост программы (пусто - все хосты)
     */
    void    send_exit(std::string node_name = "", std::string host_name = "");
    /**
     * @brief Заменять данные при публикации
     * @param json_text заменяемые данные
     * @param node_name имя целевой программы (пусто - все программы)
     * @param host_name имя целевого хоста (пусто - все хосты)
     */
    void    send_replace_messages(std::string json_text, std::string node_name = "", std::string host_name = "");
    /**
     * @brief Отчистить данные для подмены
     * @param node_name имя целевой программы (пусто - все программы)
     * @param host_name имя целевого хоста (пусто - все хосты)
     */
    void    send_replace_messages_clear(std::string node_name = "", std::string host_name = "");
    /**
     * @brief Записать сообщение в накопитель
     * @param Сообщение
     * @param Тип записи
     */
    void    storage_write(ipc::Message& message, ipc::StorageWriteType write_type);
    /**
     * @brief Записать данные в серийный порт
     * @param port_name Имя порта
     * @param buf       Данные
     * @param data_len  Размер данных
     * @return data_len или 0 - ошибка
     */
    size_t  write_to_serial(std::string port_name, const char *buf, size_t data_len);

public:
    Core();
    /**
     * @brief Core Конструктор
     * @param argc      Количество параметров запуска
     * @param argv      Массив параметров запуска
     */
    Core(int argc, char *argv[]);
    /**
     * @brief Core Конструктор
     * @param argc      Количество параметров запуска
     * @param argv      Массив параметров запуска
     * @param title     Заголовок програмы
     */
    Core(int argc, char *argv[], std::string title);
    /**
     * @brief Core Конструктор
     * @param argc        Количество параметров запуска
     * @param argv        Массив параметров запуска
     * @param description Описание програмы
     */
    Core(int argc, char *argv[], Description description);
    ~Core();
    static Core&   instance();
    /**
     * @brief Остановить обмен
     */
    void exit();
    /**
     * @brief получение текущих значений (look)
     * @param message сообщение
     * @return true при удачном прочтении
     */
    bool    read(Message &message);
    /**
     * @brief получение текущих значений (look)
     * @param message сообщение
     * @return true при удачном прочтении
     */
    bool    read(Message &message, ipc::HostNodeName host_node_name);
    /**
     * @brief отправка данных (pub)
     * @param message      Сообщение
     * @param target_nodes Модули в которые необходимо отправить сообщение
     * @param log_suffix   Фуффикс для файлов накопителя
     * @return true при удачной отправке
     */
    bool    send(Message &message, ipc::HostNodeName target_nodes = ipc::HostNodeName(ipc::HostNodeName::All_Hosts_Nodes), std::string log_suffix = "");
    /**
     * @brief отправка данных (pub)
     * @param message   Сообщение
     * @param options   Опции публикации
     * @return true при удачной отправке
     */
    bool    send(Message &message, const SenderOptions options);
    /**
     * @brief отправка данных (pub) с дублированием отправки через минимальный промежуток времени
     * @param message Сообщение
     * @param target_nodes Модули в которые необходимо отправить сообщение
     * @param log_suffix   Фуффикс для файлов накопителя
     * @return true при удачной отправке
     */
    bool    send_with_repeat(Message &message, ipc::HostNodeName target_nodes = ipc::HostNodeName(ipc::HostNodeName::All_Hosts_Nodes), std::string log_suffix = "");
    /**
     * @brief отправка данных (pub) в формате Json строки
     * @param json строка
     */
    void    send(std::string json);
    /**
     * @brief отправка данных (pub) в формате Json строки
     * @param json строка
     */
    void    send(ipc::Stream json);
    /**
     * @brief получение из очереди (poped)
     * @param message Проверяемое сообщение
     * @param target_nodes Модули от которых необходимо получить сообщение
     * @return true если сообщение находится очереди
     */
    bool    received(Message &message, ipc::HostNodeName target_nodes = ipc::HostNodeName(ipc::HostNodeName::All_Hosts_Nodes));
    /**
     * @brief проверка на срабатывание таймера (poped)
     * @param timer Проверяемый таймер
     * @return true если в очереди находится переданный таймер
     */
    bool    received(Timer &timer);
    /**
     * @brief получение из очереди (poped)
     * @return true если событие находится очереди
     */
    bool    received(Storage_path &storage_path);
    /**
     * @brief timeout
     * @return true если очередь осталась пустая, по истечению таймаута
     */
    bool    timeout();
    /**
     * @brief ожидание прихода сообщения или таймера (wait_and_pop)
     * @param timeout в секундах
     * @return
     */
    bool    receive(double timeout);
    /// ожидание прихода сообщения или таймера (wait_and_pop)
    bool    receive();
    /// проверка работы ядра
    bool    launched();
    /// время в секундах (реальное или симуляциооной в зависимости от настроек)
    DateTime  time();
    /// блокировать вызывающщий поток на 'sec' секунд
    void    sleep(double sec);

    /**
     * @brief Сообщение в бортовой журнал
     * @param type   Тип сообщения
     * @param format формат
     */
    void    log        (State type, std::string format, ...);
    /**
     * @brief Сообщение в бортовой журнал
     * @param text Текст
     * @param type Тип
     */
    void    log        (std::string text, State type = Ok);
    /**
     * @brief Сообщение в бортовой журнал, тип on
     */
    void    log_on     (ipc::Stream text)   {log(text, ipc::On);}
    /**
     * @brief Сообщение в бортовой журнал, тип on
     */
    void    log_on     (ipc::Out text)      {log(text, ipc::On);}
    /**
     * @brief Сообщение в бортовой журнал, тип off
     */
    void    log_off    (ipc::Stream text)   {log(text, ipc::Off);}
    /**
     * @brief Сообщение в бортовой журнал, тип off
     */
    void    log_off    (ipc::Out text)      {log(text, ipc::Off);}
    /**
     * @brief Сообщение в бортовой журнал, тип ok
     */
    void    log_ok     (ipc::Stream text)   {log(text, ipc::Ok);}
    /**
     * @brief Сообщение в бортовой журнал, тип ok
     */
    void    log_ok     (ipc::Out text)      {log(text, ipc::Ok);}
    /**
     * @brief Сообщение в бортовой журнал, тип warinig
     */
    void    log_warning(ipc::Stream text)   {log(text, ipc::Warning);}
    /**
     * @brief Сообщение в бортовой журнал, тип warinig
     */
    void    log_warning(ipc::Out text)      {log(text, ipc::Warning);}
    /**
     * @brief Сообщение в бортовой журнал, тип error
     */
    void    log_error  (ipc::Stream text)   {log(text, ipc::Error);}
    /**
     * @brief Сообщение в бортовой журнал, тип error
     */
    void    log_error  (ipc::Out text)      {log(text, ipc::Error);}
    /**
     * @brief Сообщение в бортовой журнал, тип debug
     */
    void    log_debug  (ipc::Stream text)   {log(text, ipc::Debug);}
    /**
     * @brief Сообщение в бортовой журнал, тип debug
     */
    void    log_debug  (ipc::Out text)      {log(text, ipc::Debug);}
    /**
     * @brief Получить имя своего хоста
     * @return
     */
    std::string hostname();
    /**
     * @brief Получить имя программы
     * @return
     */
    std::string prog_name();
    /**
     * @brief Получить текущий путь для наокпления
     */
    std::string storage_path();
    /**
     * @brief Получить последнее полученое сообщение при наличии такого
     * @return ссылку на Message, память освобождать самому
     */
    Message *received_last_message();
    /**
     * @brief Получить размер накопленной очереди
     */
    uint32_t    queue_size();

    /**
     * @brief Ожидание приема сообщения методом read
     * @param message ожидаемое сообщение
     * @param timeout_wait таймаут ожидания
     * @return true - если возраст сообщения в норме
     */
    bool wait_message(ipc::Message& message, double timeout_wait = 0.0);

    /// Установить название программы
    void    set_title(std::string title);
    /// Все регистрации сообщений окончены (Для того чтобы быстрее открыть TCP порт для WebSocket)
    void    all_registers_end();
};

}
