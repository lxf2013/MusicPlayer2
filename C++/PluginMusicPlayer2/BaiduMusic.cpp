#include "StdAfx.h"
#include "BaiduMusic.h"




/*
**加载与更新
*/
Player* BaiduMusic::c_Player = nullptr;

Player* BaiduMusic::Create()
{
	if (!c_Player)
	{
		c_Player = new BaiduMusic();
		c_Player->Initialize();
	}
	c_Player->AddInstantce();

	return c_Player;
}

BaiduMusic::~BaiduMusic()
{
	c_Player = nullptr;
}


void BaiduMusic::GetWindow()
{
	//获取窗口句柄

	HWND hWnd = NULL;
	HWND hWnd2;
	LONG lWnd;

	do
	{
		hWnd = FindWindowEx(0, hWnd, 0, L"百度音乐");
		lWnd = GetWindowLong(hWnd, GWL_STYLE);
		if ((lWnd | 0x10000000) == 0x960B004C)
		{
			hWnd2 = GetParent(hWnd);
			lWnd = GetWindowLong(hWnd, GWL_STYLE);
			if ((lWnd | 0x10000000) == 0x960B004C)
			{
				m_Window = hWnd2;
				//m_WindowMain = hWnd;
				break;
			}
		}
	} while (hWnd);

	

}

void BaiduMusic::Initialize()
{
	//获取窗口句柄
	GetWindow();


	//获取PlayerPath
	if (IsWindow(m_Window))
	{
		DWORD dwProcessId;

		GetWindowThreadProcessId(m_Window, &dwProcessId);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

		WCHAR sProcPath[MAX_PATH];
		if (GetModuleFileNameEx(hProcess, NULL, sProcPath, MAX_PATH))
			m_PlayerPath = sProcPath;

		CloseHandle(hProcess);
	}
	if (m_PlayerPath.empty())
	{
		HKEY hKey;
		RegOpenKeyEx(HKEY_CLASSES_ROOT,
			L"Applications\\BaiduMusic.exe\\shell\\open\\command",
			0,
			KEY_QUERY_VALUE,
			&hKey);

		DWORD size = 512;
		WCHAR* data = new WCHAR[size];
		DWORD type = 0;

		if (RegQueryValueEx(hKey,
			nullptr,
			nullptr,
			(LPDWORD)&type,
			(LPBYTE)data,
			(LPDWORD)&size) == ERROR_SUCCESS)
		{
			if (type == REG_EXPAND_SZ)
			{
				m_PlayerPath = data;
				m_PlayerPath.resize(m_PlayerPath.length() - 11);	// Get rid of the  /play "%1" at the end
			}
		}

		delete[] data;
		RegCloseKey(hKey);
	}

}

void BaiduMusic::UpdateData()
{
	//检查最后时间
	DWORD time = GetTickCount();
	if (time - last_time < UPDATE_INTERVAL)
		return;

	last_time = time;

	bool abnormal = false;
	wstring LastTrack = m_Track;


	//检查窗口
	if (!IsWindow(m_Window))
	{
		m_Window = NULL;

		if (time - last_init_time > 5000)
		{
			last_init_time = time;
			Initialize();
		}
	}
	if (!m_Window)
	{
		abnormal = true;
		m_Track.clear();
	}


	//获取信息
	if (!abnormal)
	{
		WCHAR Buffer[102];
		Buffer[100] = '\0';
		GetWindowText(m_Window, Buffer, 100);
		//L"%T - %A - 百度音乐"

		wstring data = Buffer;

		wstring::size_type pos;
		pos = data.find(L" - 百度音乐");

		if (pos == wstring::npos)
		{
			abnormal = true;
			//m_Track = Buffer;
		}
		else
		{
			data.resize(pos);
			pos = data.find(L" - ");
		}
		
		if (pos == wstring::npos)
		{
			abnormal = true;
			m_Track = Buffer;
		}
		else if (LastTrack.compare(data))
		{
			m_Track.assign(data);
			m_Title.assign(data, 0, pos);
			m_Artist.assign(data.erase(0, pos + 3));
			abnormal = false;
		}
	}

	if (abnormal)
	{
		//m_Track.clear();
		m_Title.clear();
		m_Artist.clear();
	}


	m_TrackChanged = m_Track.compare(LastTrack) != 0;
}


/*
**全局热键操作
*/
void BaiduMusic::VolumeMute()
{
	SendKey('S', 1, 0, 1);
}
void BaiduMusic::VolumeUp()
{
	SendKey(VK_UP, 1, 0, 1);
}
void BaiduMusic::VolumeDown()
{
	SendKey(VK_DOWN, 1, 0, 1);
}
void BaiduMusic::HideToTray()
{
	SendKey('W', 1, 0, 1);
}
void BaiduMusic::MiniMode()
{
	SendKey('M', 1, 0, 1);
}
void BaiduMusic::ExecuteBang(LPCWSTR args)
{
	if (!_wcsicmp(args, L"DesktopLyricLock"))
	{
		SendKey('E', 1, 0, 1);
	}
	else if (!_wcsicmp(args, L"Reverse"))
	{
		SendKey(VK_F7, 1, 1, 0);
	}
	else if (!_wcsicmp(args, L"Forward"))
	{
		SendKey(VK_F8, 1, 1, 0);
	}
	else
	{
		RmLog(LOG_WARNING, L"MusicPlayer.dll: Unknown Command");
	}
}

/*
**窗口操作
*/
void BaiduMusic::OpenPlayer(wstring& path)
{
	if (!IsWindow(m_Window))
	{
		path = path.empty() ? m_PlayerPath : path;
		ShellExecute(nullptr, L"open", path.empty() ? L"BaiduMusic.exe" : path.c_str(), nullptr, nullptr, SW_SHOW);
	}
	else
	{
		/*ShowWindow(m_WindowMain, SW_SHOWNORMAL);
		BringWindowToTop(m_WindowMain);*/
	}

}

void BaiduMusic::ClosePlayer()
{
	/*if (IsWindow(m_Window))
	{
		HWND hWnd = NULL;
		do
		{
			hWnd = FindWindowEx(m_Window, hWnd, 0, L"百度音乐");
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		} while (hWnd);
		
	}
		*/

	/*PostMessage(m_WindowMain, WM_SYSKEYDOWN,	VK_MENU,	0x60380001);
	PostMessage(m_WindowMain, WM_SYSKEYDOWN,	VK_F4,		0x203E0001);
	PostMessage(m_WindowMain, WM_SYSKEYUP,	VK_F4,		0xE03E0001);
	PostMessage(m_WindowMain, WM_KEYUP,		VK_MENU,	0xC0380001);*/
}

void BaiduMusic::RestorePlayer()
{
	/*if (IsWindow(m_WindowMain))
	{
		PostMessage(m_WindowMain, WM_SYSCOMMAND, SC_RESTORE, 0);
		BringWindowToTop(m_WindowMain);
	}*/

}

void BaiduMusic::MinimizePlayer()
{
	/*if (IsWindow(m_WindowMain))
		PostMessage(m_WindowMain, WM_SYSCOMMAND, SC_MINIMIZE, 0);*/
}