#pragma once
#include "Player.h"

struct BaiduMusic : public Player
{

	static Player* c_Player;

	//加载与更新
	static Player* Create();
	void GetWindow();
	void Initialize();
	void UpdateData();
	~BaiduMusic();
	

	//窗口操作
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//全局热键操作
	virtual void VolumeMute();
	virtual void VolumeUp();
	virtual void VolumeDown();
	virtual void HideToTray();
	virtual void MiniMode();

	void ExecuteBang(LPCWSTR args);

protected:
	//HWND m_WindowMain = NULL;
};