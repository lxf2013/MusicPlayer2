#pragma once

class PlayerCallBackTask: public ICallBackTask{
public:
    PlayerCallBackTask():
        ICallBackTask(L"PlayerCallBackTask")
    {
    }
    bool AddCallBack(Measure *measure) override{
        if(measure->change_action.empty()){
            return false;
        }
        switch(measure->type){
            case Measure::MEASURE_TITLE:
            case Measure::MEASURE_ARTIST:
            case Measure::MEASURE_TRACK:
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
};