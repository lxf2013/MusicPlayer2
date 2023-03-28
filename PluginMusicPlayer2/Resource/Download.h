#pragma once

#include "../ThirdPart/httplib.h"
#include <fstream>
#include <string>
#include <vector>
#include "../common.h"


class Downloader{
private:
    struct SongInfo{
        std::string id;
        std::string cover;
        int duration = 0;
        std::string info;
    };
private:
    std::string  m_id;
    std::wstring  m_track;
    int m_duration;

    httplib::Client  m_client;
    std::wstring m_download_path;
    double  m_similarity;
private:
    Result<void> SetId(const std::string &json, const std::string &track);
    Result<std::string> GetId(const std::wstring &track);
    Result<std::vector<SongInfo>> GetSongList(const std::string &json);
public:
    Downloader();
    bool Init(const std::wstring &path, double similarity);
    Result<std::wstring> DownloadLyric(const std::wstring &track);
    Result<std::wstring> DownloadCover(const std::wstring &track);
};