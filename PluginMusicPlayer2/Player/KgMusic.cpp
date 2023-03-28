#include "KgMusic.h"

KgMusic *KgMusic::c_player = nullptr;

//加载与更新
KgMusic* KgMusic::Create(){
    if(c_player == nullptr){
        MemoryCount::AllocPlayer();
        c_player = new KgMusic;
        c_player->Init();
    }
    c_player->AddInstantce();
    return c_player;
}
KgMusic::~KgMusic(){
    c_player = nullptr;
}
void KgMusic::Init(){
    HWND hWnd = NULL;
    LONG lWnd;

    do{
        hWnd = FindWindowEx(NULL, hWnd, L"kugou_ui", NULL);
        lWnd = GetWindowLong(hWnd, GWL_STYLE);
        if ((lWnd & WS_VISIBLE) &&
            (lWnd & WS_SYSMENU) &&
            GetWindowTextLength(hWnd)){
            m_window = hWnd;
            break;
        }
    } while (hWnd);
}
bool KgMusic::Update(DWORD time){
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
    if (!m_window)
    {
        abnormal = true;
        m_track.clear();
    }

    //获取信息
    if (!abnormal)
    {
        WCHAR Buffer[102];
        Buffer[100] = '\0';
        GetWindowText(m_window, Buffer, 100);
        //L"%FileName - 酷狗音乐 "(滚动)

        std::wstring data = Buffer;
        data += data;
        data += data;

        std::wstring::size_type pos;
        pos = data.find(L" - 酷狗音乐 ");

        if (pos == std::wstring::npos){
            abnormal = true;
            m_track = Buffer;
        } else {
            pos += 8;
            data.erase(0, pos);
            pos = data.find(L" - 酷狗音乐 ");
            data.resize(pos);
        }

        if (!abnormal && LastTrack.compare(data)){
            pos = 0;
            int pos2 = 0, pos3 = 0;
            int len = data.length();
            for (int i = 1; i < len; i++){
                if (data[i] == L'-'){
                    if (data[i - 1] == L' '){
                        if (data[i + 1] == L' '){
                            pos = i;
                            break;
                        }	
                        else
                            pos2 = i;
                    } else {
                        if (data[i + 1] == L' ')
                            pos2 = i;
                        else {
                            pos3 = i;
                        }
                    }
                }
            }
            pos = pos == 0 ? pos2 : pos;
            pos = pos == 0 ? pos3 : pos;

            abnormal = false;
            m_track = data;
            
            if (pos == 0){
                m_title = data;
            } else {
                m_artist.assign(data, 0, pos);
                m_title.assign(data.erase(0, pos + 1));
                
                if (m_artist[--pos] == L' ')
                    m_artist.erase(pos);
                if (m_title[0] == L' ')
                    m_title.erase(0, 1);
            }
        }
    }

    if (abnormal)
    {
        m_title.clear();
        m_artist.clear();
    }

    return m_track.compare(LastTrack) != 0;
}

//窗口操作
void KgMusic::OpenPlayer(){
    if (!IsWindow(m_window)){
        ShellExecute(nullptr, L"open", L"KuGou.exe", nullptr, nullptr, SW_SHOW);
    } else {
        PostMessage(m_window, WM_SYSCOMMAND, SC_RESTORE, 0);
        BringWindowToTop(m_window);
    }
}
void KgMusic::ClosePlayer(){
    if (IsWindow(m_window))
        PostMessage(m_window, WM_CLOSE, 0, 0);
}