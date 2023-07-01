#pragma once

#include "common.h"
#include "core.h"
#include "schema.h"

namespace ipc {

/**
 * @brief Класс для работы с серийными портами средствами библиотеки
 */
class CLASS_EXPORT Serial {
public:
        enum BaudRate
        {
            BAUDRATE_4800	 = 4800,     // 4800 bps
            BAUDRATE_9600	 = 9600,     // 9600 bps
            BAUDRATE_19200	 = 19200,    // 19200 bps
            BAUDRATE_38400	 = 38400,    // 38400 bps
            BAUDRATE_57600	 = 57600,    // 57600 bps
            BAUDRATE_115200	 = 115200,   // 115200 bps
            BAUDRATE_230400	 = 230400,   // 230400 bps
            BAUDRATE_INVALID = -1
        };
        /**
         * @brief Создает описания енама BaudRate при создании схемы
         */
        inline ipc::Schema::Enum BaundRate_descript(ipc::Schema::Enum e)
        {
            return e.title("Скорость серийного порта")
                    .add(BAUDRATE_4800  , ipc::Ok, "4800"  )
                    .add(BAUDRATE_9600  , ipc::Ok, "9600"  )
                    .add(BAUDRATE_19200 , ipc::Ok, "19200" )
                    .add(BAUDRATE_38400 , ipc::Ok, "38400" )
                    .add(BAUDRATE_57600 , ipc::Ok, "57600" )
                    .add(BAUDRATE_115200, ipc::Ok, "115200")
                    .add(BAUDRATE_230400, ipc::Ok, "230400")
                    ;
        }

        // Parity enum
        enum Parity
        {
            COM_PARITY_NONE  ,
            COM_PARITY_ODD   ,
            COM_PARITY_EVEN  ,
            COM_PARITY_MARK  ,
            COM_PARITY_SPACE
        };

        // StopBits enum
        enum StopBits
        {
            STOPBIT_ONE,
            STOPBIT_TWO,
            STOPBIT_ONE_POINT_FIVE
        };

        // Control types
        enum FlowControl
        {
            CONTROL_NONE = 0,
#if defined (WIN32) || defined (_BSD_SOURCE) || defined (_SVID_SOURCE) || defined (__QNXNTO__)
            CONTROL_HARDWARE_RTS_CTS = 0x01,
#endif
            // DSR/DTR control is not supported under POSIX systems
            // without some special drivers
#if defined (WIN32)
            CONTROL_HARDWARE_DSR_DTR = 0x02,
#endif
            CONTROL_SOFTWARE_XON_IN = 0x04,
            CONTROL_SOFTWARE_XON_OUT = 0x08
        };
        // Device signals
        enum SignalCommand
        {
            RTS_SIGNAL = 0x01,
            DTR_SIGNAL = 0x02
        };

        struct Parameters
        {
            // This sets all of the settings to default
            Parameters() {Defaults();}
            // This sets some common use settings, others leave by default
            Parameters(int baud_rate, StopBits stop_bits = STOPBIT_ONE, Parity parity = COM_PARITY_NONE,
                       FlowControl controls = CONTROL_NONE, uint8_t data_bits = 8)
            {
                Defaults();
                this->baud_rate = baud_rate;
                this->stop_bits = stop_bits;
                this->parity    = parity;
                this->controls  = controls;
                this->data_bits = data_bits;
            }
            // Sets default settings
            void Defaults() {
                baud_rate         = BAUDRATE_115200;
                stop_bits         = STOPBIT_ONE;
                parity            = COM_PARITY_NONE;
                controls          = CONTROL_NONE;
                data_bits         = 8;
                read_buffer_size  = 1500;
                write_buffer_size = 1500;
                on_char           = 0;
                off_char          = (unsigned char)0xFF;
                xon_lim           = 128;
                xoff_lim          = 128;
            }
            int        baud_rate;
            StopBits   stop_bits;
            Parity     parity;
            int        controls;
            uint8_t    data_bits;
            size_t     read_buffer_size;
            size_t     write_buffer_size;
            uint8_t    on_char;
            uint8_t    off_char;
            int        xon_lim;
            int        xoff_lim;
        };

private:
    Core &core;
    std::string       _port_name;
    std::vector<char> _data;
    Parameters        _parameters;
    bool              _register_state;
public:
    /**
     * @brief Конструктор класса без открытия порта
     */
    Serial(Core &core);
    /**
     * @brief Коструктор класса с открытием порта и регитрацией на получение данных
     * @param core          Ссылка на Core
     * @param port_name     Имя порта
     * @param parameters    Парамеры подключения
     * @param queue_len     Размер очереди
     */
    Serial(Core &core, const std::string& port_name, const Parameters& parameters, int queue_len = 100);
    /**
     * @brief Коструктор класса с открытием порта и регитрацией на получение данных
     * @param core          Ссылка на Core
     * @param port_name     Имя порта
     * @param queue_len     Размер очереди
     */
    Serial(Core &core, const std::string& port_name, int queue_len = 100);
    /**
     * @brief Открыть серийный порт и зарегистрироваться на получение события принятия данных
     * @param port_name  Имя порта
     * @param parameters Параметры порта
     * @param queue_len  Размер очереди
     * @return true - при удачном открытии серийного порта и регистрации
     */
    bool open(const std::string& port_name, const Parameters& parameters, int queue_len = 100);
    /**
     * @brief Закрыть порт и отписаться от события
     * @return true -  при удачном закрытии и отписке
     */
    bool close();
    /**
     * @brief Проверка прихода сообщения
     * @return true - если данные пришли
     */
    bool    received();
    /**
     * @brief Получить последние считаные данные
     */
    std::vector<char>&   data();
    /**
     * @brief Записать данные в серийный порт
     * @param buf       Данные
     * @param data_len  Размер данных
     * @return data_len или 0 - ошибка
     */
    size_t  write(const char *buf, size_t data_len);
    /**
     * @brief Получить название порта
     */
    std::string port() {return _port_name;}
    /**
     * @brief Получить параметры подключения
     */
    Parameters  parameters() {return  _parameters;}

    /**
     * @brief Проверка на удачное открытие порта
     */
    bool    is_open();
};

}
