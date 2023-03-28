#pragma once
#include "IPlayer.h"

class BaiduMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static BaiduMusic* c_player;
public:
	//加载与更新
	static BaiduMusic* Create();
	~BaiduMusic();
	void Init();
	bool Update(DWORD time) override;
	

	//窗口操作
	void OpenPlayer() override;
	void ClosePlayer() override;
};