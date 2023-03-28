#pragma once
#include "IPlayer.h"

class KwMusic: public IPlayer{
private:
    DWORD m_last_time = 0;
	static KwMusic *c_player;
public:
	//加载与更新
	static KwMusic* Create();
	~KwMusic();
	void Init();
	bool Update(DWORD time) override;

	//窗口操作
	void OpenPlayer() override;
	void ClosePlayer() override;


	//按键操作
	void PlayPause() override;
	void Stop() override;
	void Previous() override;
	void Next() override;
};
