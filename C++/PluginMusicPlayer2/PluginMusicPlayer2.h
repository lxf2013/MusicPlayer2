#pragma once

#include "StdAfx.h"
#include "Player.h"
#include "Download.h"
#include "Lyric.h"

// enum PlayerName
// {
// 	PLAYER_KUWO,
// 	PLAYER_KUGOU,
// 	PLAYER_QQ,
// 	PLAYER_BAIDU,
// };

enum MeasureType
{
	MEASURE_NONE,
	MEASURE_MANAGER,
	MEASURE_TITLE,
	MEASURE_ARTIST,
	MEASURE_TRACK,
	MEASURE_PLAYERPATH,
	MEASURE_STATUS,
	MEASURE_COVER,
	MEASURE_LRC_6,
	MEASURE_LRC_5,
	MEASURE_LRC_4,
	MEASURE_LRC_3,
	MEASURE_LRC_2,
	MEASURE_LRC_1,
	MEASURE_LRC0,
	MEASURE_LRC1,
	MEASURE_LRC2,
	MEASURE_LRC3,
	MEASURE_LRC4,
	MEASURE_LRC5,
	MEASURE_LRC6,
	MEASURE_LRC7,
	MEASURE_LRC8,
	MEASURE_LRC9,
};

class Manager
{
public:
	bool requireCover;
	bool requireLyric;
	uint32_t lyricOffset;
	wstring downloadPath;
	wstring name;
	wstring playerPath;
	wstring trackChangeAction;

	Downloader downloader;
	Lyric      lyric;
	Player     *player;
private:
	int ref;
private:
	Manager()
		: requireCover(false)
		, requireLyric(false)
		,lyricOffset(0)
		,downloader()
		,lyric()
		,player(nullptr)
		,ref(0)
	{
	}
	~Manager(){
		if(player){
			player->RemoveInstantce();
		}
	}
public:
	static Manager *GetManager(const wstring &name){
		for(auto &it: managers){
			if(it->name == name){
				++it->ref;
				return it;
			}
		}
		return nullptr;
	}
	static Manager *AllocManager(const wstring &name){
		for(auto &it: managers){
			if(it->name == name){
				++it->ref;
				return it;
			}
		}
		managers.push_back(new Manager);
		++managers.back()->ref;
		return managers.back();
	}
	static void FreeManager(const wstring &name){
		for(auto &it=managers.begin(); it != managers.end(); ++it){
			if((*it)->name != name){
				continue;
			}
			if(--(*it)->ref == 0){
				delete *it;
				managers.erase(it);
			}
			break;
		}
	}
private:
	static vector<Manager *> managers;
};

struct Measure
{
	MeasureType type = MEASURE_NONE;
	void *rm = nullptr;
	Manager *manager = nullptr;
};
