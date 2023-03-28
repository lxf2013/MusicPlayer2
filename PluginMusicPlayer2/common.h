#pragma once

// #include "httplib.h"
// #include <windows.h>
// #include <string>
// #include <vector>
// #include <process.h>
// #include <Psapi.h>
// #include "../../API/RainmeterAPI.h"

#include <string>
#include <windows.h>
#include "ThirdPart/RainmeterAPI.h"

struct ErrorInfo{
	enum ErrorCode{
		FAILD = -2,
		WARNING = -1,
		OK = 0,
	};

	ErrorCode err_code;
	std::wstring msg;

	ErrorInfo(): 
		err_code(OK),
		msg(L""){
	}
	ErrorInfo(ErrorCode code, const std::wstring &msg):
		err_code(code),
		msg(msg){
	}
};

template<typename T>
struct Result: public ErrorInfo{
	T result;

	Result(){
	}
	Result(T result): 
		result(result){
	}
	Result(T result, ErrorCode code, const std::wstring &msg):
		result(result),
		ErrorInfo(code, msg){
	}
};

template<>
struct Result<void>: public ErrorInfo{
	Result(){
	}
	Result(ErrorCode code, const std::wstring &msg):
		ErrorInfo(code, msg){
	}
};

class MemoryCount{
private:
	static MemoryCount memory_count;
	int measure_cnt;
	int audio_cnt;
	int task_cnt;
	int player_cnt;
	int node_cnt;
public:
	MemoryCount():
		measure_cnt(0),
		audio_cnt(0),
		task_cnt(0),
		player_cnt(0),
		node_cnt(0)
	{
	}
	~MemoryCount(){
		WCHAR buffer[12] = {};
		_itow_s(measure_cnt, buffer, 10);
		RmLog(LOG_NOTICE, (std::wstring(L"MemoryCount: Measure = ") + buffer).c_str());
		_itow_s(audio_cnt, buffer, 10);
		RmLog(LOG_NOTICE, (std::wstring(L"MemoryCount: Audio = ") + buffer).c_str());
		_itow_s(task_cnt, buffer, 10);
		RmLog(LOG_NOTICE, (std::wstring(L"MemoryCount: Task = ") + buffer).c_str());
		_itow_s(player_cnt, buffer, 10);
		RmLog(LOG_NOTICE, (std::wstring(L"MemoryCount: Player = ") + buffer).c_str());
		_itow_s(node_cnt, buffer, 10);
		RmLog(LOG_NOTICE, (std::wstring(L"MemoryCount: Node = ") + buffer).c_str());
	}
	static void AllocMeasure(){
		++memory_count.measure_cnt;
	}
	static void FreeMeasure(){
		--memory_count.measure_cnt;
	}
	static void AllocAudio(){
		++memory_count.audio_cnt;
	}
	static void FreeAudio(){
		--memory_count.audio_cnt;
	}
	static void AllocTask(){
		++memory_count.task_cnt;
	}
	static void FreeTask(){
		--memory_count.task_cnt;
	}
	static void AllocPlayer(){
		++memory_count.player_cnt;
	}
	static void FreePlayer(){
		--memory_count.player_cnt;
	}
	static void AllocNode(){
		++memory_count.node_cnt;
	}
	static void FreeNode(){
		--memory_count.node_cnt;
	}
};

void SendKey(WORD key);

void SendKey(HWND hWnd, UINT key);

void SendKey(HWND hWnd, UINT key, bool ctrl);

void SendKey(BYTE key, bool ctrl, bool shift, bool alt);