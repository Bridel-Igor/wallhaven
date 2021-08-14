#include "CategoriesAndPurity.h"
#include "HMenuGenerator.h"

PurityComponent::PurityComponent(HWND hParent, int x, int y, int width, int height)
{
	pbSFW = new PushButton(hParent, "SFW", x, y, width / 3 -1, height, 0, 0, RGB(85,150,85), RGB(55,88,55));
	pbSketchy = new PushButton(hParent, "Sketchy", x + width / 3, y, width / 3, height, 0, 0, RGB(150,150,85), RGB(88,88,55));
	pbNSFW = new PushButton(hParent, "NSFW", 1+x + (2 * width / 3), y, width / 3-1, height, 0, 0, RGB(150,85,85), RGB(88,55,55));
}

PurityComponent::~PurityComponent()
{
	delete pbSFW, pbSketchy, pbNSFW;
}

void PurityComponent::setPurity(CategoriesAndPurity cap)
{
	pbSFW->check(cap & S_PURITY_SFW);
	pbSketchy->check(cap & S_PURITY_SKETCHY);
	pbNSFW->check(cap & S_PURITY_NSFW);
}

CategoriesAndPurity PurityComponent::getPurity()
{
	return S_PURITY_SFW * pbSFW->isChecked() |
		S_PURITY_SKETCHY * pbSketchy->isChecked() |
		S_PURITY_NSFW * pbNSFW->isChecked();
}

bool PurityComponent::draw(LPDRAWITEMSTRUCT &pDIS)
{
	if (pDIS->hwndItem == pbSFW->hWnd)
	{
		pbSFW->draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbSketchy->hWnd)
	{
		pbSketchy->draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbNSFW->hWnd)
	{
		pbNSFW->draw(pDIS);
		return true;
	}
	return false;
}

bool PurityComponent::click(WPARAM &wParam)
{
	if (COMMANDEVENT(pbSFW))
	{
		pbSFW->check(!pbSFW->isChecked());
		return true;
	}
	if (COMMANDEVENT(pbSketchy))
	{
		pbSketchy->check(!pbSketchy->isChecked());
		return true;
	}
	if (COMMANDEVENT(pbNSFW))
	{
		pbNSFW->check(!pbNSFW->isChecked());
		return true;
	}
	return false;
}

CategoryComponent::CategoryComponent(HWND hParent, int x, int y, int width, int height)
{
	pbGeneral = new PushButton(hParent, "General", x, y, width / 3 - 1, height);
	pbAnime = new PushButton(hParent, "Anime", x + width / 3, y, width / 3, height);
	pbPeople = new PushButton(hParent, "People", 1+x + (2 * width / 3), y, width / 3-1, height);
}

CategoryComponent::~CategoryComponent()
{
	delete pbGeneral, pbAnime, pbPeople;
}

void CategoryComponent::setCategory(CategoriesAndPurity cap)
{
	pbGeneral->check(cap & S_CATEGORY_GENERAL);
	pbAnime->check(cap & S_CATEGORY_ANIME);
	pbPeople->check(cap & S_CATEGORY_PEOPLE);
}

CategoriesAndPurity CategoryComponent::getCategory()
{
	return S_CATEGORY_GENERAL * pbGeneral->isChecked() |
		S_CATEGORY_ANIME * pbAnime->isChecked() |
		S_CATEGORY_PEOPLE * pbPeople->isChecked();
}

bool CategoryComponent::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (pDIS->hwndItem == pbGeneral->hWnd)
	{
		pbGeneral->draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbAnime->hWnd)
	{
		pbAnime->draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbPeople->hWnd)
	{
		pbPeople->draw(pDIS);
		return true;
	}
	return false;
}

bool CategoryComponent::click(WPARAM& wParam)
{
	if (COMMANDEVENT(pbGeneral))
	{
		pbGeneral->check(!pbGeneral->isChecked());
		return true;
	}
	if (COMMANDEVENT(pbAnime))
	{
		pbAnime->check(!pbAnime->isChecked());
		return true;
	}
	if (COMMANDEVENT(pbPeople))
	{
		pbPeople->check(!pbPeople->isChecked());
		return true;
	}
	return false;
}