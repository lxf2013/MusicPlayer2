#pragma once
#include "IPlayer.h"

class KgMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static KgMusic *c_player;
public:
	//���������
	static KgMusic* Create();
	~KgMusic();
	void Init();
	bool Update(DWORD time) override;

	//���ڲ���
	void OpenPlayer() override;
	void ClosePlayer() override;
};