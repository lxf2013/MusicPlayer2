#include "StdAfx.h"
#include "KgMusic.h"




/*
**加载与更新
*/
Player* KgMusic::c_Player = nullptr;

Player* KgMusic::Create()
{
	if (!c_Player)
	{
		c_Player = new KgMusic();
		c_Player->Initialize();
	}
	c_Player->AddInstantce();

	return c_Player;
}

KgMusic::~KgMusic()
{
	c_Player = nullptr;
}

KgMusic::KgMusic()
{
	// Get temporary file for cover art
	WCHAR buffer[MAX_PATH];
	GetTempPath(MAX_PATH, buffer);
	GetTempFileName(buffer, L"jpg", 0, buffer);
	m_TempCoverPath = buffer;
}

void KgMusic::GetWindow()
{
	//获取窗口句柄

	HWND hWnd = NULL;
	LONG lWnd;

	do
	{
		hWnd = FindWindowEx(NULL, hWnd, L"kugou_ui", NULL);
		lWnd = GetWindowLong(hWnd, GWL_STYLE);
		if ((lWnd & WS_VISIBLE) &&
			(lWnd & WS_SYSMENU) &&
			GetWindowTextLength(hWnd))
		{
			m_Window = hWnd;
			break;
		}
			
	} while (hWnd);


}

void KgMusic::Initialize()
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
		RegOpenKeyEx(HKEY_CURRENT_USER,
			L"Software\\KuGou",
			0,
			KEY_QUERY_VALUE,
			&hKey);

		DWORD size = 512;
		WCHAR* data = new WCHAR[size];
		DWORD type = 0;

		if (RegQueryValueEx(hKey,
			L"AppPath",
			nullptr,
			(LPDWORD)&type,
			(LPBYTE)data,
			(LPDWORD)&size) == ERROR_SUCCESS)
		{
			if (type == REG_SZ)
			{
				m_PlayerPath = data;
				m_PlayerPath += L"\\KGMusic.exe";
			}
		}

		delete[] data;
		RegCloseKey(hKey);
	}

	//初始化临时文件地址
	WCHAR buffer[MAX_PATH];
	GetTempPath(MAX_PATH, buffer);
	GetTempFileName(buffer, L"jpg", 0, buffer);
	m_TempCoverPath = buffer;

}

void KgMusic::UpdateData()
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
		//L"%FileName - 酷狗音乐 "(滚动)

		wstring data = Buffer;
		data += data;
		data += data;

		wstring::size_type pos;
		pos = data.find(L" - 酷狗音乐 ");

		if (pos == wstring::npos)
		{
			abnormal = true;
			m_Track = Buffer;
		}
		else
		{
			pos += 8;
			data.erase(0, pos);
			pos = data.find(L" - 酷狗音乐 ");
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
						{
							pos = i;
							break;
						}	
						else
							pos2 = i;
					}
					else
					{
						if (data[i + 1] == L' ')
							pos2 = i;
						else
						{
							pos3 = i;
						}
					}
				}
			}
			pos = pos == 0 ? pos2 : pos;
			pos = pos == 0 ? pos3 : pos;

			abnormal = false;
			m_Track = data;
			
			if (pos == 0)
			{
				m_Title = data;
			}
			else
			{
				m_Artist.assign(data, 0, pos);
				m_Title.assign(data.erase(0, pos + 1));
				
				if (m_Artist[--pos] == L' ')
					m_Artist.erase(pos);
				if (m_Title[0] == L' ')
					m_Title.erase(0, 1);
			}

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
void KgMusic::VolumeMute()
{
	SendKey('S', 1, 0, 1);
}
void KgMusic::VolumeUp()
{
	SendKey(VK_UP, 0, 0, 1);
}
void KgMusic::VolumeDown()
{
	SendKey(VK_DOWN, 0, 0, 1);
}
void KgMusic::ExecuteBang(LPCWSTR args)
{
	if (!_wcsicmp(args, L"DesktopLyricShow"))
	{
		SendKey('D', 1, 0, 1);
	}
	else if (!_wcsicmp(args, L"DesktopLyricLock"))
	{
		SendKey('E', 1, 0, 1);
	}
	else if (!_wcsicmp(args, L"Reverse"))
	{
		SendKey(VK_LEFT, 1, 1, 0);
	}
	else if (!_wcsicmp(args, L"Forward"))
	{
		SendKey(VK_RIGHT, 1, 1, 0);
	}
	else
	{
		RmLog(LOG_WARNING, L"MusicPlayer.dll: Unknown Command");
	}
}


/*
**窗口操作
*/
void KgMusic::OpenPlayer(wstring& path)
{
	if (!IsWindow(m_Window))
	{
		path = path.empty() ? m_PlayerPath : path;
		ShellExecute(nullptr, L"open", path.empty() ? L"KuGou.exe" : path.c_str(), nullptr, nullptr, SW_SHOW);
	}
	else
	{
		//ShowWindow(m_Window, SW_SHOWNORMAL);
		PostMessage(m_Window, WM_SYSCOMMAND, SC_RESTORE, 0);
		BringWindowToTop(m_Window);
	}

}

void KgMusic::ClosePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_CLOSE, 0, 0);
}

void KgMusic::RestorePlayer()
{
	if (IsWindow(m_Window))
	{
		PostMessage(m_Window, WM_SYSCOMMAND, SC_RESTORE, 0);
		BringWindowToTop(m_Window);
	}

}

void KgMusic::MinimizePlayer()
{
	if (IsWindow(m_Window))
		PostMessage(m_Window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}



/*
**获取封面
*/

void KgMusic::UpdateCover()
{
	if (!m_RequireCover)
		return;
	
	if (SearchCover(m_Artist, m_CoverPath) ||
		ExtractCover(m_Artist, m_TempCoverPath, m_CoverPath)
		)
		return;
	
		m_CoverPath.clear();
	
}

bool KgMusic::SearchCover(const wstring& artist, wstring& data)
{
	
	wstring::size_type pos = artist.find_first_of(L'、');
	if (pos == wstring::npos)
		pos = artist.length();

	
	WCHAR buffer[MAX_PATH];
	GetEnvironmentVariable(L"APPDATA", buffer, sizeof(buffer) / sizeof(WCHAR));

	data = buffer;
	data += L"\\KuGou8\\SingerRes\\";
	data.append(artist,0, pos);
	data += L"\\120\\*.jpg";

	WIN32_FIND_DATA result;
	HANDLE hFind;

	hFind = FindFirstFile(data.c_str(), &result);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	
	FindClose(hFind);

	data.resize(data.length() - 5);
	data += result.cFileName;

	return true;
}


bool KgMusic::ExtractCover(const wstring& artist, const wstring& tempfile, wstring& data)
{
	//IZ, KGMusic.dll

	WCHAR buffer[MAX_PATH];
	GetEnvironmentVariable(L"APPDATA", buffer, sizeof(buffer) / sizeof(WCHAR));

	data = buffer;
	data += L"\\KuGou8\\SingerImg\\SingerRes.xml";

	FILE * fp;

	if (_wfopen_s(&fp, data.c_str(), L"rt+,ccs=UNICODE"))
	{
		RmLog(LOG_WARNING, L"MusicPlayer.dll: 读取酷狗配置文件失败");
		return false;
	}
	WCHAR sXMLBuff[1024] = { NULL };
	WCHAR sFindStr[MAX_PATH] = { NULL };
	WCHAR *sFind = NULL;

	BYTE bCoverSize =2; // 2, 1
	bCoverSize = bCoverSize > 2 ? 2 : bCoverSize;
	wcscpy_s(sFindStr, artist.c_str());

	if ((sFind = wcschr(sFindStr, L' ')) != NULL)
	{
		wcscpy_s(sXMLBuff, sFind + 1);
		*sFind = NULL;
		wcscat_s(sFindStr, L"&#32;");
		wcscat_s(sFindStr, sXMLBuff);
	}

	if ((sFind = wcschr(sFindStr, L'、')) != NULL)
	{
		*sFind = NULL;
	}

	while (fgetws(sXMLBuff, 1023, fp))
	{
		if ((sFind = wcsstr(sXMLBuff, sFindStr)) != NULL)
		{
			sFind = wcsstr(sFind, L"ResourceHash");

			if (sFind == NULL) break;

			if (swscanf_s(sFind, L"%*[^\"]\"%[^\"]", sFindStr, 33))
			{
				fclose(fp);
				data = buffer;
				data += L"\\KuGou8\\SingerImg\\";
				data += sFindStr;
				data += L".krs";

				errno_t err;

				if (err = _wfopen_s(&fp, data.c_str(), L"rb"))
				{
					data += L".kg!";
					err = _wfopen_s(&fp, data.c_str(), L"rb");
				}

				if (!err)
				{
					size_t nFileSize = 6000;
					BYTE sFileBuff[6000] = { 0 };
					nFileSize = fread(sFileBuff, 1, nFileSize, fp);
					fclose(fp);
					//return;
					BYTE* cFileBuff = sFileBuff;
					BYTE* cFileStart0 = sFileBuff + nFileSize - 1;
					BYTE* cFileStart = sFileBuff;
					BYTE* cFileEnd = sFileBuff;
					BYTE n = 0;
					size_t nc = 0;
					while (nc<nFileSize)
					{
						if (*(cFileBuff) == 0xFF && *(cFileBuff + 1) == 0xD8)
						{
							cFileStart0 = cFileBuff;
						}
						if (*(cFileBuff) == 0xFF && *(cFileBuff + 1) == 0xD9 && cFileBuff > cFileStart0)
						{
							cFileStart = cFileStart0;
							cFileEnd = cFileBuff + 4;
							n++;
							if (bCoverSize == n)break;
						}
						cFileBuff++;
						nc++;
					}
					if (n > 0)
					{
						if (!_wfopen_s(&fp, tempfile.c_str(), L"wb"))
						{
							fwrite(cFileStart, 1, cFileEnd - cFileStart, fp);
							fclose(fp);
							data = tempfile;
							return true;
						}
					}
				}
				return false;
			}
			else break;
		}
	}
	fclose(fp);
	return false;
}
