#pragma once
#include "Player.h"

struct QQMusic : public Player
{

	static Player* c_Player;
	
	//加载与更新
	static Player* Create();
	void GetWindow();
	void Initialize();
	void UpdateData();
	void UpdateCover();
	~QQMusic();

	
	//窗口操作
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//全局热键操作
	void VolumeUp();
	void VolumeDown();
	void HideToTray();

	void ExecuteBang(LPCWSTR args);

private:
	bool SearchCover(const wstring& artist, const wstring& album, wstring& data);
	bool SearchCover(const wstring& artist, wstring& data);

};


