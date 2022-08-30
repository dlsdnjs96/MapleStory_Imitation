#include "stdafx.h"

#include "Map.h"
#include "World.h"
#include "Player.h"
#include "Util.h"

void WORLD::World::Start()
{
	LoadMapData();
	//currentMap = mapList.find(1)->second;

	for (auto iter = mapList.begin();iter != mapList.end();iter++)
		iter->second->Start();
	currentMap->bgm.Play();
}

void WORLD::World::Update()
{
	//ImGui::Text("FPS: ");

	//ImGui::NewFrame();
	currentMap->Update();
}

void WORLD::World::LateUpdate()
{
	currentMap->LateUpdate();
}

void WORLD::World::End()
{
	currentMap->SaveFile();
}

void WORLD::World::RenderGUI()
{
	currentMap->RenderGUI();
}

void WORLD::World::LoadMapData()
{

	if (std::filesystem::exists(MAP_PATH))
	{
		for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(MAP_PATH))
		{
			std::string file = path.string();
			if (std::filesystem::is_regular_file(path) && file.substr(file.length() - 3, file.length()).compare("xml") == 0)
			{
				std::replace(file.begin(), file.end(), '\\', '/');
				file = file.substr(file.find_last_of('/') + 1, file.length() - file.find_last_of('/') - 5);

				Map::MapleMap* tempMap = new Map::MapleMap();
				tempMap->mapId = atoi(file.c_str());
				tempMap->LoadFile();
				mapList[tempMap->mapId] = tempMap;
				//mapList.insert({ tempMap->mapId, tempMap });
			}
		}
	}
	



	//mapList.insert({ 1, new Map::MapleMap() });

	//mapList[1]->mapId = 1;
	//mapList[1]->objects.push_back(Map::mapObject(1, {0.0f, 0.0f}));
	//mapList[1]->vertexes.push_back({ -400.f, -100.f });
	//mapList[1]->vertexes.push_back({ -200.f, 0.f });
	//mapList[1]->vertexes.push_back({ 200.f, 0.f });
	//mapList[1]->vertexes.push_back({ 400.f, -100.f });
	//mapList[1]->indicies.push_back({ 0, 1 });
	//mapList[1]->indicies.push_back({ 1, 2 });
	//mapList[1]->indicies.push_back({ 2, 3 });

	//mapList[1]->vertexes.push_back({ -200.f, -100.f });
	//mapList[1]->vertexes.push_back({ 200.f, 100.f });
	//mapList[1]->indicies.push_back({ 0, 1 });
	currentMap = mapList[1];

	//mapList[1]->SaveFile();
	//mapList[1]->Update();
}
Map::MapleMap* WORLD::World::GetCurrentMap()
{
	return currentMap;
}

Map::MapleMap* WORLD::World::GetMap(int idx)
{
	return mapList[idx];
}

void WORLD::World::Warp(int idx)
{

	PLAYER.position = CURRENT_MAP->portals[idx].to;
	CURRENT_MAP->bgm.Stop();
	currentMap = mapList[CURRENT_MAP->portals[idx].toMapId];

	CURRENT_MAP->bgm.loop = true;
	CURRENT_MAP->bgm.Play();
	PLAYER.ChangeState(PlayerState::FALL);
	Util::PlayerSound("Portal");
	MAINCAM->Location = { PLAYER.GetPosition()[0], PLAYER.GetPosition()[1] };

}
