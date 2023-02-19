#pragma once
#include "Player.h"

struct BaiduMusic : public Player
{

	static Player* c_Player;

	//���������
	static Player* Create();
	void GetWindow();
	void Initialize();
	void UpdateData();
	~BaiduMusic();
	

	//���ڲ���
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//ȫ���ȼ�����
	virtual void VolumeMute();
	virtual void VolumeUp();
	virtual void VolumeDown();
	virtual void HideToTray();
	virtual void MiniMode();

	void ExecuteBang(LPCWSTR args);

protected:
	//HWND m_WindowMain = NULL;
};