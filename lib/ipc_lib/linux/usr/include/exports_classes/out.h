#pragma once
#include "common.h"

namespace ipc {

/**
 * @brief класс для записи в лог в стиле QString (ipc::Out("%1 - %2 - %3").arg("text").arg(double).arg(int)) Максимум до 9
 */
class CLASS_EXPORT Out {
public:
    Out() {}
    Out(char* c);
    Out(const char* c);
    Out(std::string txt);
    Out& operator =(const Out& o);

    template<class T>
    Out& arg(const T& value){
        std::stringstream tmp_s;
        tmp_s.setf(std::ios::fixed);
        tmp_s << value;
        _arg_list.push_back(tmp_s.str());
        return *this;
    }

    Out& arg(const double& value, int precision);
    Out& arg(const  float& value, int precision);
    /**
     * @brief Вернуть составленну строку
     * @return текстовая строка
     */
    std::string str() const;
    operator std::string() const;
private:
    std::string _txt;
    std::vector<std::string> _arg_list;
};

}
