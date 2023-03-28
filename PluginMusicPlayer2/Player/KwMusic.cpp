#include "KwMusic.h"

KwMusic *KwMusic::c_player = nullptr;

KwMusic* KwMusic::Create(){
    if(c_player == nullptr){
        MemoryCount::AllocPlayer();
        c_player = new KwMusic;
        c_player->Init();
    }
    c_player->AddInstantce();
    return c_player;
}
KwMusic::~KwMusic(){
    c_player = nullptr;
}
void KwMusic::Init(){
    m_window = FindWindowEx(0, 0, L"kwmusicmaindlg", 0);
}
bool KwMusic::Update(DWORD time){
    // DWORD time = GetTickCount();
    if(time < m_last_time+500){
        return false;
    }
    m_last_time = time;
    
    bool abnormal = false;
    std::wstring LastTrack = m_track;


    //检查窗口
    if (!IsWindow(m_window)){
        m_window = NULL;
        Init();
    }
    if (!m_window){
        abnormal = true;
        m_track.clear();
    }


    //获取信息
    if (!abnormal){
        WCHAR Buffer[102];
        Buffer[100] = '\0';
        GetWindowText(m_window, Buffer, 100);
        //L"With You-AAA-酷我音乐 "
        //L"子-酷我音乐 虹の約束-小松未可"

        std::wstring data = Buffer;
        data += data;
        data += data;

        std::wstring::size_type pos;
        pos = data.find(L"-酷我音乐 ");

        if (pos == std::wstring::npos){
            abnormal = true;
            m_track = Buffer;
        } else {
            pos += 6;
            data.erase(0, pos);
            pos = data.find(L"-酷我音乐 ");
            data.resize(pos);
        }

        
        if (!abnormal && LastTrack.compare(data)){
            pos = 0;
            int pos2 = 0, pos3 = 0;
            int len = data.length();
            for (int i = 1; i < len; i++){
                if (data[i] == L'-'){
                    if (data[i - 1] == L' '){
                        if (data[i + 1] == L' ')
                            pos3 = i;
                        else
                            pos2 = i;
                    } else {
                        if (data[i + 1] == L' ')
                            pos2 = i;
                        else{
                            pos = i;
                            break;		//真正的分隔符左右两侧应当都不为空格
                        }
                    }
                }
            }
            pos = pos == 0 ? pos2 : pos;
            pos = pos == 0 ? pos3 : pos;

            if (pos == 0){
                abnormal = true;
                m_track = Buffer;
            } else {
                m_track.assign(data);
                m_title.assign(data, 0, pos);
                m_artist.assign(data.erase(0, pos + 1));
                abnormal = false;
            }
        }
    }

    if (abnormal){
        m_title.clear();
        m_artist.clear();
    }

    return m_track.compare(LastTrack) !=0;
}

//窗口操作
void KwMusic::OpenPlayer(){
    if (!IsWindow(m_window)){
        ShellExecute(nullptr, L"open", L"KwMusic.exe", nullptr, nullptr, SW_SHOW);
    } else {
        ShowWindow(m_window, SW_SHOWNORMAL);
        BringWindowToTop(m_window);
    }
}
void KwMusic::ClosePlayer(){
    if (IsWindow(m_window)){
        PostMessage(m_window, WM_CLOSE, 0, 0);
    }
}


//按键操作
void KwMusic::PlayPause(){
    if (IsWindow(m_window))
        SendKey(m_window, VK_F5);
}
void KwMusic::Stop(){
    if (IsWindow(m_window))
        SendKey(m_window, VK_F6);
}
void KwMusic::Previous(){
    if (IsWindow(m_window))
        SendKey(m_window, VK_LEFT, 1);
}
void KwMusic::Next(){
    if (IsWindow(m_window))
        SendKey(m_window, VK_RIGHT, 1);
}