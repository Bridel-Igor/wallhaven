#include "TrayWindow.h"
#include "resource.h"
#include "MainWindow.h"
#include "Settings.h"
#include "CollectionManager.h"

#define WM_NOTIFYICONMSG (WM_USER + 2)

TrayWindow *TrayWindow::trayWindow = nullptr;
#ifdef DEMO
bool stopDemo = false;
#endif

BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, LPCSTR pszTip)
{
	NOTIFYICONDATA tnd;

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = hDlg;
	tnd.uID = uID;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICONMSG;
	tnd.hIcon = hIcon;

	if (pszTip)
	{
		if (lstrcpynA(tnd.szTip, pszTip, sizeof(tnd.szTip))==nullptr)
			return FALSE;
	}
	else
		tnd.szTip[0] = '\0';

	return Shell_NotifyIconA(dwMessage, &tnd);
}

LRESULT TrayWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NOTIFYICONMSG && (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN))
	{
		hPopup = GetSubMenu(hMenu, 0);
		if (CollectionManager::isPrevious())
			EnableMenuItem(hPopup, 3, MF_BYPOSITION | MF_ENABLED);
		else
			EnableMenuItem(hPopup, 3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		SetForegroundWindow(hWnd);
		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hPopup, 0, pt.x, pt.y, 0, hWnd, NULL);
	}

	switch (uMsg)
	{
	case WM_CREATE:
	{		
		pszIDStatusIcon = MAKEINTRESOURCE(IDI_ICON1);
		hStatusIcon = LoadIcon(GetModuleHandleA(NULL), pszIDStatusIcon);
		TrayMessage(hWnd, NIM_ADD, 1, hStatusIcon, "Wallhaven");
		hMenu = LoadMenu(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDR_MENU1));
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_WALLHAVEN_START:
			EnableMenuItem(hPopup, 0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			EnableMenuItem(hPopup, 1, MF_BYPOSITION | MF_ENABLED);
			Settings::startSlideshow();
			break;
		case ID_WALLHAVEN_PAUSE:
			EnableMenuItem(hPopup, 0, MF_BYPOSITION | MF_ENABLED);
			EnableMenuItem(hPopup, 1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			Settings::pauseSlideshow();
			break;
		case ID_WALLHAVEN_NEXTWALLPAPER:
			Settings::replayDelay();
			CollectionManager::setNextWallpaper();
			break;
		case ID_WALLHAVEN_PREVIOUSWALLPAPER:
			Settings::replayDelay();
			CollectionManager::setPreviousWallpaper();
			break;
		case ID_WALLHAVEN_SETTINGS:
		{
			std::thread thr(MainWindow::windowThread);
			thr.detach();
			break;
		}
		case ID_WALLHAVEN_EXIT:
#ifdef DEMO
			stopDemo = true;
#endif
			TrayMessage(hWnd, NIM_DELETE, 1, hStatusIcon, "Wallhaven");
			DestroyMenu(hMenu);
			DestroyWindow(hWnd);
			break;
		}
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

#ifdef DEMO
void demoLimitation()
{
	int slept=0;
	while (slept < 600000)
	{
		if (stopDemo)
			return;
		Sleep(1000);
		slept += 1000;
	}
	SendMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLHAVEN_PAUSE, 0);
	SendMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLHAVEN_EXIT, 0);
	MessageBoxA(nullptr, "In demo version you can't run application for longer than 10 minutes.", "Wallhaven - demo", MB_OK | MB_ICONEXCLAMATION);
}
#endif

void TrayWindow::windowThread()
{
	if (trayWindow)
		return;
	trayWindow = new TrayWindow;
	trayWindow->Create("Wallhaven", NULL, NULL, 0, 0, 0, 0, NULL, NULL);
	ShowWindow(trayWindow->Window(), SW_HIDE);
#ifdef DEMO
	std::thread demoThread(demoLimitation);
#endif
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#ifdef DEMO
	demoThread.join();
#endif
	trayWindow->Destroy();
	delete trayWindow;
	Settings::exiting = true;
	Settings::abortDelay();
	trayWindow = nullptr;
}