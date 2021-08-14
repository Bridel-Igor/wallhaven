#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class SearchCollection : public BaseCollection
{
private:
	struct SearchCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity;
		char tag[255];
		char resolution[255];
	};

public:
	SearchCollection();
	~SearchCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool loadWallpaper(unsigned int index);
	LPCSTR collectionType() const { return "Search collection"; }
	LPCSTR collectionName() const;
	void openCollectionSettingsWindow();

	SearchCollectionSettings* settings;

private:
	char searchUrl[1024] = "";
	int per_page = 24;
};