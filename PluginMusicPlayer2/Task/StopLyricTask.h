#pragma once

class StopLyricTask: public ITask{
private:
    std::wstring m_name_space;
public:
    StopLyricTask(const std::wstring &name_space):
        ITask(L"StopLyricTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            if(data->resource.IsDefault(data->player->GetTrack()) == false){
                data->lyric.Stop(time - 500);
                Core::GetTaskCenter()->StopTask(m_name_space, L"LyricTask");
            }
        }
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};