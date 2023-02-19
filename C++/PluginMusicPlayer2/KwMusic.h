#pragma once
#include "Player.h"

struct KwMusic: public Player
{
	
	static Player* c_Player;

	//加载与更新
	static Player* Create();
	void Initialize();
	void GetWindow();
	void UpdateData();
	~KwMusic();

	
	//窗口操作
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//按键操作
	void PlayPause();
	void Stop();
	void Previous();
	void Next();
	void VolumeMute();
	void VolumeUp();
	void VolumeDown();
	void HideToTray();
	void MiniMode();

};








