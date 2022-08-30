#pragma once

#include "ObjectPool.h"
#include "Monster.h"

namespace Map{

	struct mapObject {
		int objectNum;
		vector<2> position;

		mapObject(int on, vector<2> pos) { objectNum = on; position = pos; };
		~mapObject() { };
	};

	struct MobSpawn
	{
		vector<2>	position;
		short		mobId;
		short		index;
		bool		isAlive;
	};

	struct Poratl
	{
		vector<2> from, to;
		int toMapId;
	};

	class MapleMap
	{
	public:
		int mapId, back;
		float timePassed;
		Sound::Sound bgm;
		vector<2> mapSize;
		std::vector<mapObject> objects;
		std::vector<vector<2>> fVertexes;
		std::vector<std::pair<int, int>> fIndicies;
		std::vector<vector<2>> rVertexes;
		std::vector<std::pair<int, int>> rIndicies;
		bool showLines;
		std::list<MONSTER::Monster*> activeMonsters;
		std::vector<MobSpawn> spawnList;
		std::vector<Poratl> portals;
		std::deque<DamageText*> damageTexts;

	public:

		void Render();
		void LateRender();

		void RenderGUI();
		void DamageTextPrint();

		Line GetFoot(int idx);
		Line GetRope(int idx);

		void MonsterSpawn();


		void SaveFile();
		void LoadFile();

		void Start();
		void Update();
		void LateUpdate();
		void End();
	};
}

