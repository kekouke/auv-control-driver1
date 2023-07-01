#pragma once
#include "common.h"

class ipc_basic_parameter_t;
class ipc_basic_message_t;

namespace ipc {

/// Класс для парсинга опиания параметра
class CLASS_EXPORT Parameter
{
public:
    /// Тип параметра
    enum Type {
        TypeSigned,                         ///< Числовой знаковый
        TypeUnsigned,                       ///< Числовой беззнаковый
        TypeReal,                           ///< Числовой дробный
        TypeEnum,                           ///< Enum
        TypeBoolean,                        ///< Логический
        TypeString,                         ///< Строковый
        TypeError                           ///< Ошибка - нет параметра
    };

    /// Описание значений ENUM'a
    struct EnumItem {
        std::string _title;                 ///< Заголовок
        std::string _name;                  ///< Имя
        std::string _comment;               ///< Коментарий
        double      _value;                 ///< Значение
        ipc::State  _state;                 ///< Состояние при этом значении
    };

private:
    Type        _type;                      ///< Тип параметра
    uint16_t    _size;                      ///< Размер данных
    uint16_t    _offset;                    ///< Смещение от начала структуры
    std::string _name;                      ///< Имя параметра
    std::string _title;                     ///< Заголовок параметра
    std::string _full_name;                 ///< Полное имя параметра (имя сообщения + путь)
    std::string _path_name;                 ///< Путь для получения занчения из JSON данных сообщения
    std::string _full_title;                ///< Полный заголовок параметра
    std::string _unit;                      ///< Единицы измерения
    std::string _comment;                   ///< комментарий
    bool        _no_limit;                  ///< Нет ограничений по значению параметра
    uint32_t    _full_name_hash;            ///< ХЭШ от полного имени параметра
    uint8_t     _precision;                 ///< точность параметра
    bool        _error_state_exist;         ///< есть значения при которых парметр может быть ipc::Error
    bool        _warning_state_exist;       ///< есть значения при которых парметр может быть ipc::Warning
    std::string _message_name;              ///< Имя сообщения


    int64_t     _default_signed;            ///< Дефолтное значение для знакового типа
    uint64_t    _default_unsigned;          ///< Дефолтное значение для беззнакового типа
    double      _default_real;              ///< Дефолтное значение для дробного типа
    double      _default_enum;              ///< Дефолтное значение для Enum
    bool        _default_boolean;           ///< Дефолтное значение для логического типа
    std::string _default_string;            ///< Дефолтное значение для строкового типа

    int64_t     _min_signed;                ///< Минимальное значение для знакового типа
    int64_t     _max_signed;                ///< Максимальное значение для знакового типа
    uint64_t    _min_unsigned;              ///< Минимальное значение для беззнакового типа
    uint64_t    _max_unsigned;              ///< Максимальное значение для беззнакового типа
    double      _min_real;                  ///< Минимальное значение для дробного типа
    double      _max_real;                  ///< Максимальное значение для дробного типа

    /// Для Boolean
    ipc::State  _true_state;                ///< Состояние для значения true
    ipc::State  _false_state;               ///< Состояние для значения false
    std::string _true_title;                ///< Заголовок для значения true
    std::string _false_title;               ///< Заголовок для значения false

    std::vector<EnumItem>   _enum_items;    ///< Список значений ENUM'а

public:
    /**
     * @brief Конструктор
     * @param par           Указатель на базовый класс параметра
     * @param msg           Указатель на базовый класс сообщения
     */
    Parameter(ipc_basic_parameter_t* par, ipc_basic_message_t* msg);
    Parameter(std::string msg_name);
    /// Получить тип параметра
    Type        type()              const {return _type;}
    /// Подучить тип параметра в виде строки
    std::string type_str()          const;
    /// Получить смещение от начала структуры
    uint16_t    offset()            const {return _offset;}
    /// Получить размер параметра
    uint16_t    size()              const {return _size;}
    /// Получить имя параметра
    std::string name()              const {return _name;}
    /// Получить заголовок параметра
    std::string title()             const {return _title;}
    std::string full_name()         const {return _full_name;}
    std::string path_name()         const {return _path_name;}
    std::string full_title()        const {return _full_title;}
    std::string unit()              const {return _unit;}
    std::string comment()           const {return _comment;}
    bool        no_limit()          const {return _no_limit;}
    uint32_t    full_name_hash()    const {return _full_name_hash;}
    uint8_t     precision()         const {return _precision;}
    std::string message_name()      const {return _message_name;}
    int64_t     default_signed()    const {return _default_signed;}
    uint64_t    default_unsigned()  const {return _default_unsigned;}
    double      default_real()      const {return _default_real;}
    double      default_enum()      const {return _default_enum;}
    bool        default_boolean()   const {return _default_boolean;}
    std::string default_string()    const {return _default_string;}
    int64_t     min_signed()        const {return _min_signed;}
    int64_t     max_signed()        const {return _max_signed;}
    uint64_t    min_unsigned()      const {return _min_unsigned;}
    uint64_t    max_unsigned()      const {return _max_unsigned;}
    double      min_real()          const {return _min_real;}
    double      max_real()          const {return _max_real;}
    ipc::State  true_state()        const {return _true_state;}
    ipc::State  false_state()       const {return _false_state;}
    std::string true_title()        const {return _true_title;}
    std::string false_title()       const {return _false_title;}

    std::vector<EnumItem> enum_items() const {return _enum_items;}


    bool        can_be_error()     const {return _error_state_exist;}
    bool        can_be_warning()   const {return _warning_state_exist;}

    void    name (std::string str)  {_name  = str;}
    void    title(std::string str)  {_title = str;}


private:
    /// Преобразовать строку с тип параметра
    Type str_to_type(const std::string str);
    /// Прочитать дефолтное значение в зависимости от типа параметра
    void    read_default_value(const json_t::object_t *json);
    /// Прочитать минимальные и максимаьные значения в зависимости от типа параметра
    void    read_min_max(const json_t::object_t *json);
    /// Прочтить состояния и заголовки для true/false знечения параметра типа boolean
    void    read_boolean_state(const json_t::object_t *json);
    /// Прочитать значения ENUM'а
    void    read_enum_items(const json_t::basic_t *json);

};

}
