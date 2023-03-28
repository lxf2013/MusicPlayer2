#pragma once
#include "IPlayer.h"

class KwMusic: public IPlayer{
private:
    DWORD m_last_time = 0;
	static KwMusic *c_player;
public:
	//���������
	static KwMusic* Create();
	~KwMusic();
	void Init();
	bool Update(DWORD time) override;

	//���ڲ���
	void OpenPlayer() override;
	void ClosePlayer() override;


	//��������
	void PlayPause() override;
	void Stop() override;
	void Previous() override;
	void Next() override;
};
