#pragma once

#include "StdAfx.h"
#include <fstream>
#include <string>
#include <vector>
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
    string  m_id;
    Client  m_client;
    wstring m_lyric_path;
    wstring m_cover_path;
    string  m_track;
    LRU     m_lru;
    wstring m_download_path;
    double  m_similarity;
private:
    bool InitLRU(const string &path);
    bool InitId(const string &track);
    void GetSongList(const string &json, vector<SongInfo> &song_infos);
    bool GetId(const string &json, const string &track);
public:
    Downloader();
    bool Init(const wstring &path, uint32_t capacity, double similarity);
    bool DownloadLyric(const wstring &track);
    bool DownloadCover(const wstring &track);
    LPCWSTR GetLyricPath();
    LPCWSTR GetCoverPath();
};