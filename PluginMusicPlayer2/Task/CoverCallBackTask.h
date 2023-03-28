#pragma once

class CoverCallBackTask: public ICallBackTask{
public:
    CoverCallBackTask():
        ICallBackTask(L"CoverCallBackTask")
    {
    }
    bool AddCallBack(Measure *measure) override{
        if(measure->change_action.empty()){
            return false;
        }
        switch(measure->type){
        case Measure::MEASURE_COVER:
                for(auto it: m_call_back){
                    if(it == measure){
                        return true;
                    }
                }
                m_call_back.push_back(measure);
                break;
        }
        return true;
    }
};