#pragma once
#include "Map.h"
#include "DamageSystem.h"

namespace WORLD
{
	class World {
	private:
		World() {}
		World(const World& ref) {}
		World& operator=(const World& ref) {}
		~World() {}
		std::unordered_map<int, Map::MapleMap*> mapList;
		Map::MapleMap* currentMap;

	public:
		ObjectPool<MONSTER::Monster> monsterPool;
		ObjectPool<DamageText> damagePool;

	public:
		static World& getIncetance() {
			static World w;
			return w;
		}
		void Start();
		void Update();
		void LateUpdate();
		void End();

		void RenderGUI();

		void LoadMapData();
		Map::MapleMap* GetCurrentMap();
		Map::MapleMap* GetMap(int idx);
		void Warp(int idx);
	};
	

};

