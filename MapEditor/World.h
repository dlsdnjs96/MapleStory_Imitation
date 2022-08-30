#pragma once
#include "Map.h"

namespace World
{
	namespace {
		typedef std::shared_ptr<Map::MapleMap> MapSptr;
		std::map<int, MapSptr> mapList;
	}

	void Update();
	void LoadMapData();
	void RenderHierarchy();

};

