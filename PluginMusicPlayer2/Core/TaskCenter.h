#pragma once

#include <functional>
#include <windows.h>
#include <map>
#include "../Task/ITask.h"

class TaskCenter{
private:
    std::map<std::wstring, std::list<ITask *>> m_tasks;
    std::map<std::wstring, std::list<ITask *>> m_runing;
public:
    void Run(const std::wstring &name_space);
    ITask *GetTask(const std::wstring &name_space, const std::wstring &id);
    ITask *AllocTask(const std::wstring &name_space, const std::wstring &id, std::function<ITask *()> func);
    void FreeTask(const std::wstring &name_space, const std::wstring &id, Measure *measure);
    bool RunTask(const std::wstring &name_space, const std::wstring &id);
    bool StopTask(const std::wstring &name_space, const std::wstring &id);
};