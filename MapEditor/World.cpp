#include "stdafx.h"

#include "Map.h"
#include "World.h"

void World::Update()
{
	mapList[1]->Update();
}

void World::LoadMapData()
{

	mapList[1] = std::make_shared<Map::MapleMap>();
	mapList[1]->objects.push_back(Map::mapObject(1, {0.0f, 0.0f}));
	mapList[1]->Update();
}

void World::RenderHierarchy()
{
	//ImGui::PushID();
	for (auto iter = mapList.begin(); iter != mapList.end(); iter++)
	{
		if (ImGui::TreeNode(std::to_string(iter->first).c_str()))
		{
			iter->second->RenderHierarchy();
			ImGui::TreePop();
		}
	}
	//ImGui::PopID();
}