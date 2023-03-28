#pragma once
#include "IPlayer.h"

class QQMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static QQMusic *c_player;
public:
	//���������
	static QQMusic* Create();
	~QQMusic();
	void Init();
	bool Update(DWORD time) override;

	//���ڲ���
	void OpenPlayer() override;
	void ClosePlayer() override;
};
