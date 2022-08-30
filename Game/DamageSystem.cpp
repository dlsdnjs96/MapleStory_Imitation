#include "stdafx.h"
#include "DamageSystem.h"
#include "Player.h"
#include "World.h"
#include "Map.h"
#include "MonsterData.h"
#include "Util.h"


namespace DamageSystem {

	void NormalAttack(Collision::RectAngle attackRange)
	{
		printf("NormalAttack\r\n");
		MONSTER::Monster* target = nullptr;

		for (auto iter = CURRENT_MAP->activeMonsters.begin(); iter != CURRENT_MAP->activeMonsters.end(); iter++)
		{
			if (Collision::Collide({ MOBDATA.GetData((*iter)->mobId)->hitbox.Length, MOBDATA.GetData((*iter)->mobId)->hitbox.Angle, (*iter)->position + MOBDATA.GetData((*iter)->mobId)->hitbox.Location }, attackRange))
			{
				printf("found target\r\n");
				target = *iter;
				break;
			}
		}
		if (target != nullptr) {
			int damage = Util::Rand(50, 80);
			target->HitByPlayer(damage);
			AddDamageText(target->position + vector<2>{0.f, MOBDATA.GetData(target->mobId)->hitbox.Length[1]}, damage, true);
		}
		return;
	}

	void AddDamageText(vector<2> from, short damage, bool type)
	{
		DamageText* temp = WORLDI.damagePool.GetObject();
		temp->addedTime = ELAPSED;
		temp->position = from;
		temp->monster = type;
		for (size_t i = 0; i <= std::to_string(damage).size(); i++)
			temp->text[i] = std::to_string(damage).c_str()[i];
		CURRENT_MAP->damageTexts.push_back(temp);
	}

	void misile()
	{
		BezierCurve3
	}
}
