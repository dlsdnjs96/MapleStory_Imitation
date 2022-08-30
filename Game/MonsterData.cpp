#include "stdafx.h"
#include "MonsterData.h"
#include "Xml.h"

void MonsterData::LoadAllData()
{
	if (std::filesystem::exists(MOB_PATH))
	{
		for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(MOB_PATH))
		{
			std::string file = path.string();
			if (std::filesystem::is_regular_file(path) && file.substr(file.length() - 3, file.length()).compare("xml") == 0)
			{
				std::replace(file.begin(), file.end(), '\\', '/');
				file = file.substr(file.find_last_of('/') + 1, file.length() - file.find_last_of('/') - 5);

				data[atoi(file.c_str())] = LoadData(atoi(file.c_str()));
			}
		}
	}
}

MonsterStat* MonsterData::LoadData(short mobId)
{
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	std::string path = "./Contents/Mob/" + std::to_string(mobId) + ".xml";
	Xml::XMLError result = doc->LoadFile(path.c_str());
	if (result != Xml::XML_SUCCESS) return nullptr;
	MonsterStat* tempData = new MonsterStat();


	Xml::XMLElement* ob;
	ob = doc->FirstChildElement();
	tempData->chaseRange		= ob->FloatAttribute("chaseRange");
	tempData->attackRange		= ob->FloatAttribute("attackRange");
	tempData->movingSpeed		= ob->FloatAttribute("movingSpeed");
	tempData->maxHp				= ob->IntAttribute("maxHp");
	tempData->physicalAttack	= ob->IntAttribute("physicalAttack");
	tempData->physicalAmour		= ob->IntAttribute("physicalAmour");
	tempData->movingType		= ob->IntAttribute("movingType");

	Xml::XMLElement* ImageSize = ob->FirstChildElement("StateImageSize");
	for (size_t i = 0; i < 10; i++)
	{
		std::string Tag = "ImageSize" + std::to_string(i);
		tempData->stateImageSize[i] = ImageSize->IntAttribute(Tag.c_str());
	}

	{
		Xml::XMLElement* HitBox = ob->FirstChildElement("HitBox");

		tempData->hitbox.Length[0]   = HitBox->FloatAttribute("LenX");
		tempData->hitbox.Length[1]   = HitBox->FloatAttribute("LenY");
		tempData->hitbox.Angle		 = HitBox->FloatAttribute("Angle");
		tempData->hitbox.Location[0] = HitBox->FloatAttribute("LocX");
		tempData->hitbox.Location[1] = HitBox->FloatAttribute("LocY");
	}

	return tempData;
}

MonsterStat* MonsterData::GetData(short mobId)
{
	return data[mobId];
}
