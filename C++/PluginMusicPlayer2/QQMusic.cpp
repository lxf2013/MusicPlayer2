#include "StdAfx.h"
#include "QQMusic.h"




/*
**加载与更新
*/

Player* QQMusic::c_Player = nullptr;

Player* QQMusic::Create()
{
	if (!c_Player)
	{
		c_Player = new QQMusic();
		c_Player->Initialize();
	}
	c_Player->AddInstantce();

	return c_Player;
}

QQMusic::~QQMusic()
{
	c_Player = nullptr;
}

void QQMusic::GetWindow()
{
	//获取窗口句柄
	m_Window = FindWindowEx(0, 0, L"QQMusic_Daemon_Wnd", 0);
}

void QQMusic::Initialize()
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
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\Tencent\\QQMusic",
			0,
			KEY_QUERY_VALUE,
			&hKey);

		DWORD size = 512;
		WCHAR* data = new WCHAR[size];
		DWORD type = 0;

		if (RegQueryValueEx(hKey,
			L"Install",
			nullptr,
			(LPDWORD)&type,
			(LPBYTE)data,
			(LPDWORD)&size) == ERROR_SUCCESS)
		{
			if (type == REG_SZ)
			{
				m_PlayerPath = data;
				m_PlayerPath += L"\\QQMusic.exe";
			}
		}

		delete[] data;
		RegCloseKey(hKey);
	}

}

void QQMusic::UpdateData()
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
		//L"%T - %A   "

		wstring data = Buffer;
		data.resize(data.length() - 3);

		wstring::size_type pos;
		pos = data.find(L" - ");

		
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

			//获取封面
			if (m_RequireCover)
				UpdateCover();
		}
	}

	if (abnormal)
	{
		//m_Track.clear();
		m_Title.clear();
		m_Artist.clear();
		m_CoverPath.clear();
	}


	m_TrackChanged = m_Track.compare(LastTrack) != 0;
}



/*
**全局热键操作
*/
void QQMusic::VolumeUp()
{
	SendKey(VK_UP, 1, 0, 1);
}

void QQMusic::VolumeDown()
{
	SendKey(VK_DOWN, 1, 0, 1);
}

void QQMusic::HideToTray()
{
	SendKey('Q', 1, 0, 1);
}

void QQMusic::ExecuteBang(LPCWSTR args)
{
	if (!_wcsicmp(args, L"DesktopLyricShow"))
	{
		SendKey('W', 1, 0, 1);
	}
	else if (!_wcsicmp(args, L"DesktopLyricLock"))
	{
		SendKey('E', 1, 0, 1);
	}
	else if (!_wcsicmp(args, L"MyFavorate"))
	{
		SendKey('V', 1, 0, 1);
	}
	else
	{
		RmLog(LOG_WARNING, L"MusicPlayer.dll: Unknown Command");
	}
}



/*
**窗口操作
*/
void QQMusic::OpenPlayer(wstring& path)
{
	if (!IsWindow(m_Window))
	{
		path = path.empty() ? m_PlayerPath : path;
		ShellExecute(nullptr, L"open", path.empty() ? L"QQMusic.exe" : path.c_str(), nullptr, nullptr, SW_SHOW);
	}
	else
	{
		ShowWindow(m_Window, SW_SHOWNORMAL);
		BringWindowToTop(m_Window);
	}

}

void QQMusic::ClosePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_CLOSE, 0, 0);
}

void QQMusic::RestorePlayer()
{
	if (IsWindow(m_Window))
	{
		PostMessage(m_Window, WM_SYSCOMMAND, SC_RESTORE, 0);
		BringWindowToTop(m_Window);
	}

}

void QQMusic::MinimizePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}




/*
**获取封面
*/

void QQMusic::UpdateCover()
{
	if (!m_RequireCover)
		return;
	
	if (SearchCover(m_Artist, m_CoverPath))
		return;

	m_CoverPath.clear();

}

bool QQMusic::SearchCover(const wstring& artist, wstring& data)
{
	wstring album = L"";
	return SearchCover(artist, album, data);
}

bool QQMusic::SearchCover(const wstring& artist, const wstring& album, wstring& data)
{
	WCHAR buffer[MAX_PATH];
	GetEnvironmentVariable(L"APPDATA", buffer, sizeof(buffer) / sizeof(WCHAR));

	data = buffer;
	data += L"\\Tencent\\QQMusic\\QQMusicPicture\\";
	
	//去除无效字符
	wstring track = artist + L"_" + album;
	WCHAR* InvalidChar = L"\\/:*?\"<>|";
	for (wstring::size_type i = 0; i < track.length(); i++)
	{
		if (wcschr(InvalidChar, track[i]))
		{
			track.erase(i);
			i--;
		}
	}

	//根据是否缺少album信息分为两种情况
	if (album.empty())
	{
		data += track + L"*.jpg";
	}
	else
	{
		data += track + L"_?.jpg";	//4,0,1,2
	}

	//RmLog(0, data.c_str());

	//确认文件是否存在
	WIN32_FIND_DATA result;
	HANDLE hFind;

	hFind = FindFirstFile(data.c_str(), &result);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	FindClose(hFind);

	if (!album.empty())
	{
		//按顺序匹配文件
		WCHAR* num = L"40123";
		wstring::size_type pos = data.length() - 5;
		FILE * fp;

		for (int i = 0; i <= 4; i++)
		{
			data[pos] = num[i];
			if (!_wfopen_s(&fp, data.c_str(), L"rb"))
			{
				fclose(fp);
				return true;
			}
		}
	}

	//缺少album或其他例外
	data = buffer;
	data += L"\\Tencent\\QQMusic\\QQMusicPicture\\";
	data += result.cFileName;

	return true;
}
