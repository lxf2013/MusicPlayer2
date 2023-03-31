
#include "Download.h"
#include <codecvt>
#include <direct.h>
#include <filesystem>
#include "../ThirdPart/cJSON.h"
#include "../ThirdPart/RainmeterAPI.h"

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

Downloader::Downloader(): m_duration(0), m_client("http://music.163.com"), m_similarity(0) {
}

Result<std::vector<Downloader::SongInfo>> Downloader::GetSongList(const std::string &json){
    if(cJSON *root = cJSON_Parse(json.c_str())){
        Result<std::vector<SongInfo>> ret = std::vector<SongInfo>();
        do{
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if(result == nullptr){
                ret.err_code = ErrorInfo::WARNING;
                ret.msg = L"json not find result";
                break;
            }

            cJSON *songs = cJSON_GetObjectItem(result, "songs");
            if(songs == nullptr){
                ret.err_code = ErrorInfo::WARNING;
                ret.msg = L"json not find songs";
                break;
            }
            int song_size = cJSON_GetArraySize(songs);
            for(int i=0; i<song_size; ++i){
                SongInfo song_info;
                cJSON *song = cJSON_GetArrayItem(songs, i);
                if(song == nullptr){
                    // ret.err_code = Result::FAILD;
                    // ret.msg = L"cJSON_GetArrayItem error";
                    continue;
                }

                cJSON *duration = cJSON_GetObjectItem(song, "duration");
                if(duration == nullptr){
                    // ret.err_code = Result::FAILD;
                    // ret.msg = L"json not find duration";
                    continue;
                }
                song_info.duration = duration->valueint;

                cJSON *item = cJSON_GetObjectItem(song, "id");
                if(item == nullptr){
                    // ret.err_code = Result::FAILD;
                    // ret.msg = L"json not find id";
                    continue;
                }
                song_info.id = std::to_string(item->valueint);

                item = cJSON_GetObjectItem(song, "name");
                if(item == nullptr){
                    // ret.err_code = Result::FAILD;
                    // ret.msg = L"json not find name";
                    continue;
                }
                song_info.info = item->valuestring;

                cJSON *artists = cJSON_GetObjectItem(song, "artists");
                if(artists == nullptr){
                    // ret.err_code = Result::FAILD;
                    // ret.msg = L"json not find artists";
                    continue;
                }
                int artist_size = cJSON_GetArraySize(artists);
                for(int j=0; j<artist_size; ++j){
                    cJSON *artist = cJSON_GetArrayItem(artists, j);
                    if(artist == nullptr){
                        // ret.err_code = Result::FAILD;
                        // ret.msg = L"cJSON_GetArrayItem error";
                        continue;
                    }

                    if(cJSON *artist_name = cJSON_GetObjectItem(artist, "name")){
                        song_info.info += " ";
                        song_info.info += artist_name->valuestring;
                    }
                }
                ret.result.push_back(song_info);
            }
        }while(0);
        cJSON_Delete(root);
        return ret;
    }
    return Result<std::vector<Downloader::SongInfo>>({}, ErrorInfo::WARNING, L"json error");
}

Result<void> Downloader::SetId(const std::string &json, const std::string &track){
    Result<std::vector<Downloader::SongInfo>> result = GetSongList(json);
    if(result.err_code != ErrorInfo::OK){
        return Result<void>(ErrorInfo::FAILD, result.msg);
    }
    int ti = -1, tdp = 0x3f3f3f3f;

    // 动态规划，找出相似度最高的一个
    for(uint32_t i=0; i < result.result.size(); ++i){
        uint32_t size = std::max(track.size(), result.result[i].info.size()) + 5;
        std::vector<std::vector<int>> dp(size, std::vector<int>(size));
        for(uint32_t i=0; i<size; ++i){
            dp[i][0] = dp[0][i] = i;
        }
        
        for(uint32_t j=1; j<256 && j<=track.size(); ++j){
            for(uint32_t k=1; k<256 && k<=result.result[i].info.size(); ++k){
                if(track[j-1] == result.result[i].info[k-1]){
                    //                 加上一个     删去一个          匹配当前
                    dp[j][k] = std::min(std::min(dp[j-1][k], dp[j][k-1]) + 1, dp[j-1][k-1]);
                }else{
                    // //                 加上一个     删去一个     替换一个
                    // dp[j][k] = min(min(dp[j-1][k], dp[j][k-1]), dp[j-1][k-1]) + 1;

                    // 此处不考虑替换
                    //             加上一个     删去一个
                    dp[j][k] = std::min(dp[j-1][k], dp[j][k-1]) + 1;
                }
            }
        }

        if(tdp > dp[track.size()][result.result[i].info.size()]){
            tdp = dp[track.size()][result.result[i].info.size()];
            ti = i;
        }
    }

    // 相似度计算
    int similarity = track.size() * 2.0 - track.size() * m_similarity;

    if(ti < 0 || tdp > similarity){
        return Result<void>(ErrorInfo::WARNING, L"not find id");
    }

    m_id = result.result[ti].id;
    m_duration = result.result[ti].duration;
    return Result<void>();
}

Result<std::string> Downloader::GetId(const std::wstring &_track){
    if(_track.empty()){
        return Result<std::string>("", ErrorInfo::FAILD, L"track is empty");
    }
    if(m_track == _track){
        return m_id;
    }

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string track = converter.to_bytes(_track);

    std::string info_list_url = "/api/search/get/?s=" + track + "&limit=10&type=1&offset=0";
    // string info_list_url = "/api/search/get/?s=" + track + "&limit=1&type=1&offset=0";
    // http://music.163.com/api/search/get/?s=光年之外 - G.E.M.&limit=10&type=1&offset=0
    if(auto res = m_client.Post(info_list_url)){
        if(res->status == 200){
            std::string temp = res->body;
            if (SetId(temp, track).err_code == ErrorInfo::OK) {
                m_track = _track;
                return m_id;
            }
        }
    }
    return Result<std::string>("", ErrorInfo::FAILD, L"not find " + _track);
}

bool Downloader::Init(const std::wstring &_path, double similarity){
    m_download_path = _path;
    m_similarity    = similarity;
    return true;
}

Result<std::wstring> Downloader::DownloadLyric(const std::wstring &_track){
    if(m_download_path.empty()){
        return Result<std::wstring>(L"", ErrorInfo::FAILD, L"download path is empty");
    }

    std::wstring path = m_download_path + L"\\" + _track + L".lrc";
    Result<std::wstring> ret = path;

    if(_waccess(path.c_str(), 0) == 0){
        return ret;
    }
    
    auto result = GetId(_track);
    if(result.err_code != ErrorInfo::OK){
        return Result<std::wstring>(L"", result.err_code, result.msg);
    }

    std::string lyric_path = "/api/song/lyric?lv=1&kv=1&tv=-1&id=" + result.result;
    // string lyric_path = "/api/song/media?id=" + m_id;

    if(auto res = m_client.Post(lyric_path)){
        if(res->status == 200){
            std::ofstream lyric(m_download_path + L"\\" + _track + L".lrc", std::ios::out);
            if (lyric.is_open()) {
                if(cJSON *root = cJSON_Parse(res->body.c_str())){
                    do{
                        cJSON *lrc = cJSON_GetObjectItem(root, "lrc");
                        if(lrc == nullptr){
                            ret.err_code = ErrorInfo::FAILD;
                            ret.msg = L"json not find lrc";
                            break;
                        }

                        cJSON *_lyric = cJSON_GetObjectItem(lrc, "lyric");
                        if(_lyric == nullptr){
                            ret.err_code = ErrorInfo::FAILD;
                            ret.msg = L"json not find lyric";
                            break;
                        }
                        lyric << "[duration:" << m_duration << "]\n";
                        lyric << _lyric->valuestring;

                        cJSON *tlrc = cJSON_GetObjectItem(root, "tlyric");
                        if(tlrc == nullptr){
                            break;
                        }

                        cJSON *tlyric = cJSON_GetObjectItem(tlrc, "lyric");
                        if(tlyric == nullptr){
                            break;
                        }
                        lyric << "[translate]\n";
                        lyric << tlyric->valuestring;
                    }while(0);
                    cJSON_Delete(root);
                }
                return ret;
            }
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return Result<std::wstring>(L"", ErrorInfo::FAILD, converter.from_bytes(strerror(errno)));
            // RmLog(LOG_WARNING, converter.from_bytes(strerror(errno)).c_str());
        }
    }
    return Result<std::wstring>(L"", ErrorInfo::FAILD, L"not find lrc");
}

Result<std::wstring> Downloader::DownloadCover(const std::wstring &_track){
    if(m_download_path.empty()){
        return Result<std::wstring>(L"", ErrorInfo::FAILD, L"download path is empty");
    }

    std::wstring path = m_download_path + L"\\" + _track + L".jpg";

    if(_waccess(path.c_str(), 0) == 0){
        return path;
    }

    auto result = GetId(_track);
    if(result.err_code != ErrorInfo::OK){
        return Result<std::wstring>(L"", result.err_code, result.msg);
    }

    // std::string cover_info_url = "/api/song/detail/?id=" + id + "&ids=%5B" + id + "%5D";
    std::string cover_info_url = "/api/song/detail/?id=" + result.result + "&ids=%5B" + result.result + "%5D";
    
    std::string img_url;
    if(auto res = m_client.Post(cover_info_url)){
        if(res->status == 200){
            img_url = res->body;
            int index = img_url.find("\"album\"");
            if (index == std::wstring::npos) {
                return Result<std::wstring>(L"", ErrorInfo::FAILD, L"not find album");
            }
            index = img_url.find("\"picUrl\"", index + 7);
            if (index == std::wstring::npos) {
                return Result<std::wstring>(L"", ErrorInfo::FAILD, L"not find picUrl");
            }
            int index1 = img_url.find(L'\"', index + 10);
            if (index1 == std::wstring::npos) {
                return Result<std::wstring>(L"", ErrorInfo::FAILD, L"not find img");
            }
            img_url = img_url.substr(index + 10, index1 - index - 10);
            img_url += "?param=200y200";
        }
    }

    if(auto res = m_client.Get(img_url)){
        if(res->status == 200){
            std::ofstream cover(m_download_path + L"\\" +  _track + L".jpg", std::ios::out | std::ios::binary);
            if (cover.is_open()) {
                cover << res->body;

                return path;
            }
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return Result<std::wstring>(L"", ErrorInfo::FAILD, converter.from_bytes(strerror(errno)));
            // RmLog(LOG_WARNING, converter.from_bytes(strerror(errno)).c_str());
        }
    }
    return Result<std::wstring>(L"", ErrorInfo::FAILD, L"not find img");
}