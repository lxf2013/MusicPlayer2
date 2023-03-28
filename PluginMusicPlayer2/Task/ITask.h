#pragma once

#include <list>
#include "../ThirdPart/RainmeterAPI.h"
#include "../PluginMusicPlayer2.h"

class TaskCenter;

class ITask{
protected:
    std::wstring m_id;
    int m_ref = 0;
    ITask *m_next = nullptr;
    friend class TaskCenter;
public:
    ITask(const std::wstring &id):
        m_id(id),
        m_ref(0),
        m_next(nullptr){
    }
    virtual ~ITask() = default;
    virtual const std::wstring &GetId(){
        return m_id;
    }
    virtual ITask * Next(DWORD time) = 0;
    // void ClearBack(){
    //     m_next = nullptr;
    // }
    void InsertBack(ITask *task){
        if(task){
            task->m_next = this->m_next;
            this->m_next = task;
        }
    }
};

class ICallBackTask: public ITask{
protected:
    std::list<Measure *> m_call_back;
public:
    ICallBackTask(const std::wstring &id): ITask(id){
    }
    virtual bool AddCallBack(Measure *measure) = 0;
    virtual bool DelCallBack(Measure *measure){
        for(auto it=m_call_back.begin(); it!=m_call_back.end(); ++it){
            if(*it == measure){
                m_call_back.erase(it);
                return true;
            }
        }
        return false;
    }
    ITask *Next(DWORD time) override{
        ITask *next = m_next;
        m_next = nullptr;

        for(auto it: m_call_back){
            RmExecute(RmGetSkin(it->rm), it->change_action.c_str());
        }
        return next;
    }
};