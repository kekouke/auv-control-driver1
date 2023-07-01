#pragma once

#include "common.h"

namespace ipc {

/**
 * @brief Класс сообщения бортового журнала
 */
class CLASS_EXPORT Blackbox
{
public:
    Blackbox();
    Blackbox(const Blackbox &b);
    ~Blackbox();

    Blackbox& operator =(Blackbox &b);
    /**
     * @brief Получить текст сообщения
     * @return Текст
     */
    std::string     text();
    /**
     * @brief Получить имя программы опубликовавшей сообшение
     * @return Имя программы
     */
    std::string     program();
    /**
     * @brief Получить имя Хоста опубликовавшей сообшение
     * @return Имя хоста
     */
    std::string     hostname();
    /**
     * @brief Получить тип сообщения
     * @return Тип
     */
    State           type();
private:
    friend class Core;
    ipc_basic_blackbox_t *basic;
};

}
