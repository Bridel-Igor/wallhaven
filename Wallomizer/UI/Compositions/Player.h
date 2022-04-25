#pragma once

#include "Static.h"
#include "IconButton.h"
#include "CollectionManager.h"

class Player
{
public:
	Player(HWND hParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, CollectionManager* collectionManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	static void updateTimer(bool forsed = false);
	static void redrawPlayers();
	void mouseHovering(WPARAM wParam);

private:
	void updateText();

	Static* stDelayRemained;
	IconButton* btnPlay, * btnPause, * btnNext, * btnPrev, * btnOpenExternal;
	static char timer[16];
	static CollectionManager* s_pCollectionManager;
};