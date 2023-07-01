#pragma once

#include "common.h"
#include "stream.h"

namespace ipc {

/// Класс описания сообщения
class CLASS_EXPORT Schema
{
public:
    /// Тип параметра
    enum Type
    {
        Type_signed,        ///< Знаковый
        Type_unsigned,      ///< Безнаковый
        Type_real,          ///< Дробной
        Type_boolean,       ///< Логический
        Type_string         ///< Строковый
    };

    /**
     * @brief Преобразование типа в строку
     * @param type Тип
     * @return текст
     */
    static std::string type_to_str(Type type);

private:
    class CLASS_EXPORT Atom_basic
    {
    protected:
        void       *_data_point;
        uint64_t    _size;
        std::string _name;
        std::string _title;
        std::string _options;
        std::string _comment;
        std::string _alarm_title;
        Type        _type;
        bool        _unvisible;
    public:
        Atom_basic(void *data_point, uint64_t size, std::string name)
            : _data_point(data_point)
            , _size(size)
            , _name(name)
            , _title(name)
            , _type(Type_signed)
            , _unvisible(false)
        {}
        virtual ~Atom_basic() {}

        /// Вернуть указатель на данные, используется для расчета offset
        uint64_t    get_data_point() const {return (uint64_t)_data_point;}
        /// Вернуть размер данных в байтах
        uint64_t    get_size()       const {return _size;}
        /// Вернуть имя параметра
        std::string get_name()       const {return _name;}
        /// Вернуть название параметра
        std::string get_title()      const {return _title;}
        /// Вернуть тип параметра
        Type        get_type()       const {return _type;}

        /// Виртуальная функция для составления описания параметра
        virtual json_t::object_t to_object() const = 0;

    protected:
        /// Установить название параметра
        void title(std::string str)      {_title = str;}
        /// Установить дополнительные опции в формате JSON
        void options(std::string str)    {_options = str;}
        /// Установить коментарий
        void comment(std::string str)    {_comment = str;}
        /// Установить название аварии при аврийном значении параметра
        void alarm_title(std::string text)  {_alarm_title = text;}

        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object_basic() const;

    };

public:
    /// Класс для описания числового параметра
    class CLASS_EXPORT Number : public Atom_basic
    {
    protected:
        std::string _unit;                  ///< единицы измерения
        double      _min;                   ///< Минимальное значение
        double      _max;                   ///< Максимальное значение
        double      _default;               ///< Значение по умолчанию
        int64_t     _precision;             ///< Точность параметра
        bool        _no_limit;              ///< Флаг, об отсутствии ограничений (min и max)
        bool        _show_bin;              ///< Флаг отображения данных в бинарном виде
        bool        _show_hex;              ///< Флаг отобрадения данных в HEX виде
        bool        _unvisible_null;        ///< Флаг скрытия данных при нулевом значении
        bool        _disable_null;          ///< Флаг "неактивности" данных при нулевом значении
        bool        _invalid_null;          ///< Когда ноль - параметр будет задизаблен. -1 - вообще исчезнет

        /// Инициализация
        void init();

    public:
        /**
         * @brief Конструкторы
         * @param value Указатель на данные
         * @param size  Размер данных
         * @param name  Имя параметра
         */
        Number(int8_t   *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_signed;   init();}
        Number(int16_t  *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_signed;   init();}
        Number(int32_t  *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_signed;   init();}
        Number(int64_t  *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_signed;   init();}
        Number(uint8_t  *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_unsigned; init();}
        Number(uint16_t *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_unsigned; init();}
        Number(uint32_t *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_unsigned; init();}
        Number(uint64_t *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_unsigned; init();}
        Number(float    *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_real;     init();}
        Number(double   *value, size_t size, std::string name): Atom_basic(value, size, name) {_type= Type_real;     init();}

        /// Установить название переменной, [значение по умолчанию] и ед.изм.
        Number& info(std::string title                              )  {Atom_basic::title(title);                               return *this;}
        Number& info(std::string title,             std::string unit)  {Atom_basic::title(title);                 _unit = unit; return *this;}
        Number& info(std::string title, double def                  )  {Atom_basic::title(title); _default = def;               return *this;}
        Number& info(std::string title, double def, std::string unit)  {Atom_basic::title(title); _default = def; _unit = unit; return *this;}

        /// Установить название переменной, используется при отображении во вьювере
        Number& title(std::string str)  {Atom_basic::title(str);   return *this;}
        /// Установить единицу измернения, используется при отображении во вьювере
        Number& unit(std::string str)   {_unit = str; return *this;}
        /// Установить дополнительные опции
        Number& options(std::string str) {Atom_basic::options(str); return *this;}
        /// Установить коментарий
        Number& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /// Установить название аварии при аврийном значении параметра
        Number& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Установить значение по умолчанию
        Number& default_(double value)      {_default   = value; return *this;}
        /// Установить минимально возможное значение параметра, используется при отображении во вьювере и в КАСе
        Number& minimum(double value)       {_min = value; _no_limit=false; return *this;}
        /// Установить максимально возможное значение параметра, используется при отображении во вьювере и в КАСе
        Number& maximum(double value)       {_max = value; _no_limit=false; return *this;}
        /// Установить точность параметра (актуално для типов float и double), используется при отображении во вьювере и при накомплении данных
        Number& precision(int64_t value)    {_precision = value; return *this;}
        /// Установить разрешенный диапазон значений для параметра (минимум и максимум)
        Number& range(double min, double max) {_min = min; _max = max; _no_limit=false; return *this;}
        /// Установить разрешенный диапазон значений для параметра (минимум и максимум)
        Number& error_range(double min, double max) {_min = min; _max = max; _no_limit=false; return *this;}
        /// Включить/отключить отображение в бинарном виде
        Number& show_bin(bool f=true) {_show_bin = f; return *this;}
        /// Включить/отключить отображение в HEX виде
        Number& show_hex(bool f=true) {_show_hex = f; return *this;}
        /// Скрывать поле при значении 0
        Number& unvisible_null(bool f = true) {_unvisible_null = f; return *this;}
        /// Не активное поле при значении 0
        Number& disable_null(bool f=true) {_disable_null = f; return *this;}
        /// Когда ноль - параметр будет задизаблен. -1 - вообще исчезнет
        Number& invalid_null(bool f=true) {_invalid_null = f; return *this;}
        /// Скрыть параметр во вьювере
        Number& unvisible(bool f=true) {_unvisible = f; return *this;}
        /// Отменить введенные ранее ограничения, минимум/максимум
        Number& no_limit() {_no_limit = true; return *this;}
        /// Вернуть значение по умолчанию
        double  get_default()    const {return _default;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string()      const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    /// Класс для описания логического параметра
    class CLASS_EXPORT Boolean : public Atom_basic
    {
    protected:
        std::string _title_true;            ///< Текст при значении true
        std::string _title_false;           ///< Текст при значении false
        State       _state_true;            ///< Состояние параметра при значении true
        State       _state_false;           ///< Состояние параметра при значении false
        bool        _default;               ///< Значение по умочанию
        bool        _enable_neighbor;       ///< Флаг, Мутировать соседный элемент
        bool        _enable_neighbors;      ///< Флаг, Мутировать все соседние элементы
        bool        _visible_neighbor;      ///< Флаг, Скрыть соседный элемент
        bool        _visible_neighbors;     ///< Флаг, Скрыть все соседние элемент
        bool        _unvisible_null;        ///< Флаг, Скрывать поле при значении false
        bool        _disable_null;          ///< Флаг, Не активное поле при значении false


    public:
        /**
         * @brief Конструктор
         * @param value Указатель на данные
         * @param size  Размер данных
         * @param name  Имя параметра
         */
        Boolean(bool *value, size_t size, std::string name)
            : Atom_basic    (value, size, name)
            , _title_true   ("true")
            , _title_false  ("false")
            , _state_true   (ipc::On)
            , _state_false  (ipc::Off)
            , _default      (false)
            , _enable_neighbor(false)
            , _enable_neighbors(false)
            , _visible_neighbor(false)
            , _visible_neighbors(false)
            , _unvisible_null(false)
            , _disable_null(false)
        {_type = Type_boolean;}

        /**
         * @brief Установить название параметра
         * @param str Название параметра
         * @return Ссылка на объект
         */
        Boolean& title      (std::string str)   {Atom_basic::title(str);   return *this;}
        /**
         * @brief Устанвить описание для значения FALSE
         * @param state Состаяние
         * @param title_false Название
         * @return Ссылка на объект
         */
        Boolean& false_     (State state, std::string title_false)
        {_state_false = state; _title_false = title_false; return *this;}
        Boolean& false_off  (std::string title_false)
        {_state_false = ipc::Off; _title_false = title_false; return *this;}
        /**
         * @brief Устанвить описание для значения TRUE
         * @param state Состаяние
         * @param title_true Название
         * @return Ссылка на объект
         */
        Boolean& true_      (State state, std::string title_true)
        {_state_true = state; _title_true = title_true; return *this;}
        Boolean& true_on    (std::string title_true)
        {_state_true = ipc::On; _title_true = title_true; return *this;}
        /**
         * @brief Установить дополнительные опции
         * @param str
         * @return Ссылка на объект
         */
        Boolean& options(std::string str) {Atom_basic::options(str); return *this;}
        /**
         * @brief Установить коментарий
         * @param str строка
         * @return Ссылка на объект
         */
        Boolean& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /**
         * @brief Установить название аварии при аврийном значении параметра
         * @param Название аварии
         */
        Boolean& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Установить значение по умолчанию
        Boolean& default_   (bool value)     {_default     = value; return *this;}
        /// Выставить сервисный флаг (Мутировать соседный элемент)
        Boolean& enable_neighbor(bool f=true)   {_enable_neighbor   = f; return *this;}
        /// Выставить сервисный флаг (Мутировать все соседние элементы)
        Boolean& enable_neighbors(bool f=true)  {_enable_neighbors  = f; return *this;}
        /// Выставить сервисный флаг (Скрыть соседный элемент)
        Boolean& visible_neighbor(bool f=true)  {_visible_neighbor  = f; return *this;}
        /// Выставить сервисный флаг (Скрыть все соседние элемент)
        Boolean& visible_neighbors(bool f=true) {_visible_neighbors = f; return *this;}
        /// Скрыть параметр во вьювере
        Boolean& unvisible(bool f=true) {_unvisible = f; return *this;}
        /// Скрывать поле при значении false
        Boolean& unvisible_null(bool f=true) {_unvisible_null = f; return *this;}
        /// Не активное поле при значении false
        Boolean& disable_null(bool f=true) {_disable_null = f; return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string()      const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    /// Класс для описания строкового параметра
    class CLASS_EXPORT String : public Atom_basic
    {
    protected:
        std::string _default;           ///< Значение по умолчанию
        bool        _unvisible_null;    ///< Флаг, Скрывать поле при значении ""
        bool        _disable_null;      ///< Флаг, Не активное поле при значении ""

        std::vector<std::string> _enum_values;

    public:
        /**
         * @brief Конструктор
         * @param value Указатель на данные
         * @param size  Размер данных
         * @param name  Имя параметра
         */
        String(void *value, size_t size, std::string name)
            : Atom_basic(value, size, name)
            , _default("")
            , _unvisible_null(false)
            , _disable_null(false)
        {_type = Type_string;}

        /// Установить название параметра, используется при отображении во вьювере
        String& title(std::string str)  {Atom_basic::title(str); return *this;}
        /// Добавить значение для Енама
        String& add_value(const std::string& str) {_enum_values.push_back(str); return *this;}
        /// Установить значние по умолчанию
        String& default_(std::string value)   {_default = value; return *this;}
        /// Установить дополнительные опции
        String& options(std::string str) {Atom_basic::options(str); return *this;}
        /// Установить коментарий
        String& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /// Установить название аварии при аврийном значении параметра
        String& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Скрыть параметр во вьювере
        String& unvisible(bool f=true) {_unvisible = f; return *this;}
        /// Скрывать поле при значении ""
        String& unvisible_null(bool f=true) {_unvisible_null = f; return *this;}
        /// Не активное поле при значении ""
        String& disable_null(bool f=true) {_disable_null = f; return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string()      const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    /// Класс для описания ENUM
    class CLASS_EXPORT Enum : public Atom_basic
    {
    public:
        /// Класс описания элемента ENUMа
        class ItemName {
        public:
            ItemName(std::string name, double value)
                : _name(name)
                , _value(value)
            {}

            std::string _name;      ///< Имя
            double      _value;     ///< Значение
        };

        /// Класс описания метаинформации
        class CLASS_EXPORT Metadata {
        private:
            std::string _title;
            std::string _comment;

            ipc::State  _state_true;
            ipc::State  _state_false;
            std::string _title_true;
            std::string _title_false;

            double      _min;
            double      _max;
            double      _default_number;
            std::string _unit;

            int64_t     _precision;

            bool        _no_limit;

        public:
            Metadata();

            Metadata& title         (std::string s) {_title          = s; return *this;}
            Metadata& unit          (std::string s) {_unit           = s; return *this;}
            Metadata& comment       (std::string s) {_comment        = s; return *this;}
            Metadata& state_true    (ipc::State s)  {_state_true     = s; return *this;}
            Metadata& state_false   (ipc::State s)  {_state_false    = s; return *this;}
            Metadata& title_true    (std::string s) {_title_true     = s; return *this;}
            Metadata& title_false   (std::string s) {_title_false    = s; return *this;}
            Metadata& precision     (int64_t i)     {_precision      = i; return *this;}
            Metadata& default_number(double v)      {_default_number = v; return *this;}
            Metadata& no_limit      (bool b)        {_no_limit       = b; return *this;}

            /// Метаданные для логического параметра
            Metadata& for_bool(ipc::State state_true, std::string title_true
                               , ipc::State state_false, std::string title_false);

            Metadata& for_number(std::string unit, double min, double max);
            Metadata& for_number(std::string unit, double min, double max, int64_t precision);

            std::string title()          const {return _title;}
            std::string comment()        const {return _comment;}
            ipc::State  state_true()     const {return _state_true;}
            ipc::State  state_false()    const {return _state_false;}
            std::string title_true()     const {return _title_true;}
            std::string title_false()    const {return _title_false;}
            std::string unit()           const {return _unit;}
            double      min()            const {return _min;}
            double      max()            const {return _max;}
            double      default_number() const {return _default_number;}
            int64_t     precision()      const {return _precision;}
            bool        is_no_limit()    const {return _no_limit;}
        };

    protected:
        /// Струкрура описания каждого значения ENUM
        struct Item
        {
            double      value;    ///< Значение
            std::string title;    ///< Название
            std::string name;     ///< Название как в enum
            State       state;    ///< Состояние, используется при отображении во вьювере и КАСе (ipc::Error - вызывает аварию)
            Metadata    metadata; ///< Метаданные
        };

        double              _default;               ///< Значение по умолчанию
        bool                _validity_neighbor;     ///< флаг validity
        bool                _validity_neighbors;    ///< флаг validitys
        std::vector<Item>   item_list;              ///< список описанных значений

    public:
        Enum(const Number &n)
            : Atom_basic((void*)n.get_data_point(), n.get_size(), n.get_name())
            , _default(n.get_default())
            , _validity_neighbor(true)
            , _validity_neighbors(true)
        {
        }

        /// Установить название параметра
        Enum& title(std::string str) {Atom_basic::title(str); return *this;}
        /// Установить значение по умолчанию
        Enum& default_(double value)   {_default = value; return *this;}
        /**
         * @brief Добавить заначение в ENUM
         * @param value Значение
         * @param title Название, используется при отображении во вьювере
         * @param state Состояние, используется при отображении во вьювере и КАСе
         */
        Enum& add(double value, State state, std::string title_str, Metadata metadata = Metadata());
        /**
         * @brief Добавить заначение в ENUM
         * @param item_name класс содержащий значение и название
         * @param title Название, используется при отображении во вьювере
         * @param state Состояние, используется при отображении во вьювере и КАСе
         */
        Enum& add(ItemName item_name, State state, std::string title_str, Metadata metadata = Metadata());
        /**
         * @brief Добавить заначение в ENUM (title = name)
         * @param item_name Класс содержащий значение и название
         * @param state     Состояние, используется при отображении во вьювере и КАСе
         */
        Enum& add(ItemName item_name, State state, Metadata metadata = Metadata());
        /**
         * @brief Добавить заначение в ENUM (state = ipc::Ok,  title = name)
         * @param item_name Класс содержащий значение и название
         */
        Enum& add(ItemName item_name);
        /// Установить дополнительные опции
        Enum& options(std::string str) {Atom_basic::options(str); return *this;}
        /// Установить коментарий
        Enum& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /// Установить название аварии при аврийном значении параметра
        Enum& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Выставить флаг validity
        Enum& validity_neighbor(bool v = true)  {_validity_neighbor = v; return *this;}
        /// Выставить флаг validity
        Enum& validity_neighbors(bool v = true) {_validity_neighbors = v; return *this;}
        /// Скрыть параметр во вьювере
        Enum& unvisible(bool f=true) {_unvisible = f; return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string()      const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
        /**
         * @brief Получить название для значения
         * @param value значение
         * @return Название параметра. "not_found" - если значение не найдено
         */
        std::string get_item_title(int value, std::string name = "") const;
        /**
         * @brief Получить имена для значения
         * @param value значение
         * @return Название параметра. "not_found" - если значение не найдено
         */
        std::string get_item_name(int value) const;
        /**
         * @brief Получить метаданные для значения
         * @param value значение
         * @return Метаданные
         */
        Metadata get_item_metadata(int value, std::string name = "") const;
    };

    /// Класс для описания вложенной структуры
    class CLASS_EXPORT Struct : public Atom_basic
    {
    private:
        json_t::object_t*   _json;              ///< Схема структуры
        bool                _title_unvisible;   ///< Флаг, скрыть заголовок структуры

    public:
        /**
         * @brief Конструктор
         * @param data_point    Указатель на структуру
         * @param name          Имя параметра
         * @param schema        Описание структуры
         */
        Struct(void *data_point, std::string name, const Schema &schema);
        Struct(const Struct &s);
        ~Struct();

        /// Установить название параметра
        Struct& title(std::string str)  {Atom_basic::title(str); return *this;}
        /// Установить дополнительные опции
        Struct& options(std::string str) {Atom_basic::options(str); return *this;}
        /// Установить коментарий
        Struct& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /// Скрыть параметр во вьювере
        Struct& unvisible(bool f=true) {_unvisible = f; return *this;}
        /// Установить название аварии при аврийном значении параметра
        Struct& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Показывать/скрывать title структуры во вьювере
        Struct& title_unvisible(bool f=true)  {_title_unvisible = f; return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string()      const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    /// Базовый класс для массивов
    class CLASS_EXPORT Array_basic : public Atom_basic
    {
    protected:
        int64_t                     _element_minimum;   ///< Минимальное количество элементов
        uint64_t                    _element_count;     ///< Количество элеменов
        std::vector<std::string>    _element_titles;    ///< Заголовки элементов
        bool                        _title_unvisible;   ///< Флаг, скрыть заголовок массива
    public:
        /**
         * @brief Конструктор
         * @param data_point    Указатель на данные
         * @param size          Размер данных
         * @param name          Имя параметра
         */
        Array_basic(void *data_point, size_t size, std::string name);
        /// Конструктор копирования
        Array_basic(const Array_basic& a);
        virtual ~Array_basic();

    protected:
        /// Установить имена параметров из описанного ENUM'а
        void set_titles(const Enum& e);
        /// Выставить флаг скрытия заголовка массива
        void title_unvisible(bool f) {_title_unvisible = f;}
        /// Скрыть параметр во вьювере
        void unvisible(bool f=true) {_unvisible = f;}
        /// Вытсавить минимальное количество элементов в массиве
        void minItem(int64_t c) {_element_minimum = c;}

        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;

    };

    /// Класс для описания массива
    class CLASS_EXPORT Array : public Array_basic
    {
    private:
        json_t::array_t*            _json;
    public:
        /**
         * @brief Конструктор
         * @param data_point    Указатель на данные
         * @param size          Размер данных
         * @param name          Имя параметра
         */
        Array(void *data_point, size_t size, std::string name);
        Array(const Array &a);
        ~Array();

        /** Добавить параметр опреденного типа в массив */
        Array& add(const Number  &item);
        Array& add(const Boolean &item);
        Array& add(const String  &item);
        Array& add(const Enum    &item);
        Array& add(const Array   &item);
        Array& add(const Struct  &item);

        /**
         * @brief Заполнить описание сразу всего массива
         * @param item      Тип из которого состоит массив
         * @param count     Количество элеменетов в массиве
         * @param titles    Список названий параметров (по порядку)
         * @return Ссылка на объект
         */
        Array& set(const Number  &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());
        Array& set(const Boolean &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());
        Array& set(const String  &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());
        Array& set(const Enum    &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());
        Array& set(const Array   &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());
        Array& set(const Struct  &item, size_t count, std::vector<std::string> titles = std::vector<std::string>());

        Array& set(const Number  &item, size_t count, const char* const titles[]);
        Array& set(const Boolean &item, size_t count, const char* const titles[]);
        Array& set(const String  &item, size_t count, const char* const titles[]);
        Array& set(const Enum    &item, size_t count, const char* const titles[]);
        Array& set(const Array   &item, size_t count, const char* const titles[]);
        Array& set(const Struct  &item, size_t count, const char* const titles[]);

        /// Устновить название параметра, используется при отображении во вьювере
        Array& title(std::string str)   {Atom_basic::title(str); return *this;}
        /// Установить дополнительные опции
        Array& options(std::string str) {Atom_basic::options(str); return *this;}
        /// Установить коментарий
        Array& comment(std::string str) {Atom_basic::comment(str); return *this;}
        /// Установить название аварии при аврийном значении параметра
        Array& alarm_title(std::string text) {Atom_basic::alarm_title(text); return *this;}
        /// Скрыть параметр во вьювере
        Array& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string      to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_number : public Array_basic
    {
    private:
        Number                   number;                    ///< Базовый тип елемента массива
        std::vector<std::string> _element_units;            ///< список единиц измерения
        std::vector<double>      _element_minimums;         ///< список минимальных значений
        std::vector<double>      _element_maximums;         ///< список максимальных значений
        std::vector<double>      _element_defaults;         ///< список дефолтных значений
        std::vector<int64_t>     _element_precisions;       ///< список точностей

    public:
        Array_number(int8_t    *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(int16_t   *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(int32_t   *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(int64_t   *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(uint8_t   *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(uint16_t  *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(uint32_t  *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(uint64_t  *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(float     *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}
        Array_number(double    *value, size_t size_element, std::string name, size_t size_all_array) : Array_basic(value, size_all_array, name), number(value, size_element, name) {_element_count = size_all_array / size_element;}

        Array_number& title  (std::string str)       {Atom_basic::title(str);         return *this;}
        Array_number& options(std::string str)       {Atom_basic::options(str);       return *this;}
        Array_number& comment(std::string str)       {Atom_basic::comment(str);       return *this;}
        Array_number& alarm_title(std::string text)  {Atom_basic::alarm_title(text);  return *this;}

        Array_number& element_title(std::string str)  {number.title(str);      return *this;}
        Array_number& element_unit (std::string str)  {number.unit (str);      return *this;}
        Array_number& element_default(double value)   {number.default_(value); return *this;}
        Array_number& element_minimum(double value)   {number.minimum(value);  return *this;}
        Array_number& element_maximum(double value)   {number.maximum(value);  return *this;}
        Array_number& element_precision(int64_t value){number.precision(value);return *this;}
        Array_number& element_comment(std::string str){number.comment(str);    return *this;}
        Array_number& element_error_range(double min, double max) {number.error_range(min, max);  return *this;}

        Array_number& element_titles    (const Enum& e)                   {set_titles(e); return *this;}
        Array_number& element_titles    (std::vector<std::string> titles) {if(titles    .size() == _element_count) _element_titles     = titles;     return *this;}
        Array_number& element_units     (std::vector<std::string> units)  {if(units     .size() == _element_count) _element_units      = units;      return *this;}
        Array_number& element_minimums  (std::vector<double> minimums)    {if(minimums  .size() == _element_count) _element_minimums   = minimums;   return *this;}
        Array_number& element_maximums  (std::vector<double> maximums)    {if(maximums  .size() == _element_count) _element_maximums   = maximums;   return *this;}
        Array_number& element_defaults  (std::vector<double> defaults)    {if(defaults  .size() == _element_count) _element_defaults   = defaults;   return *this;}
        Array_number& element_precisions(std::vector<int64_t> precisions) {if(precisions.size() == _element_count) _element_precisions = precisions; return *this;}
        Array_number& element_show_bin      (bool f=true)   {number.show_bin(f); return *this;}
        Array_number& element_show_hex      (bool f=true)   {number.show_hex(f); return *this;}
        Array_number& element_unvisible_null(bool f=true)   {number.unvisible_null(f); return *this;}
        Array_number& element_disable_null  (bool f=true)   {number.disable_null(f); return *this;}
        Array_number& element_invalid_null  (bool f=true)   {number.invalid_null(f); return *this;}
        Array_number& element_unvisible     (bool f=true)   {number.unvisible(f); return *this;}

        /// Скрыть параметр во вьювере
        Array_number& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_number& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_number& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_boolean : public Array_basic
    {
    private:
        Boolean             boolean;            ///< Базовый тип елемента массива
        std::vector<bool>   _element_defaults;  ///< список дефолтных значений

    public:
        Array_boolean(bool *value, size_t size_element, std::string name, size_t size_all_array)
            : Array_basic(value, size_all_array, name)
            , boolean(value, size_element, name)
        {_element_count = size_all_array / size_element;}

        Array_boolean& title  (std::string str)       {Atom_basic::title(str);         return *this;}
        Array_boolean& options(std::string str)       {Atom_basic::options(str);       return *this;}
        Array_boolean& comment(std::string str)       {Atom_basic::comment(str);       return *this;}
        Array_boolean& alarm_title(std::string text)  {Atom_basic::alarm_title(text); return *this;}

        Array_boolean& element_title(std::string str) {boolean.title(str);      return *this;}
        Array_boolean& element_default(bool value)    {boolean.default_(value); return *this;}

        Array_boolean& element_false(State state, std::string title_false)    {boolean.false_(state, title_false); return *this;}
        Array_boolean& element_true (State state, std::string title_true)     {boolean.true_ (state, title_true ); return *this;}

        Array_boolean& element_titles    (const Enum& e)                   {set_titles(e); return *this;}
        Array_boolean& element_titles    (std::vector<std::string> titles) {if(titles  .size() == _element_count) _element_titles     = titles;     return *this;}
        Array_boolean& element_defaults  (std::vector<bool> defaults)      {if(defaults.size() == _element_count) _element_defaults   = defaults;   return *this;}

        Array_boolean& element_enable_neighbor  (bool f=true) {boolean.enable_neighbor(f);   return *this;}
        Array_boolean& element_enable_neighbors (bool f=true) {boolean.enable_neighbor(f);   return *this;}
        Array_boolean& element_visible_neighbor (bool f=true) {boolean.visible_neighbor(f);  return *this;}
        Array_boolean& element_visible_neighbors(bool f=true) {boolean.visible_neighbors(f); return *this;}
        Array_boolean& element_unvisible        (bool f=true) {boolean.unvisible(f);         return *this;}
        Array_boolean& element_unvisible_null   (bool f=true) {boolean.unvisible_null(f);    return *this;}
        Array_boolean& element_disable_null     (bool f=true) {boolean.disable_null(f);      return *this;}

        /// Скрыть параметр во вьювере
        Array_boolean& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_boolean& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_boolean& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_string : public Array_basic
    {
    private:
        String                      string;                 ///< Базовый тип елемента массива
        std::vector<std::string>    _element_defaults;      ///< список дефолтных значений
    public:
        Array_string(void *value, size_t size_element, std::string name, size_t size_all_array)
            : Array_basic(value, size_all_array, name)
            , string(value, size_element, name)
        {_element_count = size_all_array / size_element;}

        Array_string& title  (std::string str) {Atom_basic::title(str);   return *this;}
        Array_string& options(std::string str) {Atom_basic::options(str); return *this;}
        Array_string& comment(std::string str) {Atom_basic::comment(str); return *this;}
        Array_string& alarm_title(std::string text)  {Atom_basic::alarm_title(text); return *this;}

        Array_string& element_title  (std::string str) {string.title(str);    return *this;}
        Array_string& element_default(std::string str) {string.default_(str); return *this;}

        Array_string& element_titles    (const Enum& e)                     {set_titles(e); return *this;}
        Array_string& element_titles    (std::vector<std::string> titles)   {if(titles  .size() == _element_count) _element_titles     = titles;     return *this;}
        Array_string& element_defaults  (std::vector<std::string> defaults) {if(defaults.size() == _element_count) _element_defaults   = defaults;   return *this;}

        Array_string& element_unvisible     (bool f=true) {string.unvisible(f); return *this;}
        Array_string& element_unvisible_null(bool f=true) {string.unvisible_null(f); return *this;}
        Array_string& element_disable_null  (bool f=true) {string.disable_null(f); return *this;}

        /// Скрыть параметр во вьювере
        Array_string& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_string& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_string& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_enum : public Array_basic
    {
    private:
        Enum                    _enum;                  ///< Базовый тип елемента массива
        std::vector<int64_t>    _element_defaults;      ///< список дефолтных значений
    public:
        Array_enum(const Number &n, size_t size_all_array)
            : Array_basic((void*)n.get_data_point(), size_all_array, n.get_name())
            , _enum(n)
        {_element_count = size_all_array / n.get_size();}

        Array_enum(const Enum &n, size_t size_all_array)
            : Array_basic((void*)n.get_data_point(), size_all_array, n.get_name())
            , _enum(n)
        {_element_count = size_all_array / n.get_size();}

        Array_enum& title  (std::string str) {Atom_basic::title(str);   return *this;}
        Array_enum& options(std::string str) {Atom_basic::options(str); return *this;}
        Array_enum& comment(std::string str) {Atom_basic::comment(str); return *this;}
        Array_enum& alarm_title(std::string text)  {Atom_basic::alarm_title(text); return *this;}

        Array_enum& element_title  (std::string str) {_enum.title(str);    return *this;}
        Array_enum& element_default(double value) {_enum.default_(value);  return *this;}
        Array_enum& element_add    (double value, State state, std::string title_str)
        {_enum.add(value, state, title_str); return *this;}

        Array_enum& element_titles    (const Enum& e)                   {set_titles(e); return *this;}
        Array_enum& element_titles    (std::vector<std::string> titles) {if(titles  .size() == _element_count) _element_titles     = titles;     return *this;}
        Array_enum& element_defaults  (std::vector<int64_t> defaults)   {if(defaults.size() == _element_count) _element_defaults   = defaults;   return *this;}

        Array_enum& element_validity_neighbor   (bool f=true) {_enum.validity_neighbor(f);  return *this;}
        Array_enum& element_validity_neighbors  (bool f=true) {_enum.validity_neighbors(f); return *this;}
        Array_enum& element_unvisible           (bool f=true) {_enum.unvisible(f);          return *this;}


        /// Скрыть параметр во вьювере
        Array_enum& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_enum& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_enum& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_struct : public Array_basic
    {
    private:
        Struct  _struct;            ///< Базовый тип елемента массива
        bool    _all_items_equal;   ///< флаг одинаковости описания всех элеметров массива
    public:
        Array_struct(void *data_point, std::string name, const Schema &schema, size_t size_all_array);

        Array_struct& title  (std::string str) {Atom_basic::title(str);   return *this;}
        Array_struct& options(std::string str) {Atom_basic::options(str); return *this;}
        Array_struct& comment(std::string str) {Atom_basic::comment(str); return *this;}
        Array_struct& alarm_title(std::string text)  {Atom_basic::alarm_title(text); return *this;}

        Array_struct& element_title  (std::string str)                 {_struct.title(str); return *this;}
        Array_struct& element_titles (const Enum& e)                   {set_titles(e);      _all_items_equal = false; return *this;}
        Array_struct& element_titles (std::vector<std::string> titles) {if(titles.size() == _element_count) {_element_titles = titles; _all_items_equal = false;} return *this;}

        Array_struct& element_title_unvisible(bool f = true)            {_struct.title_unvisible(f); return *this;}
        Array_struct& element_unvisible(bool f=true)                    {_struct.unvisible(f); return *this;}
        /// Скрыть параметр во вьювере
        Array_struct& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_struct& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_struct& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        /// Выставить флаг о том что все элементы массива имеют одинаковое описание (схема при этом сокращается)
        Array_struct& all_items_equal(bool b = true) {_all_items_equal = b; return *this;}
        /// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

    class CLASS_EXPORT Array_array : public Array_basic
    {
    private:
        json_t::object_t* _json;
        size_t            _element_size;
        std::string       _element_title;
    public:
        Array_array(void *data_point, size_t size, std::string name)
            : Array_basic(data_point, size, name)
            , _json(0)
            , _element_size(0)
            , _element_title("")
        {}
        Array_array(const Array_array &a);
        ~Array_array();

        Array_array& title  (std::string str) {Atom_basic::title(str);   return *this;}
        Array_array& options(std::string str) {Atom_basic::options(str); return *this;}
        Array_array& comment(std::string str) {Atom_basic::comment(str); return *this;}
        Array_array& alarm_title(std::string text)  {Atom_basic::alarm_title(text); return *this;}

        Array_array& set(const Array_boolean &item);
        Array_array& set(const Array_enum    &item);
        Array_array& set(const Array_number  &item);
        Array_array& set(const Array_string  &item);
        Array_array& set(const Array_struct  &item);
        Array_array& set(const Array_array   &item);

        Array_array& element_title(std::string str) {_element_title = str; return *this;}

        /// Скрыть параметр во вьювере
        Array_array& unvisible(bool f=true) {Array_basic::unvisible(f); return *this;}
        /// Скрыть заголовок во вьювере
        Array_array& title_unvisible(bool f = true) {Array_basic::title_unvisible(f); return *this;}
        /// Выставить минимальное количество элементов в массиве. Default = максимальному
        Array_array& minItems(int64_t c) {Array_basic::minItem(c); return *this;}
        //// Составить описание параметра в формате JSON-схемы
        std::string to_string() const;
        /// Составить описание параметра в формате JSON-объекта
        json_t::object_t to_object() const;
    };

private:
    json_t::object_t* _json;                ///< Описание всех элементов
    json_t::array_t*  _json_oneOf;          ///< Описание для oneOf
    void*             _data_point;          ///< Указатель на начало данных структуры
    uint64_t          _size;                ///< Размер данных
    std::string       _name;                ///< Имя структуры
    std::string       _title;               ///< Заголовок структуры
    std::string       _options;             ///< Опции
    std::string       _comment;             ///< коментарий
    MessageType       _message_type;        ///< Тип сообщения
    double            _age;                 ///< Возраст сообщения
    bool              _no_age;              ///< Флаг отсутсвия возраста

    /// инициализация
    void init();

public:
    template<typename T>
    Schema(T* t)
        : _message_type(Filtered)
        , _age(2.0)
        , _no_age(false)
    {
        _data_point = t;
        _size       = sizeof(T);
        _name       = typeid_name_to_str(typeid(T).name());
        _title      = _name;
        init();
    }
    Schema(const Schema &s);
    ~Schema();

    /// Установить название описываемого сообщения, используется при отображении во вьювере
    Schema& title(std::string str)  {_title = str; return *this;}
    /**
     * @brief Установить тип сообщения
     * @param t MessageType (Filtered или NotFiltered) ( NotFiltered - не фильтруются в сеть, должны быть не высокочастотные)
     */
    Schema& type(MessageType t);
    /// Установить тип сообщения "Не фильтровать"
    Schema& not_filtered()  {return decimation_off();}
    /// Установить тип сообщения "Фильтровать"
    Schema& decimation_on() {return type(ipc::Filtered);}
    /// Установить тип сообщения "Не фильтровать"
    Schema& decimation_off() {return type(ipc::NotFiltered);}
    /**
     * @brief Установить максимальный возраст сообщения
     * (по-умолчанию = 2.0 )
     * @param age Возраст в секундах
     * @return Ссылка на объект
     */
    Schema& age(double age) {_age = age; _no_age = false; return *this;}
    /// Установить максимальный возраст сообщения
    Schema& age_on (double age = 2.0) {_age = age; _no_age = false; return *this;}
    /// Указать что нет возраста для этого сообщения
    Schema& age_off() {_no_age = true; return *this;}
    /// Указать что нет возраста для этого сообщения
    Schema& no_age() {return age_off();}
    /// Установить опции
    Schema& options(std::string str) {_options = str; return *this;}
    /// Установить комментарий
    Schema& comment(std::string str) {_comment = str; return *this;}

    /** Добавить параметр в зависимости от типа */
    Schema& add(const Schema  &item);
    Schema& add(const Atom_basic &item);

    Schema& add_oneOf(const Schema &item);

    /// Венуть указатель на данные, испозууется для определения offset у параметров
    int64_t     get_data_point() const {return (int64_t)_data_point;}
    /// Вернуть размер данныех в байтах
    size_t     get_size()       const {return _size;}
    /**
     * @brief Составить полное описание сообщения в формате JSON-схемы (с верхним уровнем содержащим название сообщения)
     * @return Описание сообщения
     */
    std::string to_string()       const;
    /**
     * @brief Составить описание сообщения в формате JSON-схемы (Описние только параметров, без верхнего уровня)
     * @return Описание сообщения
     */
    std::string to_string_short() const;
    json_t::object_t to_object() const;
    json_t::object_t to_object_short() const;

    json_t::object_t to_object_properties() const;
};

}
