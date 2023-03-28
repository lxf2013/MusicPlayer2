#include "common.h"

MemoryCount MemoryCount::memory_count;

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

void SendKey(HWND hWnd, UINT key)
{
	UINT VSC = MapVirtualKey(key, 0);
	PostMessage(hWnd, WM_KEYDOWN, key, 0x00000001 | VSC << 16);
	PostMessage(hWnd, WM_KEYUP, key, 0xC0000001 | VSC << 16);
}

void SendKey(HWND hWnd, UINT key, bool ctrl)
{
	UINT VSC_CTRL = MapVirtualKey(VK_CONTROL, 0) << 16;
	UINT VSC = MapVirtualKey(key, 0) << 16;

	if (key >= 'A' && key <= 'Z')
	{

		PostMessage(hWnd, WM_KEYDOWN, VK_CONTROL, 0x00000001 | VSC_CTRL);
		PostMessage(hWnd, WM_KEYDOWN, key, 0x00000001 | VSC);
		//PostMessage(hWnd, WM_CHAR,		key - 0x40,	0x00000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, key, 0xC0000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, VK_CONTROL, 0xC0000001 | VSC_CTRL);
	}
	else
	{
		PostMessage(hWnd, WM_KEYDOWN, VK_CONTROL, 0x00000001 | VSC_CTRL);
		PostMessage(hWnd, WM_KEYDOWN, key, 0x01000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, key, 0xC1000001 | VSC);
		PostMessage(hWnd, WM_KEYUP, VK_CONTROL, 0xC0000001 | VSC_CTRL);
	}
}

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