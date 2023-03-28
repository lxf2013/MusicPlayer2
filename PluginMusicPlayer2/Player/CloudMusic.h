#pragma once
#include "IPlayer.h"

class CloudMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static CloudMusic *c_player;
public:
	//加载与更新
	static CloudMusic* Create();
	~CloudMusic();
	void Init();
	bool Update(DWORD time) override;

	//窗口操作
	void OpenPlayer() override;
	void ClosePlayer() override;
};
