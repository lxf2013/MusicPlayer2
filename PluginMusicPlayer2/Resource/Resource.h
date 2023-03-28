#pragma once

#include "Download.h"
#include "LRU.h"

class Resource{
private:
    Downloader   m_downloader;
    LRU          m_lru;
    LRU          m_failed;
    std::string  m_path;
	std::wstring m_default_cover;
	std::wstring m_default_lyric;
    std::wstring m_cover_path;
    std::wstring m_lyric_path;
public:
    bool Reset(const std::wstring &default_cover, const std::wstring &default_lyric, const std::wstring &_path, double similarity, int capacity);
    const std::wstring &GetLyricPath(const std::wstring &_track);
    const std::wstring &GetCoverPath(const std::wstring &_track);
    bool IsDefault(const std::wstring &_track);
};