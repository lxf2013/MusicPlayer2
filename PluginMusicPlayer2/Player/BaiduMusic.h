#pragma once
#include "IPlayer.h"

class BaiduMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static BaiduMusic* c_player;
public:
	//���������
	static BaiduMusic* Create();
	~BaiduMusic();
	void Init();
	bool Update(DWORD time) override;
	

	//���ڲ���
	void OpenPlayer() override;
	void ClosePlayer() override;
};