#pragma once
#include "StdAfx.h"




//发送按键
void SendKey(WORD key);
void SendKey(HWND hWnd, UINT key);
void SendKey(HWND hWnd, UINT key, bool ctrl);
void SendKey(BYTE key, bool ctrl, bool shift, bool alt);



struct Player
{
	bool m_TrackChanged = false;
	bool m_RequireCover = false;

	//加载与更新
	virtual void Initialize() = 0;
	virtual void GetWindow() = 0;
	virtual void UpdateData() {}
	//virtual void UpdateCover() = 0;
	void AddInstantce()	{ m_InstanceCount++; }
	void RemoveInstantce() { if (--m_InstanceCount <= 0) { delete this; } }

	//返回信息
	LPCWSTR GetTitle() { return m_Title.c_str(); }
	LPCWSTR GetArtist() { return m_Artist.c_str(); }
	LPCWSTR GetTrack() { return m_Track.c_str(); }
	LPCWSTR GetPlayerPath() { return m_PlayerPath.c_str(); }
	LPCTSTR GetCoverPath() { return m_CoverPath.c_str(); }
	int GetStatus() { return m_Window ? 1 : 0; }


	//窗口操作
	virtual void OpenPlayer(wstring& path) {}
	virtual void ClosePlayer() {}
	virtual void RestorePlayer() {}
	virtual void MinimizePlayer() {}


	//按键操作
	virtual void PlayPause() { SendKey(VK_MEDIA_PLAY_PAUSE); }
	virtual void Stop() { SendKey(VK_MEDIA_STOP); }
	virtual void Next() { SendKey(VK_MEDIA_NEXT_TRACK); }
	virtual void Previous() { SendKey(VK_MEDIA_PREV_TRACK); }

	virtual void VolumeMute() {}
	virtual void VolumeUp() {}
	virtual void VolumeDown() {}
	virtual void HideToTray() {}
	virtual void MiniMode() {}

	virtual void ExecuteBang(LPCWSTR args) { RmLog(LOG_WARNING, L"MusicPlayer.dll: Unknown Command"); }

protected:
	int m_InstanceCount = 0;
	HWND m_Window = NULL;

	wstring m_Track;
	wstring m_Title;
	wstring m_Artist;
	wstring m_PlayerPath;
	wstring m_CoverPath;

	DWORD last_time = 0;
	DWORD last_init_time = 0;

};
