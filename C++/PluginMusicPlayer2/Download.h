#pragma once

#include "StdAfx.h"
#include <fstream>
#include <string>
#include "LRU.h"

using namespace std;
using namespace httplib;

class Downloader{
private:
    struct SongInfo{
        string id;
        string info;
    };
private:
    string m_id;
    Client m_client;
    wstring m_lyric_path;
    wstring m_cover_path;
    string  m_track;
    LRU     m_lru;
private:
    bool InitLRU(const string &path);
    bool Init(const string &track);
    void GetSongList(const string &json, vector<SongInfo> &song_infos);
    bool GetId(const string &json, const string &track);
public:
    Downloader();
    Downloader(const Downloader &) = delete;
    Downloader& operator=(const Downloader&) = delete;
public:
    wstring m_download_path;
    
    static Downloader* GetInstance();
    bool DownloadLyric(const wstring &track);
    bool DownloadCover(const wstring &track);
    LPCWSTR GetLyricPath();
    LPCWSTR GetCoverPath();
};