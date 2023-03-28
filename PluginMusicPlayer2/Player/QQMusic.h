#pragma once
#include "IPlayer.h"

class QQMusic : public IPlayer{
private:
    DWORD m_last_time = 0;
	static QQMusic *c_player;
public:
	//加载与更新
	static QQMusic* Create();
	~QQMusic();
	void Init();
	bool Update(DWORD time) override;

	//窗口操作
	void OpenPlayer() override;
	void ClosePlayer() override;
};
