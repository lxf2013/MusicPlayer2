#pragma once
// #include "StdAfx.h"
#include <string>
#include "../common.h"

class IPlayer{
protected:
	HWND m_window = NULL;
	int m_ref = 0;
	std::wstring m_title;
	std::wstring m_artist;
	std::wstring m_track;

protected:
	void AddInstantce()	{ 
		++m_ref;
	}
public:
	virtual ~IPlayer() = default;
	void RemoveInstantce() { 
		if (--m_ref <= 0) { 
			MemoryCount::FreePlayer();
			delete this; 
		} 
	}
	virtual bool Update(DWORD time) = 0;

	//返回信息
	virtual const std::wstring &GetTitle(){
		return m_title;
	}
	virtual const std::wstring &GetArtist(){
		return m_artist;
	}
	virtual const std::wstring &GetTrack(){
		return m_track;
	}
	int GetStatus() { 
		if(m_window == false){
			Update(GetTickCount());
		}
		return m_window ? 1 : 0; 
	}


	//窗口操作
	virtual void OpenPlayer() = 0;
	virtual void ClosePlayer() = 0;


	//按键操作
	virtual void PlayPause() { 
		SendKey(VK_MEDIA_PLAY_PAUSE); 
	}
	virtual void Stop() { 
		SendKey(VK_MEDIA_STOP); 
	}
	virtual void Next() { 
		SendKey(VK_MEDIA_NEXT_TRACK); 
	}
	virtual void Previous() { 
		if(GetStatus() == false){
			Update(GetTickCount());
		}
		SendKey(VK_MEDIA_PREV_TRACK); 
	}
};
