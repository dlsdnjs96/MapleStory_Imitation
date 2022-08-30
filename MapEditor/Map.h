#pragma once


namespace Map{

	struct mapObject {
		int objectNum;
		vector<2> position;

		mapObject(int on, vector<2> pos) { objectNum = on; position = pos; };
		~mapObject() { };
	};

	class MapleMap
	{
	public:
		int mapId;
		vector<2> mapSize;
		std::vector<mapObject> objects;
		vector<2> vertexes;
		std::pair<int, int> indicies;

	public:
		MapleMap() { mapId = 1; };

		void Update();
		void SaveFile();
		void LoadFile();
		void RenderHierarchy();
	};
}

