#pragma once
#include "Player.h"

struct KwMusic: public Player
{
	
	static Player* c_Player;

	//���������
	static Player* Create();
	void Initialize();
	void GetWindow();
	void UpdateData();
	~KwMusic();

	
	//���ڲ���
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//��������
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








