#include "out.h"

ipc::Out::Out(char *c)
{
    _txt = std::string(c);
}

ipc::Out::Out(const char *c)
{
    _txt = std::string(c);
}

ipc::Out::Out(std::string txt)
{
    _txt = txt;
}

ipc::Out &ipc::Out::operator =(const ipc::Out &o)
{
    _txt = o._txt;
    _arg_list = o._arg_list;
    return *this;
}

ipc::Out &ipc::Out::arg(const double &value, int precision)
{
    std::stringstream tmp_s;
    tmp_s.setf(std::ios::fixed);
    tmp_s.precision(precision);
    tmp_s << value;
    _arg_list.push_back(tmp_s.str());
    return *this;
}

ipc::Out &ipc::Out::arg(const float &value, int precision)
{
    return arg(static_cast<double>(value), precision);
}

std::string ipc::Out::str() const
{
    std::string res_text = _txt;
    if(!_arg_list.empty())
    {
        uint16_t count = 1;
        for(std::vector<std::string>::const_iterator it=_arg_list.begin(); it!=_arg_list.end(); ++it)
        {
            std::stringstream t;
            t << "%" << count;
            count++;
            std::string index = t.str();
            size_t found = res_text.find(index);
            while(found != std::string::npos)
            {
                res_text.replace(found, index.size(), *it);
                found = res_text.find(index, found);
            }
        }
    }

    return res_text;
}

ipc::Out::operator std::string() const
{
    return str();
}
