#pragma once

#include "ipc.h"
#include "messages.h"
#include "motion.h"

template <typename LogType>
class Sender
{
public:
    Sender(ipc::Core*);
    LogType& GetData();
    ~Sender();
    void Send() const;

    void Initialize();

private:
    ipc::Sender<LogType> *sender;
    ipc::Core* core;
};

template<typename LogType>
Sender<LogType>::Sender(ipc::Core* core) {
    sender = new ipc::Sender<LogType>(*core);
    this->core = core;
}

template<typename LogType>
Sender<LogType>::~Sender()
{
    delete sender;
    sender = nullptr;
    core = nullptr;
}

template<typename LogType>
LogType& Sender<LogType>::GetData()
{
    return sender->_;
}

template<typename LogType>
void Sender<LogType>::Send() const
{
    sender->send();
}

