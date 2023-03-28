#pragma once

class LyricTask: public ITask{
private:
    std::wstring m_name_space;
    DWORD m_last_time = 0;
public:
    LyricTask(const std::wstring name_space):
        ITask(L"LyricTask"),
        m_name_space(name_space),
        m_last_time(0)
    {
    }
    ITask * Next(DWORD time) override{        
        ITask *next = m_next;
        m_next = nullptr;

        if(time < m_last_time+100){
            return next;
        }
        m_last_time = time;

        if(CoreData *data = Core::GetData(m_name_space)){
            if(data->lyric.Update(time)){
                ITask *lyric_callback = Core::GetTaskCenter()->GetTask(m_name_space, L"LyricCallBackTask");
                this->InsertBack(lyric_callback);                
            }
        }

        next = m_next;
        m_next = nullptr;
        return next;
    }
};