#include "StdAfx.h"
#include "KwMusic.h"




/*
**加载与更新
*/
Player* KwMusic::c_Player = nullptr;

Player* KwMusic::Create()
{
	if (!c_Player)
	{
		c_Player = new KwMusic();
		c_Player->Initialize();
	}
	c_Player->AddInstantce();

	return c_Player;
}

KwMusic::~KwMusic()
{
	c_Player = nullptr;
}

void KwMusic::GetWindow()
{
	//获取窗口句柄
	m_Window = FindWindowEx(0, 0, L"kwmusicmaindlg", 0);
}

void KwMusic::Initialize()
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
			L"kuwo\\Shell\\open\\command",
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
			if (type == REG_SZ)
			{
				m_PlayerPath = data;
				m_PlayerPath.erase(0, 1);						// Get rid of the leading quote
				m_PlayerPath.resize(m_PlayerPath.length() - 6);	// And the " "%1" at the end
			}
		}

		delete[] data;
		RegCloseKey(hKey);
	}

}

void KwMusic::UpdateData()
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
		//L"With You-AAA-酷我音乐 "
		//L"子-酷我音乐 虹の約束-小松未可"

		wstring data = Buffer;
		data += data;
		data += data;

		wstring::size_type pos;
		pos = data.find(L"-酷我音乐 ");

		if (pos == wstring::npos)
		{
			abnormal = true;
			m_Track = Buffer;
		}
		else
		{
			pos += 6;
			data.erase(0, pos);
			pos = data.find(L"-酷我音乐 ");
			data.resize(pos);
		}

		
		if (!abnormal && LastTrack.compare(data))
		{
			pos = 0;
			int pos2 = 0, pos3 = 0;
			int len = data.length();
			for (int i = 1; i < len; i++)
			{
				if (data[i] == L'-')
				{
					if (data[i - 1] == L' ')
					{
						if (data[i + 1] == L' ')
							pos3 = i;
						else
							pos2 = i;
					}
					else
					{
						if (data[i + 1] == L' ')
							pos2 = i;
						else
						{
							pos = i;
							break;		//真正的分隔符左右两侧应当都不为空格
						}

					}
				}
			}
			pos = pos == 0 ? pos2 : pos;
			pos = pos == 0 ? pos3 : pos;

			if (pos == 0)
			{
				abnormal = true;
				m_Track = Buffer;
			}
			else
			{
				m_Track.assign(data);
				m_Title.assign(data, 0, pos);
				m_Artist.assign(data.erase(0, pos + 1));
				abnormal = false;
			}

		}
	}

	if (abnormal)
	{
		//m_Track.clear();
		m_Title.clear();
		m_Artist.clear();
	}

	m_TrackChanged = m_Track.compare(LastTrack) !=0;
}




/*
**按键控制
*/
void KwMusic::PlayPause()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_F5);
}

void KwMusic::Stop()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_F6);
}

void KwMusic::Previous()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_LEFT, 1);
}

void KwMusic::Next()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_RIGHT, 1);
}

void KwMusic::VolumeMute()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, 'S', 1);
}

void KwMusic::VolumeUp()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_UP, 1);
}

void KwMusic::VolumeDown()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, VK_DOWN, 1);
}

void KwMusic::HideToTray()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, 'H', 1);
}

void KwMusic::MiniMode()
{
	if (IsWindow(m_Window))
		SendKey(m_Window, 'M', 1);
}



/*
**窗口操作
*/
void KwMusic::OpenPlayer(wstring& path)
{
	if (!IsWindow(m_Window))
	{
		path = path.empty() ? m_PlayerPath : path;
		ShellExecute(nullptr, L"open", path.empty() ? L"KwMusic.exe" : path.c_str(), nullptr, nullptr, SW_SHOW);
	}
	else
	{
		ShowWindow(m_Window, SW_SHOWNORMAL);
		BringWindowToTop(m_Window);
	}

}

void KwMusic::ClosePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_CLOSE, 0, 0);
}

void KwMusic::RestorePlayer()
{
	if (IsWindow(m_Window))
	{
		PostMessage(m_Window, WM_SYSCOMMAND, SC_RESTORE, 0);
		BringWindowToTop(m_Window);
	}

}

void KwMusic::MinimizePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}


