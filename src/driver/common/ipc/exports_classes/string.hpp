#pragma once

#include "common.h"

namespace ipc {

#pragma pack(push,1)
/**
 * @brief базовый класс для строк
 */
template<int size_>
class String
{
public:
    String(){clear();}

    String(const char *str)
    {
        int copy_size = (strlen(str) > (sizeof(d))) ? (sizeof(d)) : strlen(str);    // максимальная длина данных
        clear();                                                                    // обнулить все данные
        memcpy(d, str, copy_size);                                                  // скопировать необходимые данные
    }
    String& operator =(const char *str)
    {
        int copy_size = (strlen(str) > (sizeof(d))) ? (sizeof(d)) : strlen(str);
        clear();
        memcpy(d, str, copy_size);
        return *this;
    }
    String& operator =(String<size_> txt)
    {
        int copy_size = size_;
        clear();
        memcpy(d, txt.d, copy_size);
        return *this;
    }
    String& operator =(char i)
    {
        clear();
        if(size_ > 0) d[0] = i;
        return *this;
    }
    String& operator =(std::string str)
    {
        int copy_size = (str.size() > (sizeof(d))) ? (sizeof(d)) : str.size();
        clear();
        memcpy(d, str.c_str(), copy_size);
        return *this;
    }

    operator std::string() const {
        return to_std_string();
    }

    void        clear(){memset(d, 0, sizeof(d));}
    size_t      size() {return sizeof(d);}
    void       *data() {return d;}

    std::string to_std_string() const {
        return std::string(d, size_).c_str();
    }
private:
    char d[size_];
};

#pragma pack(pop)

}
