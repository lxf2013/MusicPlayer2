#pragma once

class StopPlayerTask: public ITask{
private:
    std::wstring m_name_space;
public:
    StopPlayerTask(const std::wstring &name_space):
        ITask(L"StopPlayerTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        Core::GetTaskCenter()->StopTask(m_name_space, L"PlayerTask");
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};