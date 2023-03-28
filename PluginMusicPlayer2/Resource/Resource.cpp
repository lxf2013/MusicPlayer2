
#include "Resource.h"
#include <codecvt>
#include <filesystem>
#include <direct.h>

#include "../ThirdPart/RainmeterAPI.h"

bool Resource::Reset(const std::wstring &default_cover, const std::wstring &default_lyric, const std::wstring &_path, double similarity, int capacity){
    m_default_cover = default_cover;
    m_default_lyric = default_lyric;
    m_downloader.Init(_path, similarity);
    m_lru.SetCapacity(capacity);
    m_failed.SetCapacity(50);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string path = converter.to_bytes(_path);
    m_path = path;

    std::filesystem::path dir(path);
    if (std::filesystem::exists(dir) == false) {
        // if(mkdir(path.c_str()) != 0){

        // }
        return !mkdir(path.c_str());
    }
    for (auto& it : std::filesystem::directory_iterator(path)) {
        std::string temp = it.path().stem().u8string();

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        temp = m_lru.Insert(temp);
        if (temp.size()) {
            DeleteFile(converter.from_bytes(path + "\\" + temp + ".lrc").c_str());
            DeleteFile(converter.from_bytes(path + "\\" + temp + ".jpg").c_str());
        }
    }
    return true;
}


const std::wstring &Resource::GetLyricPath(const std::wstring &_track){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string track = converter.to_bytes(_track);
    if (m_failed.Query(track) || _track.empty()) {
        return m_default_lyric;
    }
    
    auto result = m_downloader.DownloadLyric(_track);
    if(result.err_code != ErrorInfo::OK){
        m_failed.Insert(track);
        return m_default_lyric;
    }
    
    std::string temp = m_lru.Insert(track);
    if(temp.size()){
        DeleteFile(converter.from_bytes(m_path + "\\" + temp + ".lrc").c_str());
        DeleteFile(converter.from_bytes(m_path + "\\" + temp + ".jpg").c_str());
    }
    return m_lyric_path = result.result;
}


const std::wstring &Resource::GetCoverPath(const std::wstring &_track){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string track = converter.to_bytes(_track);
    if (m_failed.Query(track) || _track.empty()) {
        return m_default_cover;
    }
    
    auto result = m_downloader.DownloadCover(_track);
    if(result.err_code != ErrorInfo::OK){
        m_failed.Insert(track);
        return m_default_cover;
    }
    
    std::string temp = m_lru.Insert(track);
    if(temp.size()){
        DeleteFile(converter.from_bytes(m_path + "\\" + temp + ".lrc").c_str());
        DeleteFile(converter.from_bytes(m_path + "\\" + temp + ".jpg").c_str());
    }
    return m_cover_path = result.result;
}

bool Resource::IsDefault(const std::wstring &_track){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string track = converter.to_bytes(_track);
// RmLog(LOG_WARNING, _track.c_str());
    if (m_failed.Query(track) || _track.empty()) {
// RmLog(LOG_WARNING, L"true");
        return true;
    }
// RmLog(LOG_WARNING, L"false");
    return false;
}