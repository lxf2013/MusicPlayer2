
#include "Audio.h"
#include "../common.h"
#include <FunctionDiscoveryKeys_devpkey.h>

const CLSID CLSID_MMDeviceEnumerator	= __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator		= __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient				= __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient       = __uuidof(IAudioCaptureClient);

#define SAFE_RELEASE(p)			if((p)!=NULL) { (p)->Release(); (p) = NULL; }

Audio *Audio::c_audio = nullptr;

Audio* Audio::Create(){
    if(c_audio == nullptr){
        c_audio = new Audio;
        MemoryCount::AllocAudio();
        c_audio->Init();
    }
    c_audio->AddInstance();
    return c_audio;
}

void Audio::AddInstance(){
    ++m_ref;
}

void Audio::RemoveInstance() { 
    if (--m_ref <= 0) { 
        Release();
        delete this; 
        MemoryCount::FreeAudio();
        c_audio = nullptr;
    } 
}

bool Audio::Init(){
    if(CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&m_enum) == S_OK) {
        HRESULT hr = m_enum->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_dev);
        // HRESULT hr = m_enum->GetDefaultAudioEndpoint(eRender, eConsole, &m_dev);
        if(FAILED(hr)) { 
            return false;
        }
        
        hr = m_dev->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_clAudio);
        if(FAILED(hr)){
            return false;
        }

        hr = m_clAudio->GetMixFormat(&m_wfx);
        if(FAILED(hr)) { 
            return false;
        }

        // initialize the audio client
        hr = m_clAudio->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 10000000, 0, m_wfx, NULL);
        if(FAILED(hr)){
            return false;
        }

        hr = m_clAudio->GetService(IID_IAudioCaptureClient, (void**)&m_clCapture);
        if(FAILED(hr)) { 
            return false;
        }
        
        hr = m_clAudio->Start();
        if(FAILED(hr)) { 
            return false;
        }
        // SAFE_RELEASE(m_enum);
        return m_success = true;
    }
    return false;
}
bool Audio::Update(DWORD time){
    // DWORD time = GetTickCount();
    if(m_success == false){
        return false;
    }
    if(time < m_last_time+100){
        return m_status;
    }
    m_last_time = time;

    while(m_cnt == 10){
        IMMDevice*		dev = nullptr;
        HRESULT hr = m_enum->GetDefaultAudioEndpoint(eRender, eMultimedia, &dev);
        if(FAILED(hr)) { 
            break;
        }

        IPropertyStore*	props		= NULL;
        if(dev->OpenPropertyStore(STGM_READ, &props) == S_OK) {
            PROPVARIANT	varName;	PropVariantInit(&varName);
            if(props->GetValue(PKEY_Device_FriendlyName, &varName) == S_OK) {
                WCHAR devName[64] = {};
                _snwprintf_s(devName, _TRUNCATE, L"%s", varName.pwszVal);
                if(m_last_dev != devName){
                    Release();
                    Init();
                    m_last_dev = devName;
                }
            }
            PropVariantClear(&varName);
        }
        SAFE_RELEASE(props);
        SAFE_RELEASE(dev);
        break;
    }

    BYTE*			buffer;
    UINT32			nFrames;
    DWORD			flags;
    UINT64			pos;
    double          temp = 0;
    while (m_clCapture->GetBuffer(&buffer, &nFrames, &flags, &pos, NULL) == S_OK) {
        float* s = (float*)buffer;
        for (unsigned int iFrame = 0; iFrame < nFrames; ++iFrame) {
            temp += (float)*s++ + (float)*s++;
        }
        m_clCapture->ReleaseBuffer(nFrames);
    }
    if((unsigned double)temp > 0.1){
        m_cnt = 0;
        m_status = true;
    }else if(++m_cnt > 5){
        m_cnt > 10 && (m_cnt = 0);
        m_status = false;
    }
    return m_status;
}
int Audio::GetStatus(){
    return m_status;
}
void Audio::Release(){
    if(m_clAudio){
        m_clAudio->Stop();
    }
    SAFE_RELEASE(m_clCapture);
    if(m_wfx){
        CoTaskMemFree(m_wfx);
        m_wfx = nullptr;
    }
    SAFE_RELEASE(m_clAudio);
    SAFE_RELEASE(m_dev);
    SAFE_RELEASE(m_enum);
}
