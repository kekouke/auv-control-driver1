#pragma once

#include "common.h"
#include <sys/time.h>
#include <string>

namespace ipc {

/// Класс для работы со временем
class CLASS_EXPORT DateTime
{
public:
    enum ClockID {
        Clock_Realtime = 0,         ///< Системное время
        Clock_Monotonic             ///< Время без скачков
    };

protected:
    double _time_sec;               ///< Время в секундах
public:
    /// Текущее время
    DateTime(ClockID clock_id = Clock_Realtime);
    DateTime(double time);
    DateTime(const DateTime& d);

    double TimeDouble() const;

    /// Венуть количество микросекунд
    uint64_t Microseconds() const;
    /// Венуть количество милисекунд
    uint64_t Miliseconds() const;
    /// Вернуть количество секунд
    uint32_t Seconds()  const;
    /// Вернуть количество минут
    uint32_t Minutes()  const;
    /// Вернуть количество часов
    uint32_t Hours()    const;
    /// Вернуть количество дней
    uint32_t Days()     const;

    /// Вернуть количество остаток Микросекунд в Милисекунде
    int32_t MicrosecondsOfMilisecond() const;
    /// Вернуть количество остаток Милисекунд в Секунде
    int32_t MilisecondsOfSecond() const;
    /// Вернуть количество остаток Секунд в Минуте
    int32_t SecondsOfMinute() const;
    /// Вернуть количество остаток Минут в Часе
    int32_t MinutesOfHour  () const;
    /// Вернуть количество остаток Часов в Дне
    int32_t HoursOfDay     () const;

    // Добавить значения
    DateTime& AddMicroseconds(const uint32_t val);
    DateTime& AddMiliseconds (const uint32_t val);
    DateTime& AddSeconds     (const uint32_t val);
    DateTime& AddMinutes     (const uint32_t val);
    DateTime& AddHours       (const uint32_t val);
    DateTime& AddDays        (const uint32_t val);

    // Удалить значения
    DateTime& RremoveMicroseconds(const uint32_t val);
    DateTime& RremoveMiliseconds (const uint32_t val);
    DateTime& RemoveSeconds      (const uint32_t val);
    DateTime& RemoveMinutes      (const uint32_t val);
    DateTime& RemoveHours        (const uint32_t val);
    DateTime& RemoveDays         (const uint32_t val);

    DateTime operator=      (const double right);
    DateTime operator=      (const DateTime right);
    DateTime operator+=     (const DateTime right);
    DateTime operator-=     (const DateTime right);
    DateTime operator+      (const DateTime right) const;
    DateTime operator-      (const DateTime right) const;
    DateTime operator+      (const double   right) const;
    DateTime operator-      (const double   right) const;
    bool     operator>      (const DateTime right) const;
    bool     operator<      (const DateTime right) const;
    bool     operator>=     (const DateTime right) const;
    bool     operator<=     (const DateTime right) const;
    bool     operator==     (const DateTime right) const;
    bool     operator>      (const double right) const;
    bool     operator<      (const double right) const;
    bool     operator>=     (const double right) const;
    bool     operator<=     (const double right) const;
    bool     operator==     (const double right) const;

    /// Преобразовать время в строку по формату
    std::string to_string(std::string format = "%Y-%m-%d_%H-%M-%S", bool localtime = true) const;
    /// Преобразовать время в строку по формату с учетом часового пояса
    std::string to_string_localtime(std::string format = "%Y-%m-%d_%H-%M-%S") const;
    /// Преобразовать время в строку по формату по Гринвичу
    std::string to_string_utc(std::string format = "%Y-%m-%d_%H-%M-%S") const;
    /// Преобразовать милисекунды (доли секунды) в строку (001)
    std::string Miliseconds_to_string() const;

    operator double() const;

protected:
    void check_valid();

    void get_clock_realtime();
    void get_clock_monotonic();

    std::string tm_to_string(tm* t, std::string format) const;
};

class CLASS_EXPORT DateTimeRealTime : public DateTime {
public:
    DateTimeRealTime() : DateTime(Clock_Realtime) {}
};

class CLASS_EXPORT DateTimeMonotonic : public DateTime {
public:
    DateTimeMonotonic() : DateTime(Clock_Monotonic) {}
    DateTimeMonotonic(const DateTime& d) : DateTime(d) {}

    DateTimeMonotonic operator= (const double right);

private:
    std::string to_string(std::string, bool) const {return "";};
    std::string to_string_localtime(std::string) const {return "";};

};

}
