#include <windows.h>

/*
VC��֤Ӧ�ó���ֻ��һ��ʵ��������
ͨ��#pragma data_segԤ����ָ���һ���½ڣ��ڴ˽��п���volatile�ؼ��ֶ���һ�����������ұ��������г�ʼ����Volatile�ؼ���ָ���˱�������Ϊ�ⲿ���̷��ʡ����Ϊ��ʹ�ñ����ܹ��ڽ��̻�������з������ã���Ҫ��������Ϊ���������ͬʱ������ж���д����Ȩ�ޡ������ͨ��#pragma commentԤ����ָ����֪ͨ��������
*/
#pragma data_seg("Shared") 
int volatile g_lAppInstance = 0;
#pragma data_seg() 
#pragma comment(linker,"/section:Shared,RWS")

static HHOOK hKeyboardHook;
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam,LPARAM lParam)
{
	PKBDLLHOOKSTRUCT hs = (PKBDLLHOOKSTRUCT)lParam;
	if (code == HC_ACTION) {
		if (hs->vkCode == VK_SPACE && GetKeyState(VK_CONTROL) < 0) {
			switch (wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				keybd_event(VK_SHIFT, 0, 0, 0);
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
				return 1;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				return 1;
			}
		}
	}
	return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	if (0 == g_lAppInstance)
	{
		g_lAppInstance = 1;
	}
	else if (1 == g_lAppInstance)
	{
		::MessageBox(NULL, TEXT("�˳���֮ǰ�Ѿ��������ˡ�"), TEXT("ע��"), MB_OK);
		return FALSE;
	}
	MSG msg;
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}