#pragma once

class PlayerTask: public ITask{
private:
    std::wstring m_name_space;
    DWORD m_last_time;
public:
    PlayerTask(const std::wstring &name_space):
        ITask(L"PlayerTask"),
        m_name_space(name_space),
        m_last_time(0)
    {
    }
    ITask *Next(DWORD time) override{
        ITask *next = m_next;
        m_next = nullptr;

        if(time < m_last_time+500){
            return next;
        }
        m_last_time = time;

        if(CoreData *data = Core::GetData(m_name_space)){
// RmLog(LOG_WARNING, L"haha");
            if(data->player->Update(time)){
                ITask *player_callback = Core::GetTaskCenter()->GetTask(m_name_space, L"PlayerCallBackTask");
                ITask *cover_update    = Core::GetTaskCenter()->GetTask(m_name_space, L"CoverUpdateTask");
                ITask *lyric_update    = Core::GetTaskCenter()->GetTask(m_name_space, L"LyricUpdateTask");
                this->InsertBack(player_callback);
                this->InsertBack(cover_update);
                this->InsertBack(lyric_update);
            }
        }
        
        next = m_next;
        m_next = nullptr;
        return next;
    }
};