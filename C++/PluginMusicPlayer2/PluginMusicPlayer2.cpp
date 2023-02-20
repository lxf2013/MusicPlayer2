#include "PluginMusicPlayer2.h"
#include "KwMusic.h"
#include "QQMusic.h"
#include "BaiduMusic.h"
#include "KgMusic.h"

vector<Manager*> Manager::managers;

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;
	measure->rm = rm;
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;

	if(measure->manager){
		Manager::FreeManager(measure->manager->name);
	}
	delete measure;
}


PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;
	measure->rm = rm;
	
	//³õÊ¼»¯ PlayerType
	LPCWSTR type = RmReadString(rm, L"Type", L"");

	if (_wcsicmp(L"Manager", type) == 0)
	{
		measure->type = MEASURE_MANAGER;
		Manager *manager = Manager::GetManager(RmGetMeasureName(rm));
		if(manager){
			wstring error = RmGetMeasureName(rm);
			error += L" ÒÑ´æÔÚ£¬¿ÉÄÜÔÚ±¾ÎÄ¼þÖÐ£¬Ò²¿ÉÄÜÔÚÆäËüÎÄ¼þÖÐ";
			RmLog(rm, LOG_WARNING, error.c_str());
			measure->manager = manager;
			return;
		}

		manager = Manager::AllocManager(RmGetMeasureName(rm));
		measure->manager = manager;

		//³õÊ¼»¯ PlayerName
		LPCWSTR str = RmReadString(rm, L"PlayerName", L"");

		if (!_wcsicmp(L"KwMusic", str))
		{
			manager->player = KwMusic::Create();
		}
		else if (!_wcsicmp(L"KgMusic", str))
		{
			manager->player = KgMusic::Create();
		}
		else if (!_wcsicmp(L"QQMusic", str))
		{
			manager->player = QQMusic::Create();
		}
		else if (!_wcsicmp(L"BaiduMusic", str))
		{
			manager->player = BaiduMusic::Create();
		}
		else
		{
			wstring error = L"ÎÞÐ§µÄ PlayerName: ";
			error += str;
			RmLog(rm, LOG_ERROR, error.c_str());
			manager->player = QQMusic::Create();
		}
		
		int offset = RmReadInt(rm, L"LyricOffset", -500);
		double similarity = RmReadDouble(rm, L"Similarity", 0.9);
		LPCWSTR path = RmPathToAbsolute(rm, RmReadString(rm, L"DownloadPath", L"download"));
		int     capacity = RmReadInt(rm, L"Capacity", 100);

		manager->downloader.Init(path, capacity, similarity);
		manager->lyric.Init(offset);

		manager->playerPath = RmReadString(rm, L"PlayerPath", L"");
		manager->trackChangeAction = RmReadString(rm, L"TrackChangeAction", L"");
	}
	else if (_wcsicmp(L"Title", type) == 0)
	{
		measure->type = MEASURE_TITLE;
	}
	else if (_wcsicmp(L"Artist", type) == 0)
	{
		measure->type = MEASURE_ARTIST;
	}
	else if (_wcsicmp(L"Track", type) == 0)
	{
		measure->type = MEASURE_TRACK;
	}
	else if (_wcsicmp(L"PLAYERPATH", type) == 0)
	{
		measure->type = MEASURE_PLAYERPATH;
	}
	else if (_wcsicmp(L"Status", type) == 0)
	{
		measure->type = MEASURE_STATUS;
	}
	else if (_wcsicmp(L"Cover", type) == 0)
	{
		measure->type = MEASURE_COVER;
	}
	else if (_wcsicmp(L"lrc_6", type) == 0)
	{
		measure->type = MEASURE_LRC_6;
	}
	else if (_wcsicmp(L"lrc_5", type) == 0)
	{
		measure->type = MEASURE_LRC_5;
	}
	else if (_wcsicmp(L"lrc_4", type) == 0)
	{
		measure->type = MEASURE_LRC_4;
	}
	else if (_wcsicmp(L"lrc_3", type) == 0)
	{
		measure->type = MEASURE_LRC_3;
	}
	else if (_wcsicmp(L"lrc_2", type) == 0)
	{
		measure->type = MEASURE_LRC_2;
	}
	else if (_wcsicmp(L"lrc_1", type) == 0)
	{
		measure->type = MEASURE_LRC_1;
	}
	else if (_wcsicmp(L"lrc0", type) == 0)
	{
		measure->type = MEASURE_LRC0;
	}
	else if (_wcsicmp(L"lrc1", type) == 0)
	{
		measure->type = MEASURE_LRC1;
	}
	else if (_wcsicmp(L"lrc2", type) == 0)
	{
		measure->type = MEASURE_LRC2;
	}
	else if (_wcsicmp(L"lrc3", type) == 0)
	{
		measure->type = MEASURE_LRC3;
	}
	else if (_wcsicmp(L"lrc4", type) == 0)
	{
		measure->type = MEASURE_LRC4;
	}
	else if (_wcsicmp(L"lrc5", type) == 0)
	{
		measure->type = MEASURE_LRC5;
	}
	else if (_wcsicmp(L"lrc6", type) == 0)
	{
		measure->type = MEASURE_LRC6;
	}
	else if (_wcsicmp(L"lrc7", type) == 0)
	{
		measure->type = MEASURE_LRC7;
	}
	else if (_wcsicmp(L"lrc8", type) == 0)
	{
		measure->type = MEASURE_LRC8;
	}
	else if (_wcsicmp(L"lrc9", type) == 0)
	{
		measure->type = MEASURE_LRC9;
	}
	else
	{
		std::wstring error = L"无效的 Type: ";
		error += type;
		RmLog(rm, LOG_WARNING, error.c_str());
	}
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;
	if(measure->type != MEASURE_MANAGER){
		return 0.0;
	}

	Player* player = measure->manager->player;
	player->UpdateData();

	if(measure->manager->requireCover && player->m_TrackChanged){
		measure->manager->downloader.DownloadCover(player->GetTrack());
	}
	if(measure->manager->requireLyric){
		// if(player->m_TrackChanged && measure->manager->downloader.DownloadLyric(player->GetTrack())){
		if(player->m_TrackChanged){
			measure->manager->downloader.DownloadLyric(player->GetTrack());
			measure->manager->lyric.Load(measure->manager->downloader.GetLyricPath());
		}
		measure->manager->lyric.Update();
	}
	if (player->m_TrackChanged && !measure->manager->trackChangeAction.empty()){
		RmExecute(RmGetSkin(measure->rm), measure->manager->trackChangeAction.c_str());
	}

	return 0.0;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;
	if(measure->manager == nullptr){
		measure->manager = Manager::GetManager(RmReadString(measure->rm, L"Manager", L""));
		if(measure->manager == nullptr){
			wstring error = L"Î´ÕÒµ½ Manager: ";
			error += RmReadString(measure->rm, L"Manager", L"");
			RmLog(measure->rm, LOG_WARNING, error.c_str());
			return nullptr;
		}
	}

	Player* player = measure->manager->player;
	Manager *manager = measure->manager;
	static WCHAR buffer[12];
	LPCWSTR str;

	switch (measure->type)
	{
	case MEASURE_TITLE:
		return player->GetTitle();
		break;

	case MEASURE_ARTIST:
		return player->GetArtist();
		break;

	case MEASURE_TRACK:
		return player->GetTrack();
		break;

	case MEASURE_PLAYERPATH:
		str = player->GetPlayerPath();
		return !*str ? manager->playerPath.c_str() : str;
		break;

	case MEASURE_STATUS:
		_itow_s(player->GetStatus(), buffer, 10);
		return buffer;
		break;

	case MEASURE_COVER:
		manager->requireCover = true;
		return manager->downloader.GetCoverPath();
		break;

	case MEASURE_LRC_6:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-6);
		break;

	case MEASURE_LRC_5:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-5);
		break;

	case MEASURE_LRC_4:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-4);
		break;

	case MEASURE_LRC_3:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-3);
		break;

	case MEASURE_LRC_2:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-2);
		break;

	case MEASURE_LRC_1:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(-1);
		break;

	case MEASURE_LRC0:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(0);
		break;

	case MEASURE_LRC1:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(1);
		break;

	case MEASURE_LRC2:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(2);
		break;

	case MEASURE_LRC3:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(3);
		break;

	case MEASURE_LRC4:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(4);
		break;

	case MEASURE_LRC5:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(5);
		break;

	case MEASURE_LRC6:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(6);
		break;

	case MEASURE_LRC7:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(7);
		break;

	case MEASURE_LRC8:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(8);
		break;

	case MEASURE_LRC9:
		manager->requireLyric = true;
		return manager->lyric.GetLyric(9);
		break;
	}

	return nullptr;
}


PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
{
	Measure* measure = (Measure*)data;
	if(measure->manager == nullptr){
		RmLog(measure->rm, LOG_WARNING, L" ManagerÎª¿Õ");
		return;
	}
	Player* player = measure->manager->player;
	
	if (!_wcsnicmp(args, L"Open",4))
	{
		player->OpenPlayer(measure->manager->playerPath);
	}
	else if (player->GetStatus())
	{
		if (!_wcsnicmp(args, L"Close",5))
		{
			player->ClosePlayer();
			measure->manager->lyric.Stop();
		}
		else if (!_wcsicmp(args, L"Restore"))
		{
			player->RestorePlayer();
		}
		else if (!_wcsicmp(args, L"Minimize"))
		{
			player->MinimizePlayer();
		}
		else if (!_wcsicmp(args, L"PlayPause"))
		{
			player->PlayPause();
			measure->manager->lyric.StopOrStart();
		}
		else if (!_wcsicmp(args, L"Stop"))
		{
			player->Stop();
			measure->manager->lyric.Stop();
		}
		else if (!_wcsicmp(args, L"Previous"))
		{
			player->Previous();
			measure->manager->lyric.Start();
		}
		else if (!_wcsicmp(args, L"Next"))
		{
			player->Next();
			measure->manager->lyric.Start();
		}
		else if (!_wcsicmp(args, L"VolumeMute"))
		{
			player->VolumeMute();
		}
		else if (!_wcsicmp(args, L"VolumeUp"))
		{
			player->VolumeUp();
		}
		else if (!_wcsicmp(args, L"VolumeDown"))
		{
			player->VolumeDown();
		}
		else if (!_wcsnicmp(args, L"Hide",4))
		{
			player->HideToTray();
		}
		else if (!_wcsicmp(args, L"MiniMode"))
		{
			player->MiniMode();
		}
		else
		{
			player->ExecuteBang(args);
		}
	}
	else
	{
		RmLog(measure->rm, LOG_WARNING, L"Player isn't running");
	}
}





/*
**·¢ËÍ°´¼üÏûÏ¢
*/
//ÏòÈ«¾Ö·¢ËÍµ¥Ò»¼ü»÷
void SendKey(WORD key)
{
	KEYBDINPUT kbi = { 0 };
	kbi.wVk = key;
	kbi.dwExtraInfo = (ULONG_PTR)GetMessageExtraInfo();

	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki = kbi;

	SendInput(1, &input, sizeof(INPUT));
}
//Ïò´°¿Ú·¢ËÍµ¥Ò»¼ü»÷
void SendKey(HWND hWnd, UINT key)
{
	UINT VSC = MapVirtualKey(key, 0);
	PostMessage(hWnd, WM_KEYDOWN, key, 0x00000001 | VSC << 16);
	PostMessage(hWnd, WM_KEYUP, key, 0xC0000001 | VSC << 16);
}
//Ïò´°¿Ú·¢ËÍ´øCtrlµÄ¼ü»÷
void SendKey(HWND hWnd, UINT key, bool ctrl)
{
	UINT VSC_CTRL = MapVirtualKey(VK_CONTROL, 0) << 16;
	UINT VSC = MapVirtualKey(key, 0) << 16;

	if (key >= 'A' && key <= 'Z')
	{	//×ÖÄ¸

		PostMessage(hWnd, WM_KEYDOWN, VK_CONTROL, 0x00000001 | VSC_CTRL);
		PostMessage(hWnd, WM_KEYDOWN, key, 0x00000001 | VSC);
		//PostMessage(hWnd, WM_CHAR,		key - 0x40,	0x00000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, key, 0xC0000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, VK_CONTROL, 0xC0000001 | VSC_CTRL);
	}
	else
	{	//·½Ïò¼ü
		PostMessage(hWnd, WM_KEYDOWN, VK_CONTROL, 0x00000001 | VSC_CTRL);
		PostMessage(hWnd, WM_KEYDOWN, key, 0x01000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, key, 0xC1000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, VK_CONTROL, 0xC0000001 | VSC_CTRL);
	}
}
//ÏòÈ«¾Ö·¢ËÍ´øÐÞÊÎ¼üµÄ¼ü»÷
void SendKey(BYTE key, bool ctrl, bool shift, bool alt)
{
	if (ctrl)
		keybd_event(VK_CONTROL, 0, 0, 0);
	if (shift)
		keybd_event(VK_SHIFT, 0, 0, 0);
	if (alt)
		keybd_event(VK_MENU, 0, 0, 0);

	keybd_event(key, 0, 0, 0);
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);


	if (alt)
		keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
	if (shift)
		keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	if (ctrl)
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

}