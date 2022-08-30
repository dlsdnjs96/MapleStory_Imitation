#pragma once


class MonsterData
{
private:
	MonsterData() {}
	MonsterData(const MonsterData& ref) {}
	MonsterData& operator=(const MonsterData& ref) {}
	~MonsterData() {}

	std::map<short, MonsterStat*> data;

public:
	static MonsterData& getIncetance() {
		static MonsterData md;
		return md;
	}

	void LoadAllData();
	MonsterStat* LoadData(short mobId);
	MonsterStat* GetData(short mobId);
};

