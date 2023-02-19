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
    , m_offset(0) {
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

bool Lyric::Load(const wstring &file){
    m_lyric = vector<Line>(1, {0, L""});
    m_current_pos = 0;
    
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
        // while(i<file_size && file_buf[i] != '[' && file_buf[i] != '\\' && file_buf[i] != '"' && file_buf[i] != '\r' && file_buf[i] != '\n'){
        while(i<file_size && file_buf[i] != '[' && file_buf[i] != '\\' && file_buf[i] != '"'){
            buf += file_buf[i++];
        }
        if(buf.empty()){
            continue;
        }
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        m_lyric.push_back({ms + s*1000 + m*60*1000, converter.from_bytes(buf) });
    }

    delete[] file_buf;
    m_stop        = false;
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

void Lyric::Update(){
	DWORD time = GetTickCount();
    if(m_current_pos == 0){
        m_last_time = time + m_offset;
        ++m_current_pos;
        return ;
    }
    if(m_current_pos >= m_lyric.size()){
        m_current_pos = 0;
        return ;
    }
    if(m_stop == false && time >= m_last_time + m_lyric[m_current_pos].time){
        ++m_current_pos;
    }
}
LPCWSTR Lyric::GetLyric(int32_t pos){
    if(uint32_t(m_current_pos + pos) >= m_lyric.size()){
        return m_lyric[0].lrc.c_str();
    }
    return m_lyric[m_current_pos + pos].lrc.c_str();
}
