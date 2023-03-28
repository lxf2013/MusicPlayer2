#pragma once
#include "IPlayer.h"

class KgMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static KgMusic *c_player;
public:
	//加载与更新
	static KgMusic* Create();
	~KgMusic();
	void Init();
	bool Update(DWORD time) override;

	//窗口操作
	void OpenPlayer() override;
	void ClosePlayer() override;
};