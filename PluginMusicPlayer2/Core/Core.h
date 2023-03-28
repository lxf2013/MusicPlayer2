#pragma once

#include "../Resource/Resource.h"
#include <map>
#include <string>
#include <vector>
#include "TaskCenter.h"
#include "../Lyric/Lyric.h"
#include "../Player/IPlayer.h"
#include "../Audio/Audio.h"

class Core;

class CoreData{
private:
	int    ref = 0;
	friend class Core;
public:
	Resource     resource;
	Lyric        lyric;
	Audio       *audio  = nullptr;
	IPlayer     *player = nullptr;
};

class Core{
private:
    static std::map<std::wstring, CoreData> m_name_space;
	static TaskCenter m_task_center;
private:
	static std::wstring GetNameSpace(const std::wstring &path);
	static std::wstring GetNameSpace(void *rm);
public:
	static inline CoreData *GetData(const std::wstring &name_space);
	static std::wstring AllocData(Measure *measure);
	static void FreeData(Measure *measure);
	static std::wstring FlushData(Measure *measure);
	static void Update(const std::wstring &name_space);
	static TaskCenter *GetTaskCenter();
};