#include <iostream>
#include <fstream>
#include <codecvt>
#include "Lyric.h"
#include "../../API/RainmeterAPI.h"
using namespace std;

Lyric::Lyric() 
    : m_lyric(1, { 0, L"" })
    , m_current_pos(0)
    , m_last_time(0)
    , m_stop(true)
    , m_stop_time(0) 
    , m_offset(0)
    , m_stat(false) {
}

uint64_t Lyric::GetNum(const char *file_buf, uint32_t file_size, uint32_t &i){
    uint64_t ret = 0;
    while(i<file_size && uint32_t(file_buf[i]-'0') < 10){
        ret *= 10;
        ret += file_buf[i]-'0';
        ++i;
    }
    return ret;
}

bool Lyric::Init(int offset){
    m_offset = offset;
    return true;
}

bool Lyric::GetTranslate(const char *file_buf, uint32_t file_size, uint32_t &i){
    vector<Line> lyric;

    wstring_convert<codecvt_utf8<wchar_t>> converter;
    
    while(i<file_size){
        if(file_buf[i++] != '['){
            continue;
        }
        uint64_t m = GetNum(file_buf, file_size, i);
        if(file_buf[i++] != ':'){
            continue;
        }
        uint64_t s = GetNum(file_buf, file_size, i);
        if(file_buf[i++] != '.'){
            continue;
        }
        uint64_t ms = GetNum(file_buf, file_size, i);
        if(file_buf[i++] != ']'){
            continue;
        }
        ms = (ms < 100)? ms * 10: ms;
        string buf;
        while(i<file_size && file_buf[i] != '[' && file_buf[i] != '\\' && file_buf[i] != '"'){
            buf += file_buf[i++];
        }
        if(buf.empty()){
            continue;
        }
        lyric.push_back({ms + s*1000 + m*60*1000, converter.from_bytes(buf) });
    }

    for(int i=0, j=0; i<lyric.size(); ++i){
        while(j<m_lyric.size() && lyric[i].time != m_lyric[j].time){
            ++j;
        }
        if(j<m_lyric.size()){
            m_lyric[j].tlrc = lyric[i].lrc;
        }else{
            break;
        }
    }
    return true;
}

bool Lyric::Load(const wstring &file){
    m_lyric = vector<Line>(1, {0, L""});
    m_current_pos = 0;
    m_duration = 0;
    m_stat     = false;

    wstring_convert<codecvt_utf8<wchar_t>> converter;
    if(file.empty()){
        return false;
    }
    ifstream lyric(file, ios::in);
    if(lyric.is_open() == false){
        return false;
    }
    lyric.seekg(0, ios::end);
    uint32_t file_size = lyric.tellg();
    lyric.seekg(0, ios::beg);

    char *file_buf = new char[file_size+5]{};
    lyric.read(file_buf, file_size);
    
    for(uint32_t i=0; i<file_size;){
        if(file_buf[i++] != '['){
            continue;
        }
        uint64_t m = GetNum(file_buf, file_size, i);
        if(file_buf[i] != ':'){
            string key;
            while(i<file_size && file_buf[i] != ':' && file_buf[i] != ']'){
                key += file_buf[i++];
            }
            if(key == "translate"){
                (file_buf[i++] == ']') && GetTranslate(file_buf, file_size, i);
                break;
            }
            if(file_buf[i++] != ':'){
                continue;
            }
            
            if(key == "duration"){
                uint64_t num = GetNum(file_buf, file_size, i);
                if(file_buf[i++] != ']'){
                    continue;
                }
                m_duration = num;
            }else{
                string buf;
                while(i<file_size && file_buf[i] != ']' && file_buf[i] != '\\' && file_buf[i] != '"'){
                    buf += file_buf[i++];
                }
                if(buf.empty()){
                    continue;
                }
                m_lyric.push_back({0, converter.from_bytes(buf) });
            }
            continue;
        }
        uint64_t s = GetNum(file_buf, file_size, ++i);
        if(file_buf[i++] != '.'){
            continue;
        }
        uint64_t ms = GetNum(file_buf, file_size, i);
        if(file_buf[i++] != ']'){
            continue;
        }
        ms = (ms < 100)? ms * 10: ms;
        string buf;
        while(i<file_size && file_buf[i] != '[' && file_buf[i] != '\\' && file_buf[i] != '"'){
            buf += file_buf[i++];
        }
        if(buf.empty()){
            continue;
        }
        m_lyric.push_back({ms + s*1000 + m*60*1000, converter.from_bytes(buf) });
    }

    delete[] file_buf;
    m_stop        = false;
    m_stat        = true;
    
    m_lyric.push_back({m_duration, L""});
    return true;
}

void Lyric::Stop(){
    if(m_stop == false){
        m_stop_time = GetTickCount();
        m_stop = true;
    }
}

void Lyric::Start(){
    if(m_stop == true){
        m_last_time += GetTickCount() - m_stop_time;
        m_stop_time = 0;
        m_stop = false;
    }
}

void Lyric::StopOrStart(){
    if(m_stop == true){
        Start();
    }else{
        Stop();
    }
}

bool Lyric::Update(){
	DWORD time = GetTickCount();
    if(m_current_pos == 0){
        m_last_time = time + m_offset;
        ++m_current_pos;
        return true;
    }
    if(m_current_pos+1 >= m_lyric.size()){
        m_current_pos = 0;
        return true;
    }
    if(m_stop == false && time >= m_last_time + m_lyric[m_current_pos+1].time){
        ++m_current_pos;
        return true;
    }
    return false;
}

bool Lyric::GetStat(){
    return m_stat;
}

LPCWSTR Lyric::GetLyric(int32_t pos){
    if(uint32_t(m_current_pos + pos) >= m_lyric.size()){
        return m_lyric[0].lrc.c_str();
    }
    return m_lyric[m_current_pos + pos].lrc.c_str();
}

LPCWSTR Lyric::GetTLyric(int32_t pos){
    if(uint32_t(m_current_pos + pos) >= m_lyric.size()){
        return m_lyric[0].tlrc.c_str();
    }
    return m_lyric[m_current_pos + pos].tlrc.c_str();
}
