#pragma once

class LyricCallBackTask: public ICallBackTask{
public:
    LyricCallBackTask():
        ICallBackTask(L"LyricCallBackTask")
    {
    }
    bool AddCallBack(Measure *measure) override{
        if(measure->change_action.empty()){
            return false;
        }
                // RmLog(LOG_WARNING, L"callback");
        switch(measure->type){
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