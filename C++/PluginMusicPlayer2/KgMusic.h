#pragma once
#include "Player.h"

struct KgMusic : public Player
{

	static Player* c_Player;

	//���������
	static Player* Create();
	void Initialize();
	void GetWindow();
	void UpdateData();
	void UpdateCover();
	~KgMusic();
	KgMusic();
	

	//���ڲ���
	void OpenPlayer(wstring& path);
	void ClosePlayer();
	void RestorePlayer();
	void MinimizePlayer();


	//��������
	void VolumeMute();
	void VolumeUp();
	void VolumeDown();
	
	void ExecuteBang(LPCWSTR args);

private:
	bool SearchCover(const wstring& artist, wstring& data);
	bool ExtractCover(const wstring& artis, const wstring& tempfile, wstring& data);

	wstring m_TempCoverPath;
};