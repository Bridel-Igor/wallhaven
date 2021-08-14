#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"

class PushButton
{
	bool checked = false;
	COLORREF checked1=RGB(130, 130, 130), checked2=RGB(80, 80, 80);

public:
	PushButton(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0, COLORREF color1= RGB(130, 130, 130), COLORREF color2= RGB(80, 80, 80))
	{
		hMenu = HMenuGenerator::getNewHMenu();
		hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, hMenu, NULL, NULL);
		checked1 = color1;
		checked2 = color2;
	}
	~PushButton()
	{
		DestroyWindow(hWnd);
		HMenuGenerator::releaseHMenu(hMenu);
	}
	void check(bool state)
	{
		if (checked != state)
		{
			checked = state;
			InvalidateRect(hWnd, nullptr, true);
		}
	}
	bool isChecked()
	{
		return checked;
	}
	void draw(LPDRAWITEMSTRUCT pDIS)
	{
		RECT temp;
		HBRUSH color;
		int r1 = 70, g1 = 70, b1 = 70;
		int r2 = 40, g2 = 40, b2 = 40;
		if (checked)
		{
			r1 = GetRValue(checked1), g1 = GetGValue(checked1), b1 = GetBValue(checked1);
			r2 = GetRValue(checked2), g2 = GetGValue(checked2), b2 = GetBValue(checked2);
		}
		for (int i = 0; i < pDIS->rcItem.bottom; i++)
		{
			int r, g, b;
			r = r1 + (i * (r2 - r1) / pDIS->rcItem.bottom);
			g = g1 + (i * (g2 - g1) / pDIS->rcItem.bottom);
			b = b1 + (i * (b2 - b1) / pDIS->rcItem.bottom);

			temp.left = 0;
			temp.top = i;
			temp.right = pDIS->rcItem.right;
			temp.bottom = i + 1;

			color = CreateSolidBrush(RGB(r, g, b));
			FillRect(pDIS->hDC, &temp, color);
		}

		SetTextColor(pDIS->hDC, checked? RGB(255, 255, 255) : RGB(200, 200, 200));
		SetBkMode(pDIS->hDC, TRANSPARENT);
		SetTextAlign(pDIS->hDC, TA_CENTER);
		char staticText[32];
		int len = SendMessage(pDIS->hwndItem, WM_GETTEXT, ARRAYSIZE(staticText), (LPARAM)staticText);
		TextOutA(pDIS->hDC, (pDIS->rcItem.right - pDIS->rcItem.left) / 2, pDIS->rcItem.left + 3, staticText, len);
	}

	HWND hWnd = NULL;
	HMENU hMenu;
};