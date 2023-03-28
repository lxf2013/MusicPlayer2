#pragma once

class StartPlayerTask: public ITask{
private:
    std::wstring m_name_space;
public:
    StartPlayerTask(const std::wstring &name_space):
        ITask(L"StartPlayerTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        Core::GetTaskCenter()->RunTask(m_name_space, L"PlayerTask");
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};