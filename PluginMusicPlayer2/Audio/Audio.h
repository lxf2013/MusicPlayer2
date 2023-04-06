#pragma once 

#include <windows.h>
#include <AudioClient.h>
#include <AudioPolicy.h>
#include <MMDeviceApi.h>
#include <string>

class Audio{
private:
    IMMDeviceEnumerator*	m_enum = nullptr;
    IMMDevice*				m_dev = nullptr;
    IAudioClient*			m_clAudio = nullptr;
    WAVEFORMATEX*			m_wfx = nullptr;
    IAudioCaptureClient*	m_clCapture = nullptr;
    bool                    m_success = false;
    DWORD                   m_last_time = 0;
    std::wstring            m_last_dev;
    int                     m_cnt = 0;
    bool                    m_status = false;
    int                     m_ref = 0;
	static Audio           *c_audio;
private:
    void AddInstance();
public:
    bool Update(DWORD time);
    int  GetStatus();
    bool Init();
    void Release();
	void RemoveInstance();
	static Audio* Create();
};
