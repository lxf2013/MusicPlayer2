#include "TaskCenter.h"
#include "../common.h"

void TaskCenter::Run(const std::wstring &name_space){
    auto tasks = m_runing.find(name_space);
    if(tasks == m_runing.end()){
        return ;
    }
    DWORD time = GetTickCount();
    for(auto it: tasks->second){
// std::wstring msg;
// int cnt = 0;
        for(ITask *task=it; task; task=task->Next(time)){
// ++cnt;
// msg += task->GetId();
// msg += L" -> ";
        }
// msg += L"nullptr";
// if(cnt > 1){
// RmLog(LOG_WARNING, msg.c_str());
// }
    }
}
ITask *TaskCenter::GetTask(const std::wstring &name_space, const std::wstring &id){
    auto it = m_tasks.find(name_space);
    if(it == m_tasks.end()){
        return nullptr;
    }
    for(auto it1: it->second){
        if(it1->GetId() == id){
            return it1;
        }
    }
    return nullptr;
}
ITask *TaskCenter::AllocTask(const std::wstring &name_space, const std::wstring &id, std::function<ITask *()> func){
    if(auto task = GetTask(name_space, id)){
        ++task->m_ref;
        return task;
    }
    
    auto task = func();
    m_tasks[name_space].push_back(task);
    ++task->m_ref;
    return task;
}
void TaskCenter::FreeTask(const std::wstring &name_space, const std::wstring &id, Measure *measure){
    auto it = m_tasks.find(name_space);
    if(it == m_tasks.end()){
        return;
    }
    for(auto it1=it->second.begin(); it1!=it->second.end(); ++it1){
        if((*it1)->GetId() != id){
            continue;
        }
        if(--(*it1)->m_ref > 0){
            if(auto ptr = dynamic_cast<ICallBackTask *>(*it1)){
                ptr->DelCallBack(measure);
            }
            break;
        }
        auto it2 = m_runing.find(name_space);
        if(it2 != m_runing.end()){
            for(auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3){
                if((*it3)->GetId() == id){
                    it2->second.erase(it3);
                    break;
                }
            }
            if(it2->second.empty()){
                m_runing.erase(name_space);
            }
        }
        MemoryCount::FreeTask();
        delete *it1;
        it->second.erase(it1);
        if(it->second.empty()){
            m_tasks.erase(name_space);
        }
        break;
    }
}
bool TaskCenter::RunTask(const std::wstring &name_space, const std::wstring &id){
    if(auto it = GetTask(name_space, id)){
        auto &it2 = m_runing[name_space];
        for(auto it3: it2){
            if(it3->GetId() == id){
                return true;
            }
        }
        it2.push_back(it);
        return true;
    }
    return false;
}
bool TaskCenter::StopTask(const std::wstring &name_space, const std::wstring &id){
    if(auto it = GetTask(name_space, id)){
        auto &it1 = m_runing[name_space];
        for(auto it2=it1.begin(); it2!=it1.end(); ++it2){
            if((*it2)->GetId() == id){
                it1.erase(it2);
                break;
            }
        }
        return true;
    }
    return false;
}