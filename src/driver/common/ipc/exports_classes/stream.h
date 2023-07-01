#pragma once

#include "common.h"

namespace ipc {

/**
 * @brief класс для записи в лог в стиле cout (ipc::Stream() << "text" << double << int)
 */
class CLASS_EXPORT Stream {
public:
    Stream() {}
    Stream(const Stream &s);
    Stream& operator =(const Stream &s);

    template<class T>
    Stream &operator <<(const T& arg) {ss << arg; return *this;}

    template<class T>
    Stream & append(const T& arg, size_t min_len = 0)
    {
        ss << arg;
        if(min_len > 0)
        {
            std::stringstream tmp;
            tmp << arg;
            std::string tmp_str = tmp.str();
            for(size_t i = tmp_str.length(); i < min_len; ++i)
                ss << " ";
        }
        return *this;
    }
    /**
     * @brief Вернуть составленну строку
     * @return текстовая строка
     */
    std::string str() const;
    operator std::string() const;
private:
    std::stringstream ss;
};

}
