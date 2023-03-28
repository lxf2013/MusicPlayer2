#pragma once

class StopStatusCallBackTask: public ICallBackTask{
private:
    std::wstring m_name_space;
public:
    StopStatusCallBackTask(const std::wstring &name_space):
        ICallBackTask(L"StopStatusCallBackTask"),
        m_name_space(name_space)
    {
    }
    bool AddCallBack(Measure *measure) override{
        if(measure->stop_action.empty()){
            return false;
        }
        switch(measure->type){
            case Measure::MEASURE_STATUS:
                for(auto it: m_call_back){
                    if(it == measure){
                        return false;
                    }
                }
                m_call_back.push_back(measure);
                break;
        }
        return true;
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            for(auto it: m_call_back){
                RmExecute(RmGetSkin(it->rm), it->stop_action.c_str());
                // if(data->audio->GetStatus()){
                //     if(it->play_action.size()){
                //         RmExecute(RmGetSkin(it->rm), it->play_action.c_str());
                //     }
                // }else{
                //     if(it->stop_action.size()){
                //         RmExecute(RmGetSkin(it->rm), it->stop_action.c_str());
                //     }
                // }
            }
        }

        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};