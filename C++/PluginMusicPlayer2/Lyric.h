#pragma once

#include "StdAfx.h"
#include <vector>
#include <string>

//#include <windows.h>

using namespace std;

class Lyric{
private:
    struct Line{
        uint64_t time;
        wstring   lrc;
    };

private:
    vector<Line> m_lyric;
    uint32_t     m_current_pos;
	DWORD        m_last_time;
    bool         m_stop;
    DWORD        m_stop_time;
    int          m_offset;

private:
    uint64_t GetNum(const char *file_buf, uint32_t file_size, uint32_t &i);

public:
    Lyric();
    Lyric(const Lyric&) = delete;
    Lyric& operator=(const Lyric&) = delete;

public:
    static Lyric* GetInstance();
    bool Load(const wstring &file, int offset);
    void Stop();
    void Start();
    void StopOrStart();
    void Update();
    LPCWSTR GetLyric(int32_t pos);
};
