#pragma once

class AudioTask: public ITask{
private:
    std::wstring m_name_space;
    int          m_last_audio;
    int          m_run_status;
    int          m_run_player;
    int          m_run_lyric;
    unsigned int m_timeout_cnt;
public:
    AudioTask(const std::wstring &name_space):
        ITask(L"AudioTask"),
        m_name_space(name_space),
        m_last_audio(-1),
        m_run_status(-1),
        m_run_player(-1),
        m_run_lyric(-1),
        m_timeout_cnt(0)
    {
    }
    ITask *Next(DWORD time) override{
        if(CoreData *data = Core::GetData(m_name_space)){
            bool flag = data->audio->Update(time);
            if(flag){
                m_timeout_cnt=0;
                if(m_run_status != 1){
                    ITask *status = Core::GetTaskCenter()->GetTask(m_name_space, L"PlayStatusCallBackTask");
                    this->InsertBack(status);
                    m_run_status = 1;
                }
                // if(m_run_player != 1){
                //     ITask *start_player = Core::GetTaskCenter()->GetTask(m_name_space, L"StartPlayerTask");
                //     this->InsertBack(start_player);
                //     m_run_player = 1;
                // }
                if(m_run_lyric != 1){
                    ITask *start_lyric  = Core::GetTaskCenter()->GetTask(m_name_space, L"StartLyricTask");
                    this->InsertBack(start_lyric);
                    m_run_lyric = 1;
                }
            }else{
                m_timeout_cnt? ++m_timeout_cnt: m_timeout_cnt+=5;
                if(m_run_status != 0){
                    ITask *status = Core::GetTaskCenter()->GetTask(m_name_space, L"StopStatusCallBackTask");
                    this->InsertBack(status);
                    m_run_status = 0;
                }
                // if(m_run_player != 0){
                //     ITask *stop_player = Core::GetTaskCenter()->GetTask(m_name_space, L"StopPlayerTask");
                //     this->InsertBack(stop_player);
                //     m_run_player = 0;
                // }
                if(m_run_lyric != 0 && m_timeout_cnt * 100 >= data->timeout){
                    ITask *stop_lyric  = Core::GetTaskCenter()->GetTask(m_name_space, L"StopLyricTask");
                    this->InsertBack(stop_lyric);
                    m_run_lyric = 0;
                }
            }
        }

        ITask *next = m_next;
        m_next = nullptr;
        return next;
    }
};