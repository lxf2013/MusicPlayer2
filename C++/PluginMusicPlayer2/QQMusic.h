#pragma once
#include "Player.h"

struct QQMusic : public Player
{

	static Player* c_Player;
	
	//���������
	static Player* Create();
	void GetWindow();
	void Initialize();
	void UpdateData();
	void UpdateCover();
	~QQMusic();

	
	//���ڲ���
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//ȫ���ȼ�����
	void VolumeUp();
	void VolumeDown();
	void HideToTray();

	void ExecuteBang(LPCWSTR args);

private:
	bool SearchCover(const wstring& artist, const wstring& album, wstring& data);
	bool SearchCover(const wstring& artist, wstring& data);

};


