#pragma once 

class LyricUpdateTask: public ITask{
private:
    std::wstring m_name_space;
    std::wstring m_last_lyric;
public:
    LyricUpdateTask(const std::wstring &name_space):
        ITask(L"LyricUpdateTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            auto result = data->resource.GetLyricPath(data->player->GetTrack());
            if(result != m_last_lyric){
                m_last_lyric = result;
                data->lyric.Load(result);
            }
        }
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};