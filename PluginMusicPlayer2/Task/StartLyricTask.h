#pragma once

class StartLyricTask: public ITask{
private:
    std::wstring m_name_space;
public:
    StartLyricTask(const std::wstring &name_space):
        ITask(L"StartLyricTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            data->lyric.Start(time);
        }

        Core::GetTaskCenter()->RunTask(m_name_space, L"LyricTask");
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};