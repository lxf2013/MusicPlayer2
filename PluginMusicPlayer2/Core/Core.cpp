#include "Core.h"
#include "../ThirdPart/RainmeterAPI.h"
#include "../Player/BaiduMusic.h"
#include "../Player/CloudMusic.h"
#include "../Player/KgMusic.h"
#include "../Player/KwMusic.h"
#include "../Player/QQMusic.h"

#include "../Task/CoverCallBackTask.h"
#include "../Task/CoverUpdateTask.h"
#include "../Task/LyricCallBackTask.h"
#include "../Task/LyricTask.h"
#include "../Task/LyricUpdateTask.h"
#include "../Task/PlayerCallBackTask.h"
#include "../Task/PlayerTask.h"
#include "../Task/AudioTask.h"
#include "../Task/StartPlayerTask.h"
#include "../Task/StopPlayerTask.h"
#include "../Task/StartLyricTask.h"
#include "../Task/StopLyricTask.h"
#include "../Task/PlayStatusCallBackTask.h"
#include "../Task/StopStatusCallBackTask.h"


std::map<std::wstring, CoreData> Core::m_name_space;
TaskCenter Core::m_task_center;

std::wstring Core::GetNameSpace(const std::wstring &path){
    std::vector<std::wstring> dirs;
    std::wstring temp;
    for(auto ch: path){
        if(ch != '\\'){
            temp += ch;
            continue;
        }
        
        if(temp == L".."){
            if(dirs.empty()){
                // return Result(L"", Result::FAILD, path + L" path error");
                return L"";
            }
            dirs.pop_back();
        } else if(temp == L"."){
        } else if(temp.size()){
            dirs.push_back(temp);
        }
        temp.clear();
    }
    if(temp.size()){
        dirs.push_back(temp);
    }
    temp.clear();
    for(auto &it: dirs){
        temp += L"\\";
        temp += std::move(it);
    }
    return temp;
}

std::wstring Core::GetNameSpace(void *rm){
    std::wstring path = L"\\";
    path += RmGetSkinName(rm);
    path += L"\\";
    path += RmGetMeasureName(rm);
    return path;
}



inline CoreData *Core::GetData(const std::wstring &name_space){
    auto it = m_name_space.find(name_space);
    if(it == m_name_space.end()){
        return nullptr;
    }
    return &it->second;
}

std::wstring Core::AllocData(Measure *measure){
    IPlayer *player = nullptr;
    LPCWSTR str = RmReadString(measure->rm, L"PlayerName", L"????");
    if (!_wcsicmp(L"KwMusic", str)){
        player = KwMusic::Create();
    }else if (!_wcsicmp(L"KgMusic", str)){
        player = KgMusic::Create();
    }else if (!_wcsicmp(L"QQMusic", str)){
        player = QQMusic::Create();
    }else if (!_wcsicmp(L"BaiduMusic", str)){
        player = BaiduMusic::Create();
    }else if (!_wcsicmp(L"CloudMusic", str)){
        player = CloudMusic::Create();
    }else if (!_wcsicmp(L"????", str)){
        RmLog(measure->rm, LOG_WARNING, L"not find PlayerName");
        return L"";
    }else{
        std::wstring error = L"invalid PlayerName: ";
        error += str;
        RmLog(measure->rm, LOG_WARNING, error.c_str());
        return L"";
    }

    std::wstring name_space = GetNameSpace(measure->rm);
    auto &it = m_name_space[name_space];
    ++it.ref;
    if(it.player){
        it.player->RemoveInstantce();
    }
    it.player = player;

    if(it.audio == nullptr){
        it.audio = Audio::Create();
    }

    std::wstring default_cover = RmPathToAbsolute(measure->rm, RmReadString(measure->rm, L"DefaultCover", L""));
    std::wstring default_lyric = RmPathToAbsolute(measure->rm, RmReadString(measure->rm, L"DefaultLyric", L""));
    std::wstring path  = RmPathToAbsolute(measure->rm, RmReadString(measure->rm, L"DownloadPath", L"download"));
    double similarity = RmReadDouble(measure->rm, L"Similarity", 1.4);
    int     capacity  = RmReadInt(measure->rm, L"Capacity", 100);
    it.resource.Reset(default_cover, default_lyric, path, similarity, capacity);
    
    int offset = RmReadInt(measure->rm, L"LyricOffset", 0);
    it.lyric.Init(offset);
    it.lyric.Load(default_lyric.c_str());

    it.timeout = unsigned int(RmReadInt(measure->rm, L"LyricTimeout", 10000));
    return name_space;
}

void Core::FreeData(Measure *measure){
    if(measure->type == Measure::MEASURE_NONE){
        return;
    }
    if(measure->type != Measure::MEASURE_MANAGER){
        m_task_center.FreeTask(measure->name_space, L"PlayerTask", measure);
        m_task_center.FreeTask(measure->name_space, L"StopPlayerTask", measure);
        m_task_center.FreeTask(measure->name_space, L"StartPlayerTask", measure);
        m_task_center.FreeTask(measure->name_space, L"AudioTask", measure);
        switch(measure->type){
            case Measure::MEASURE_TITLE:
            case Measure::MEASURE_ARTIST:
            case Measure::MEASURE_TRACK:
                m_task_center.FreeTask(measure->name_space, L"PlayerCallBackTask", measure);
                break;
            case Measure::MEASURE_STATUS:
                m_task_center.FreeTask(measure->name_space, L"PlayStatusCallBackTask", measure);
                m_task_center.FreeTask(measure->name_space, L"StopStatusCallBackTask", measure);
                break;
            case Measure::MEASURE_COVER:
                m_task_center.FreeTask(measure->name_space, L"CoverUpdateTask", measure);
                m_task_center.FreeTask(measure->name_space, L"CoverCallBackTask", measure);
                break;
            case Measure::MEASURE_LRC_6:
            case Measure::MEASURE_LRC_5:
            case Measure::MEASURE_LRC_4:
            case Measure::MEASURE_LRC_3:
            case Measure::MEASURE_LRC_2:
            case Measure::MEASURE_LRC_1:
            case Measure::MEASURE_LRC0:
            case Measure::MEASURE_LRC1:
            case Measure::MEASURE_LRC2:
            case Measure::MEASURE_LRC3:
            case Measure::MEASURE_LRC4:
            case Measure::MEASURE_LRC5:
            case Measure::MEASURE_LRC6:
            case Measure::MEASURE_LRC7:
            case Measure::MEASURE_LRC8:
            case Measure::MEASURE_LRC9:
            case Measure::MEASURE_TLRC_6:
            case Measure::MEASURE_TLRC_5:
            case Measure::MEASURE_TLRC_4:
            case Measure::MEASURE_TLRC_3:
            case Measure::MEASURE_TLRC_2:
            case Measure::MEASURE_TLRC_1:
            case Measure::MEASURE_TLRC0:
            case Measure::MEASURE_TLRC1:
            case Measure::MEASURE_TLRC2:
            case Measure::MEASURE_TLRC3:
            case Measure::MEASURE_TLRC4:
            case Measure::MEASURE_TLRC5:
            case Measure::MEASURE_TLRC6:
            case Measure::MEASURE_TLRC7:
            case Measure::MEASURE_TLRC8:
            case Measure::MEASURE_TLRC9:
                m_task_center.FreeTask(measure->name_space, L"LyricTask", measure);
                m_task_center.FreeTask(measure->name_space, L"LyricUpdateTask", measure);
                m_task_center.FreeTask(measure->name_space, L"LyricCallBackTask", measure);
                m_task_center.FreeTask(measure->name_space, L"StopLyricTask", measure);
                m_task_center.FreeTask(measure->name_space, L"StartLyricTask", measure);
                break;

        }
    }else{
        auto it = m_name_space.find(measure->name_space);
        if(it != m_name_space.end() && --it->second.ref < 1){
            if(it->second.player){
                it->second.player->RemoveInstantce();
                it->second.player = nullptr;
            }
            if(it->second.audio){
                it->second.audio->RemoveInstance();
                it->second.audio = nullptr;
            }
            m_name_space.erase(it);
        }
    }
}
std::wstring Core::FlushData(Measure *measure){
    if(measure->type == Measure::MEASURE_NONE || measure->type == Measure::MEASURE_MANAGER){
        return L"";
    }
    LPCWSTR str = RmReadString(measure->rm, L"Manager", L"????");
    if (!_wcsicmp(L"????", str)){
        RmLog(measure->rm, LOG_ERROR, L"not find Manager");
        return L"";
    }
    
    std::wstring path;
    if(str[0] != '\\'){
        path = L"\\";
        path += RmGetSkinName(measure->rm);
        path += L"\\";
    }
    path += str;
    path = GetNameSpace(path);

    if(m_name_space.count(path) == 0){
        std::wstring error = L"not find Manager: ";
        error += str;
        RmLog(measure->rm, LOG_WARNING, error.c_str());
        // return L"";
    }

    ITask *task = m_task_center.AllocTask(path, L"PlayerTask", [path]() -> ITask * {
        MemoryCount::AllocTask();
        return new PlayerTask(path);
    });
    m_task_center.RunTask(path, L"PlayerTask");

    m_task_center.AllocTask(path, L"StopPlayerTask", [path]() -> ITask* {
        MemoryCount::AllocTask();
        return new StopPlayerTask(path);
    });
    m_task_center.AllocTask(path, L"StartPlayerTask", [path]() -> ITask* {
        MemoryCount::AllocTask();
        return new StartPlayerTask(path);
    });

    task = m_task_center.AllocTask(path, L"AudioTask", [path]() -> ITask * {
        MemoryCount::AllocTask();
        return new AudioTask(path);
    });
    m_task_center.RunTask(path, L"AudioTask");

    switch(measure->type){
        case Measure::MEASURE_TITLE:
        case Measure::MEASURE_ARTIST:
        case Measure::MEASURE_TRACK:{
            ITask* task = m_task_center.AllocTask(path, L"PlayerCallBackTask", []() -> ITask* {
                MemoryCount::AllocTask();
                return new PlayerCallBackTask();
            });
            if(auto ptr = dynamic_cast<ICallBackTask *>(task)){
                ptr->AddCallBack(measure);
            }
        }break;
        case Measure::MEASURE_STATUS:{
            ITask* task = m_task_center.AllocTask(path, L"PlayStatusCallBackTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new PlayStatusCallBackTask(path);
            });
            if(auto ptr = dynamic_cast<ICallBackTask *>(task)){
                ptr->AddCallBack(measure);
            }
            
            task = m_task_center.AllocTask(path, L"StopStatusCallBackTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new StopStatusCallBackTask(path);
            });
            if(auto ptr = dynamic_cast<ICallBackTask *>(task)){
                ptr->AddCallBack(measure);
            }
        }break;
        case Measure::MEASURE_COVER:{
            m_task_center.AllocTask(path, L"CoverUpdateTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new CoverUpdateTask(path);
            });
            ITask* task = m_task_center.AllocTask(path, L"CoverCallBackTask", []() -> ITask* {
                MemoryCount::AllocTask();
                return new CoverCallBackTask();
            });
            if(auto ptr = dynamic_cast<ICallBackTask *>(task)){
                ptr->AddCallBack(measure);
            }
        }break;
        case Measure::MEASURE_LRC_6:
        case Measure::MEASURE_LRC_5:
        case Measure::MEASURE_LRC_4:
        case Measure::MEASURE_LRC_3:
        case Measure::MEASURE_LRC_2:
        case Measure::MEASURE_LRC_1:
        case Measure::MEASURE_LRC0:
        case Measure::MEASURE_LRC1:
        case Measure::MEASURE_LRC2:
        case Measure::MEASURE_LRC3:
        case Measure::MEASURE_LRC4:
        case Measure::MEASURE_LRC5:
        case Measure::MEASURE_LRC6:
        case Measure::MEASURE_LRC7:
        case Measure::MEASURE_LRC8:
        case Measure::MEASURE_LRC9:
        case Measure::MEASURE_TLRC_6:
        case Measure::MEASURE_TLRC_5:
        case Measure::MEASURE_TLRC_4:
        case Measure::MEASURE_TLRC_3:
        case Measure::MEASURE_TLRC_2:
        case Measure::MEASURE_TLRC_1:
        case Measure::MEASURE_TLRC0:
        case Measure::MEASURE_TLRC1:
        case Measure::MEASURE_TLRC2:
        case Measure::MEASURE_TLRC3:
        case Measure::MEASURE_TLRC4:
        case Measure::MEASURE_TLRC5:
        case Measure::MEASURE_TLRC6:
        case Measure::MEASURE_TLRC7:
        case Measure::MEASURE_TLRC8:
        case Measure::MEASURE_TLRC9:{
            ITask* task = m_task_center.AllocTask(path, L"LyricTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new LyricTask(path);
            });
            m_task_center.RunTask(path, L"LyricTask");

            m_task_center.AllocTask(path, L"LyricUpdateTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new LyricUpdateTask(path);
            });
            task = m_task_center.AllocTask(path, L"LyricCallBackTask", []() -> ITask* {
                MemoryCount::AllocTask();
                return new LyricCallBackTask();
            });
            if(auto ptr = dynamic_cast<ICallBackTask *>(task)){
                ptr->AddCallBack(measure);
            }
            
            m_task_center.AllocTask(path, L"StopLyricTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new StopLyricTask(path);
            });
            m_task_center.AllocTask(path, L"StartLyricTask", [path]() -> ITask* {
                MemoryCount::AllocTask();
                return new StartLyricTask(path);
            });
        }break;
    }
    return path;
}
void Core::Update(const std::wstring &name_space){
    m_task_center.Run(name_space);
}
TaskCenter *Core::GetTaskCenter(){
    return &m_task_center;
}
