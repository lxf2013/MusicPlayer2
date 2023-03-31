#pragma once

// #include "StdAfx.h"
#include <vector>
#include <string>

#include <windows.h>

// using namespace std;

class Lyric{
private:
    struct Line{
        uint64_t time;
        std::wstring   lrc;
        std::wstring   tlrc;
    };

private:
    std::vector<Line> m_lyric;
    uint32_t     m_current_pos;
	DWORD        m_last_time;
    bool         m_stop;
    DWORD        m_stop_time;
    int          m_offset;
    uint64_t     m_duration;
    // bool         m_stat;

private:
    uint64_t GetNum(const char *file_buf, uint32_t file_size, uint32_t &i);
    bool GetTranslate(const char *file_buf, uint32_t file_size, uint32_t &i);

public:
    Lyric();

public:
    bool Init(int offset);
    bool Load(const std::wstring &file);
    void Stop();
    void Stop(DWORD time);
    void Start();
    void Start(DWORD time);
    void StopOrStart();
    bool Update(DWORD time);
    // bool GetStat();
    bool IsEnd();
    LPCWSTR GetLyric(int32_t pos);
    LPCWSTR GetTLyric(int32_t pos);
};
