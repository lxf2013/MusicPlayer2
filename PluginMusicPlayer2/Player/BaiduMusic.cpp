#include "BaiduMusic.h"

BaiduMusic *BaiduMusic::c_player = nullptr;

//加载与更新
BaiduMusic* BaiduMusic::Create(){
    if(c_player == nullptr){
        MemoryCount::AllocPlayer();
        c_player = new BaiduMusic;
        c_player->Init();
    }
    c_player->AddInstantce();
    return c_player;
}
BaiduMusic::~BaiduMusic(){
    c_player = nullptr;
}
void BaiduMusic::Init(){
    HWND hWnd = NULL;
    HWND hWnd2;
    LONG lWnd;

    do{
        hWnd = FindWindowEx(0, hWnd, 0, L"百度音乐");
        lWnd = GetWindowLong(hWnd, GWL_STYLE);
        if ((lWnd | 0x10000000) == 0x960B004C){
            hWnd2 = GetParent(hWnd);
            lWnd = GetWindowLong(hWnd, GWL_STYLE);
            if ((lWnd | 0x10000000) == 0x960B004C){
                m_window = hWnd2;
                break;
            }
        }
    } while (hWnd);
}
bool BaiduMusic::Update(DWORD time){
    // DWORD time = GetTickCount();
    if(time < m_last_time+500){
        return false;
    }
    m_last_time = time;
    
    bool abnormal = false;
    std::wstring LastTrack = m_track;


    //检查窗口
    if (!IsWindow(m_window)) {
        m_window = NULL;
        Init();
    }
    if (!m_window) {
        abnormal = true;
        m_track.clear();
    }

    //获取信息
    if (!abnormal){
        WCHAR Buffer[102];
        Buffer[100] = '\0';
        GetWindowText(m_window, Buffer, 100);
        //L"%T - %A - 百度音乐"

        std::wstring data = Buffer;

        std::wstring::size_type pos;
        pos = data.find(L" - 百度音乐");

        if (pos == std::wstring::npos){
            abnormal = true;
        } else {
            data.resize(pos);
            pos = data.find(L" - ");
        }
        
        if (pos == std::wstring::npos){
            abnormal = true;
            m_track = Buffer;
        } else if (LastTrack.compare(data)) {
            m_track.assign(data);
            m_title.assign(data, 0, pos);
            m_artist.assign(data.erase(0, pos + 3));
            abnormal = false;
        }
    }

    if (abnormal){
        m_title.clear();
        m_artist.clear();
    }

    return m_track.compare(LastTrack) != 0;
}


//窗口操作
void BaiduMusic::OpenPlayer(){
    if (!IsWindow(m_window)){
        ShellExecute(nullptr, L"open", L"BaiduMusic.exe", nullptr, nullptr, SW_SHOW);
    }
}
void BaiduMusic::ClosePlayer(){

}