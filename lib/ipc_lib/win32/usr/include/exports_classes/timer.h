#pragma once

#include "common.h"
#include "core.h"

namespace ipc {

/**
 * @brief Класс таймера
 */
class CLASS_EXPORT Timer
{
private:
    ipc_basic_timer_t *basic;
    friend class Core;
    Core &core;
    // запрещено копирование
    Timer(const Timer &t) :core(t.core) {}
    void operator =(const Timer &) {}
public:
    /**
     * @brief Конструктор. Регистрация в системе
     * @param core            Ядро
     * @param realtime_always Тип таймера (true - realtime, не влияет на ускорение времени)
     */
    Timer(Core &core, bool realtime_always = false);
    /**
     * @brief Конструктор. Регистрация в системе
     * @param core            Ядро
     * @param interval        Интревал таймера в секундах
     * @param realtime_always Тип таймера (true - realtime, не влияет на ускорение времени)
     */
    Timer(Core &core, double interval, bool realtime_always = false);
    ~Timer();
    /**
     * @brief Проверка срабатывания таймера
     * @return true если таймер сработал
     */
    bool received() {return core.received(*this);}

    /**
     * @brief Запуск таймера
     * @param interval  Интервал таймера в секундах
     */
    void    start(double interval);
    /**
     * @brief Запуск таймера с задержкой
     * @param interval  Интервал таймера в секундах
     * @param delay     Задержка в секундах
     */
    void    start_delay(double interval, double delay);
    /**
     * @brief Однократное срабатываение таймера
     * @param interval  Интервал таймера в секундах
     */
    void    start_once(double interval);
    /**
     * @brief Перезапуск таймера
     * @param interval  Интервал таймера в секундах
     */
    void    restart(double interval);
    /**
     * @brief Перезапуск таймера с задержкой
     * @param interval  Интервал таймера в секундах
     * @param delay     Задержка в секундах
     */
    void    restart_delay(double interval, double delay);
    /**
     * @brief Остановить таймер
     */
    void    stop();
    /**
     * @brief Проверка запущен ли таймер
     * @return true если тамер активен
     */
    bool    is_started();
};

}
