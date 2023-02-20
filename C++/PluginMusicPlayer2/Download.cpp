
#include "Download.h"
#include <codecvt>
#include <direct.h>
#include <filesystem>
#include "cJSON.h"
#include "../../API/RainmeterAPI.h"

// const char *url = "http://music.163.com/api/search/get/?s=邓紫棋 光年之外&limit=1&type=1&offset=0";
// string GetId(url){
    // int index1 = result.find(L"\"songs\":[{\"id\":", index1 + 1);
    // if (index1 == string::npos) break;
    // int index2 = result.find(L',', index1);
    // return result.substr(index1 + 15, index2 - index1 - 15);
// }
// const char *lyric_url = "http://music.163.com/api/song/media?id=" + GetId(url);

// string GetImg(url){
    // int index = result.find("\"album\"");
    // if (index == wstring::npos) return -1;
    // index = result.find("\"picUrl\"", index + 7);
    // if (index == wstring::npos) return -1;
    // int index1 = result.find(L'\"', index + 10);
    // result = result.substr(index + 10, index1 - index - 10);
// }
// const char *turl = "http://music.163.com/api/song/detail/?id=" + GetId(url) + "&ids=%5B" + GetId(url) + "%5D";
// const char *cover_url = GetImg(turl)

/*static string wstring2string(const wstring& wstr) {
    string result;
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    if (len <= 0) {
        return result;
    }
    char* buffer = new char[len + 1];
    if (buffer == NULL) {
        return result;
    }
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = 0;
    result.append(buffer);
    delete[] buffer;
    return result;
}*/

Downloader::Downloader(): m_id(), m_client("http://music.163.com"), m_lru(1) {
}

void Downloader::GetSongList(const string &json, vector<SongInfo> &song_infos){
    if(cJSON *root = cJSON_Parse(json.c_str())){
        do{
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if(result == nullptr){
                break;
            }

            cJSON *songs = cJSON_GetObjectItem(result, "songs");
            if(songs == nullptr){
                break;
            }
            int song_size = cJSON_GetArraySize(songs);
            for(int i=0; i<song_size; ++i){
                SongInfo song_info;
                cJSON *song = cJSON_GetArrayItem(songs, i);
                if(song == nullptr){
                    continue;
                }

                cJSON *duration = cJSON_GetObjectItem(song, "duration");
                if(duration == nullptr){
                    continue;
                }
                song_info.duration = duration->valueint;

                cJSON *item = cJSON_GetObjectItem(song, "id");
                if(item == nullptr){
                    continue;
                }
                song_info.id = to_string(item->valueint);

                item = cJSON_GetObjectItem(song, "name");
                if(item == nullptr){
                    continue;
                }
                song_info.info = item->valuestring;

                cJSON *artists = cJSON_GetObjectItem(song, "artists");
                if(artists == nullptr){
                    continue;
                }
                int artist_size = cJSON_GetArraySize(artists);
                for(int j=0; j<artist_size; ++j){
                    cJSON *artist = cJSON_GetArrayItem(artists, j);
                    if(artist == nullptr){
                        continue;
                    }

                    if(cJSON *artist_name = cJSON_GetObjectItem(artist, "name")){
                        song_info.info += " ";
                        song_info.info += artist_name->valuestring;
                    }
                }
                song_infos.push_back(song_info);
            }
        }while(0);
        cJSON_Delete(root);
    }
}

bool Downloader::GetId(const string &json, const string &track){
    vector<SongInfo> song_infos;
    GetSongList(json, song_infos);
    int ti = -1, tdp = 0x3f3f3f3f;

    // 动态规划，找出相似度最高的一个
    for(uint32_t i=0; i < song_infos.size(); ++i){
        int dp[256][256] = {};
        for(int i=0; i<256; ++i){
            dp[i][0] = dp[0][i] = i;
        }
        
        for(uint32_t j=1; j<256 && j<=track.size(); ++j){
            for(uint32_t k=1; k<256 && k<=song_infos[i].info.size(); ++k){
                if(track[j-1] == song_infos[i].info[k-1]){
                    //                 加上一个     删去一个          匹配当前
                    dp[j][k] = min(min(dp[j-1][k], dp[j][k-1]) + 1, dp[j-1][k-1]);
                }else{
                    // //                 加上一个     删去一个     替换一个
                    // dp[j][k] = min(min(dp[j-1][k], dp[j][k-1]), dp[j-1][k-1]) + 1;

                    // 此处不考虑替换
                    //             加上一个     删去一个
                    dp[j][k] = min(dp[j-1][k], dp[j][k-1]) + 1;
                }
            }
        }

        if(tdp > dp[track.size()][song_infos[i].info.size()]){
            tdp = dp[track.size()][song_infos[i].info.size()];
            ti = i;
        }
    }

    // 相似度计算
    int similarity = track.size() * 2.0 - track.size() * m_similarity;

    if(ti < 0 || tdp > similarity){
        return false;
    }

    m_id = song_infos[ti].id;
    m_duration = song_infos[ti].duration;
    return true;
}

bool Downloader::InitLRU(const string &path){
    std::filesystem::path dir(path);
    if (std::filesystem::exists(dir) == false) {
        return !mkdir(path.c_str());
    }
    for (auto& it : std::filesystem::directory_iterator(path)) {
        string temp = it.path().stem().u8string();

        wstring_convert<codecvt_utf8<wchar_t>> converter;

        temp = m_lru.insert(temp);
        if (temp.size()) {
            DeleteFile(converter.from_bytes(path + "\\" + temp + ".lrc").c_str());
            DeleteFile(converter.from_bytes(path + "\\" + temp + ".jpg").c_str());
        }
    }
    return true;
}

bool Downloader::InitId(const string &track){
    if(track.empty()){
        return false;
    }
    if(m_track == track){
        return true;
    }

    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string path = converter.to_bytes(m_download_path);

    string info_list_path = "/api/search/get/?s=" + track + "&limit=10&type=1&offset=0";
    // string info_list_path = "/api/search/get/?s=" + track + "&limit=1&type=1&offset=0";
    
    if(auto res = m_client.Post(info_list_path)){
        if(res->status == 200){
            string temp = res->body;
            if (GetId(temp, track) == false) {
                return false;
            }
            m_track = track;

            temp = m_lru.insert(track);
            if(temp.size()){
                DeleteFile(converter.from_bytes(path + "\\" + temp + ".lrc").c_str());
                DeleteFile(converter.from_bytes(path + "\\" + temp + ".jpg").c_str());
            }
            return true;
        }
    }
    return false;
}

bool Downloader::Init(const wstring &_path, uint32_t capacity, double similarity){
    m_download_path = _path;
    m_similarity    = similarity;
    
    m_lru.Update(capacity);

    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string path = converter.to_bytes(m_download_path);
    return InitLRU(path);
}

bool Downloader::DownloadLyric(const wstring &_track){
    m_lyric_path = L"";
    if(m_download_path.empty()){
        return false;
    }
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string track = converter.to_bytes(_track);

    if(_waccess((m_download_path + L"\\" + _track + L".lrc").c_str(), 0) == 0){
        m_lyric_path = m_download_path + L"\\" +  _track + L".lrc";
        return true;
    }
    if(InitId(track) == false){
        return false;
    }
    string lyric_path = "/api/song/lyric?lv=1&kv=1&tv=-1&id=" + m_id;
    // string lyric_path = "/api/song/media?id=" + m_id;

    if(auto res = m_client.Post(lyric_path)){
        if(res->status == 200){
            ofstream lyric(m_download_path + L"\\" + _track + L".lrc", ios::out);
            if (lyric.is_open()) {
                if(cJSON *root = cJSON_Parse(res->body.c_str())){
                    do{
                        cJSON *lrc = cJSON_GetObjectItem(root, "lrc");
                        if(lrc == nullptr){
                            break;
                        }

                        cJSON *_lyric = cJSON_GetObjectItem(lrc, "lyric");
                        if(_lyric == nullptr){
                            break;
                        }
                        lyric << "[duration:" << m_duration << "]\n";
                        lyric << _lyric->valuestring;
                        m_lyric_path = m_download_path + L"\\" + _track + L".lrc";
                    }while(0);
                    cJSON_Delete(root);
                }
                return true;
            }
            RmLog(LOG_WARNING, converter.from_bytes(strerror(errno)).c_str());
        }
    }
    return false;
}

bool Downloader::DownloadCover(const wstring &_track){
    m_cover_path = L"";
    if(m_download_path.empty()){
        return false;
    }
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string track = converter.to_bytes(_track);

    if(_waccess((m_download_path + L"\\" + _track + L".jpg").c_str(), 0) == 0){
        m_cover_path = m_download_path;
        m_cover_path += L"\\";
        m_cover_path += _track;
        m_cover_path += L".jpg";
        return true;
    }

    if(InitId(track) == false){
        return false;
    }

    string cover_info_path = "/api/song/detail/?id=" + m_id + "&ids=%5B" + m_id + "%5D";
    
    string img_url;
    if(auto res = m_client.Post(cover_info_path)){
        if(res->status == 200){
            img_url = res->body;
            int index = img_url.find("\"album\"");
            if (index == wstring::npos) {
                return false;
            }
            index = img_url.find("\"picUrl\"", index + 7);
            if (index == wstring::npos) {
                return false;
            }
            int index1 = img_url.find(L'\"', index + 10);
            if (index1 == wstring::npos) {
                return false;
            }
            img_url = img_url.substr(index + 10, index1 - index - 10);
            img_url += "?param=200y200";
        }
    }

    if(auto res = m_client.Get(img_url)){
        if(res->status == 200){
            ofstream cover(m_download_path + L"\\" +  _track + L".jpg", ios::out | ios::binary);
            if (cover.is_open()) {
                cover << res->body;

                m_cover_path = m_download_path;
                m_cover_path += L"\\";
                m_cover_path += _track;
                m_cover_path += L".jpg";
                return true;
            }
            RmLog(LOG_WARNING, converter.from_bytes(strerror(errno)).c_str());
        }
    }
    return false;
}

LPCWSTR Downloader::GetLyricPath() {
    return m_lyric_path.c_str();
}

LPCWSTR Downloader::GetCoverPath() {
    return m_cover_path.c_str();
}