#pragma once 

class CoverUpdateTask: public ITask{
private:
    std::wstring m_name_space;
    std::wstring m_last_cover;
public:
    CoverUpdateTask(const std::wstring &name_space):
        ITask(L"CoverUpdateTask"),
        m_name_space(name_space)
    {
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            auto result = data->resource.GetCoverPath(data->player->GetTrack());
            if(result != m_last_cover){
                m_last_cover = result;
                ITask *cover_callback = Core::GetTaskCenter()->GetTask(m_name_space, L"CoverCallBackTask");
                this->InsertBack(cover_callback);
            }
        }
        
        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};