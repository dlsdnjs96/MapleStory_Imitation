#include "stdafx.h"

#include "Xml.h"
#include "Camera.h"
#include "Map.h"
#include "World.h"
#include "DamageSystem.h"



Line Map::MapleMap::GetFoot(int idx)
{
	return { fVertexes[fIndicies[idx].first], fVertexes[fIndicies[idx].second] };
}

Line Map::MapleMap::GetRope(int idx)
{
	return { rVertexes[rIndicies[idx].first], rVertexes[rIndicies[idx].second] };
}

void Map::MapleMap::MonsterSpawn()
{
	auto iter = activeMonsters.begin();
	while (iter != activeMonsters.end())
	{
		(*iter)->Update();
		if (!(*iter)->isAlive) {
			spawnList.at((*iter)->spawnIndex).isAlive = false;
			WORLDI.monsterPool.ReturnObject(*iter);
			activeMonsters.erase(iter++);
		}
		else iter++;
	}

	if (timePassed < 3.f)
		return;
	timePassed = 0.f;

	size_t len = spawnList.size();
	for (size_t i = 0; i < len; i++)
	{
		if (!spawnList.at(i).isAlive)
		{
			auto newMob = WORLDI.monsterPool.GetObject();
			newMob->Start(spawnList.at(i).mobId);
			newMob->spawnIndex = spawnList.at(i).index;
			newMob->position = spawnList.at(i).position;
			spawnList.at(i).isAlive = true;
			activeMonsters.push_back(newMob);
		}
	}

	printf("activeMonsters %d\r\n", activeMonsters.size());
}

void Map::MapleMap::Start()
{
	WORLDI.monsterPool.Initialize(spawnList.size());
	timePassed = 0.f;
}

void Map::MapleMap::Update()
{
	timePassed += DELTA;
	Render();
	MonsterSpawn();
	DamageTextPrint();
}

void Map::MapleMap::LateUpdate()
{
	LateRender();
}

void Map::MapleMap::End()
{
}

void Map::MapleMap::Render()
{
	Rendering::Image::Component image;


	image.Content = "Map/Back/" + std::to_string(back);
	image.Location = { 0.f, 0.f };
	image.Length = image.GetLength();
	image.Draw(true);

	std::string path = "Map/Object/";

	size_t lenVector = objects.size();
	for (size_t i = 0; i < lenVector; i++) {
		image.Content = "Map/Object/" + std::to_string(objects[i].objectNum);
		image.Location = objects[i].position;
		image.Length = image.GetLength();
		image.Draw(true);
	}

	if (showLines || true)
	{
		lenVector = fIndicies.size();
		for (size_t i = 0; i < lenVector; i++) {
			image.Content = "MapFoot1";
			image.Location = (fVertexes[fIndicies[i].first] + fVertexes[fIndicies[i].second]) / 2.f;
			image.Length = { length(fVertexes[fIndicies[i].first] - fVertexes[fIndicies[i].second]), 5.f };

			vector<2> temp = fVertexes[fIndicies[i].second] - fVertexes[fIndicies[i].first];
			image.Angle = atan2f(temp[1], temp[0]) * 180.f / 3.141592f;

			image.Draw(true);
		}
		image.Angle = 0.f;

		size_t lenVector = fVertexes.size();
		for (size_t i = 0; i < lenVector; i++) {
			image.Content = "MapFoot0";
			image.Location = fVertexes[i];
			image.Length = { 10.f, 10.f };
			image.Draw(true);
		}


		lenVector = rIndicies.size();
		for (size_t i = 0; i < lenVector; i++) {
			image.Content = "MapFoot3";
			image.Location = (rVertexes[rIndicies[i].first] + rVertexes[rIndicies[i].second]) / 2.f;
			image.Length = { length(rVertexes[rIndicies[i].first] - rVertexes[rIndicies[i].second]), 5.f };

			vector<2> temp = rVertexes[rIndicies[i].second] - rVertexes[rIndicies[i].first];
			image.Angle = atan2f(temp[1], temp[0]) * 180.f / 3.141592f;

			image.Draw(true);
		}
		image.Angle = 0.f;

		lenVector = rVertexes.size();
		for (size_t i = 0; i < lenVector; i++) {
			image.Content = "MapFoot2";
			image.Location = rVertexes[i];
			image.Length = { 10.f, 10.f };
			image.Draw(true);
		}
	}


	lenVector = portals.size();
	for (size_t i = 0; i < lenVector; i++) {
		image.Content = "Map/portal";
		image.Length = image.GetLength();
		image.Location = { portals[i].from[0], portals[i].from[1] + (image.Length[1] / 2.f)};
		image.Draw(true);
	}

}

void Map::MapleMap::LateRender()
{
	Rendering::Image::Component image;
	float textP = -40.f;

	image.Length = { 0.f, 0.f };
	for (auto& iter : damageTexts)
	{
		for (size_t i = 0; iter->text[i] != '\0'; i++) {
			if (iter->monster) {
				image.Content = "UI/Damage/base/" + std::to_string(iter->text[i] - '0');
				textP += image.Length[0] / 2.f;
				image.Length = image.GetLength();
				textP += image.Length[0] / 2.f;
				image.Location = { iter->position[0] + textP, iter->position[1] };
				image.Draw(false);
			}
			else {
				image.Content = "UI/Damage/violet/" + std::to_string(iter->text[i] - '0');
				textP += image.Length[0] / 2.f;
				image.Length = image.GetLength();
				textP += image.Length[0] / 2.f;
				image.Location = { iter->position[0] + textP, iter->position[1] };
				image.Draw(false);
			}
		}
	}
}

void Map::MapleMap::RenderGUI()
{
	ImGui::Begin("Map");
	if (ImGui::BeginTabBar("Verticies")) {
		if (ImGui::BeginTabItem("fVertexes"))
		{
			for (int i = 0;i < fVertexes.size();i++)
			if (ImGui::TreeNode(std::to_string(i).c_str()))
			{
			
					ImGui::DragFloat("X", &fVertexes[i][0], 0.5);
					ImGui::DragFloat("Y", &fVertexes[i][1], 0.5);
				
				ImGui::TreePop();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Indicies"))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void Map::MapleMap::DamageTextPrint()
{
	while (!damageTexts.empty())
	{
		if (damageTexts.front()->addedTime + 1.f < ELAPSED)
			damageTexts.pop_front();
		else
			break;
	}
	for (auto& iter : damageTexts)
		iter->position[1] += DELTA * 30.f;
	

}


void Map::MapleMap::SaveFile()
{
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	Xml::XMLElement* ob = doc->NewElement("Root");
	doc->LinkEndChild(ob);


	ob->SetAttribute("MapId", mapId);
	ob->SetAttribute("Back", back);
	ob->SetAttribute("MapSizeX", mapSize[0]);
	ob->SetAttribute("MapSizeY", mapSize[1]);
	ob->SetAttribute("BGM", bgm.Content.c_str());

	Xml::XMLElement* Objects = doc->NewElement("Objects");
	ob->LinkEndChild(Objects);
	Objects->SetAttribute("Size", (int)objects.size());

	for (size_t i = 0;i < objects.size();i++)
	{
		std::string temp = "Object" + std::to_string(i);
		Xml::XMLElement* Object = doc->NewElement(temp.c_str());
		Objects->LinkEndChild(Object);
		Object->SetAttribute("objNum", objects[i].objectNum);
		Object->SetAttribute("posX", objects[i].position[0]);
		Object->SetAttribute("posY", objects[i].position[1]);
	}

	{
		Xml::XMLElement* Foot = doc->NewElement("Foot");
		ob->LinkEndChild(Foot);
		Xml::XMLElement* Vertexes = doc->NewElement("Vertexes");
		Foot->LinkEndChild(Vertexes);
		Vertexes->SetAttribute("Size", (int)fVertexes.size());

		for (size_t i = 0; i < fVertexes.size(); i++)
		{
			std::string temp = "Vertex" + std::to_string(i);
			Xml::XMLElement* Vertex = doc->NewElement(temp.c_str());
			Vertexes->LinkEndChild(Vertex);
			Vertex->SetAttribute("posX", fVertexes[i][0]);
			Vertex->SetAttribute("posY", fVertexes[i][1]);
		}

		Xml::XMLElement* Indicies = doc->NewElement("Indicies");
		Foot->LinkEndChild(Indicies);
		Indicies->SetAttribute("Size", (int)fIndicies.size());

		for (size_t i = 0; i < fIndicies.size(); i++)
		{
			std::string temp = "Index" + std::to_string(i);
			Xml::XMLElement* Index = doc->NewElement(temp.c_str());
			Indicies->LinkEndChild(Index);
			Index->SetAttribute("pairA", fIndicies[i].first);
			Index->SetAttribute("pairB", fIndicies[i].second);
		}
	}

	{
		Xml::XMLElement* Rope = doc->NewElement("Rope");
		ob->LinkEndChild(Rope);
		Xml::XMLElement* Vertexes = doc->NewElement("Vertexes");
		Rope->LinkEndChild(Vertexes);
		Vertexes->SetAttribute("Size", (int)rVertexes.size());

		for (size_t i = 0; i < rVertexes.size(); i++)
		{
			std::string temp = "Vertex" + std::to_string(i);
			Xml::XMLElement* Vertex = doc->NewElement(temp.c_str());
			Vertexes->LinkEndChild(Vertex);
			Vertex->SetAttribute("posX", rVertexes[i][0]);
			Vertex->SetAttribute("posY", rVertexes[i][1]);
		}

		Xml::XMLElement* Indicies = doc->NewElement("Indicies");
		Rope->LinkEndChild(Indicies);
		Indicies->SetAttribute("Size", (int)rIndicies.size());

		for (size_t i = 0; i < rIndicies.size(); i++)
		{
			std::string temp = "Index" + std::to_string(i);
			Xml::XMLElement* Index = doc->NewElement(temp.c_str());
			Indicies->LinkEndChild(Index);
			Index->SetAttribute("pairA", rIndicies[i].first);
			Index->SetAttribute("pairB", rIndicies[i].second);
		}
	}


	{
		Xml::XMLElement* Monster = doc->NewElement("Monster");
		ob->LinkEndChild(Monster);
		Monster->SetAttribute("Size", (int)spawnList.size());


		for (size_t i = 0; i < spawnList.size(); i++)
		{
			std::string temp = "Mob" + std::to_string(i);
			Xml::XMLElement* Mob = doc->NewElement(temp.c_str());
			Monster->LinkEndChild(Mob);
			Mob->SetAttribute("mobId", spawnList[i].mobId);
			Mob->SetAttribute("posX", spawnList[i].position[0]);
			Mob->SetAttribute("posY", spawnList[i].position[1]);
		}
	}


	{
		Xml::XMLElement* Portal = doc->NewElement("Portal");
		ob->LinkEndChild(Portal);
		Portal->SetAttribute("Size", (int)portals.size());


		for (size_t i = 0; i < portals.size(); i++)
		{
			std::string temp = "pt" + std::to_string(i);
			Xml::XMLElement* pt = doc->NewElement(temp.c_str());
			Portal->LinkEndChild(pt);
			pt->SetAttribute("fromX", portals[i].from[0]);
			pt->SetAttribute("fromY", portals[i].from[1]);
			pt->SetAttribute("toX", portals[i].to[0]);
			pt->SetAttribute("toY", portals[i].to[1]);
			pt->SetAttribute("toMapId", portals[i].toMapId);
			float fromX = pt->FloatAttribute("fromX");
			float fromY = pt->FloatAttribute("fromY");
			float toX = pt->FloatAttribute("toX");
			float toY = pt->FloatAttribute("toY");
			int toMapId = pt->IntAttribute("toMapId");
		}
	}



	std::string path = "./Contents/Map/" + std::to_string(mapId) + ".xml";
	doc->SaveFile(path.c_str());
	delete doc;
}

void Map::MapleMap::LoadFile()
{

	Xml::XMLDocument* doc = new Xml::XMLDocument();
	std::string path = "./Contents/Map/" + std::to_string(mapId) + ".xml";
	Xml::XMLError result = doc->LoadFile(path.c_str());
	if (result != Xml::XML_SUCCESS) return;


	Xml::XMLElement* ob;
	ob = doc->FirstChildElement();
	mapId = ob->IntAttribute("MapId");
	back = ob->IntAttribute("Back");
	mapSize[0] = ob->FloatAttribute("MapSizeX");
	mapSize[1] = ob->FloatAttribute("MapSizeY");
	printf("%s\r\n", ob->Attribute("BGM"));

	bgm.Content = ob->Attribute("BGM");
	bgm.loop = true;
	bgm.volume = 0.1f;


	Xml::XMLElement* Objects = ob->FirstChildElement("Objects");
	size_t lenObj = Objects->IntAttribute("Size");
	objects.clear();
	for (size_t i = 0; i < lenObj; i++)
	{
		std::string Tag = "Object" + std::to_string(i);
		Xml::XMLElement* Object = Objects->FirstChildElement(Tag.c_str());
		int objNum = Object->IntAttribute("objNum");
		float objX = Object->FloatAttribute("posX");
		float objY = Object->FloatAttribute("posY");

		objects.push_back(mapObject(objNum, { objX , objY }));
	}

	{
		Xml::XMLElement* Foot = ob->FirstChildElement("Foot");
		Xml::XMLElement* Vertexes;
		
		fVertexes.clear();
		if (Vertexes = Foot->FirstChildElement("Vertexes")) {
			lenObj = Vertexes->IntAttribute("Size");
			for (size_t i = 0; i < lenObj; i++)
			{
				std::string Tag = "Vertex" + std::to_string(i);
				Xml::XMLElement* Vertex = Vertexes->FirstChildElement(Tag.c_str());
				float objX = Vertex->FloatAttribute("posX");
				float objY = Vertex->FloatAttribute("posY");

				fVertexes.push_back(vector<2>{ objX, objY });
			}
		}

		Xml::XMLElement* Indicies;
		fIndicies.clear();
		if (Indicies = Foot->FirstChildElement("Indicies")) {
			lenObj = Indicies->IntAttribute("Size");
			for (size_t i = 0; i < lenObj; i++)
			{
				std::string Tag = "Index" + std::to_string(i);
				Xml::XMLElement* Index = Indicies->FirstChildElement(Tag.c_str());
				int pairA = Index->IntAttribute("pairA");
				int pairB = Index->IntAttribute("pairB");

				fIndicies.push_back(std::pair{ pairA , pairB });
			}
		}
	}

	{
		Xml::XMLElement* Rope = ob->FirstChildElement("Rope");
		Xml::XMLElement* Vertexes = Rope->FirstChildElement("Vertexes");
		lenObj = Vertexes->IntAttribute("Size");
		rVertexes.clear();
		for (size_t i = 0; i < lenObj; i++)
		{
			std::string Tag = "Vertex" + std::to_string(i);
			Xml::XMLElement* Vertex = Vertexes->FirstChildElement(Tag.c_str());
			float objX = Vertex->FloatAttribute("posX");
			float objY = Vertex->FloatAttribute("posY");

			rVertexes.push_back(vector<2>{ objX, objY });
		}

		Xml::XMLElement* Indicies = Rope->FirstChildElement("Indicies");
		lenObj = Indicies->IntAttribute("Size");
		rIndicies.clear();
		for (size_t i = 0; i < lenObj; i++)
		{
			std::string Tag = "Index" + std::to_string(i);
			Xml::XMLElement* Index = Indicies->FirstChildElement(Tag.c_str());
			int pairA = Index->IntAttribute("pairA");
			int pairB = Index->IntAttribute("pairB");

			rIndicies.push_back(std::pair{ pairA , pairB });
		}
	}

	{
		Xml::XMLElement* Monster = ob->FirstChildElement("Monster");
		lenObj = Monster->IntAttribute("Size");
		for (size_t i = 0; i < lenObj; i++)
		{
			std::string Tag = "Mob" + std::to_string(i);
			Xml::XMLElement* Index = Monster->FirstChildElement(Tag.c_str());
			short mobId = static_cast<short>(Index->IntAttribute("mobId"));
			float posX = Index->FloatAttribute("posX");
			float posY = Index->FloatAttribute("posY");

			spawnList.push_back(MobSpawn{ { posX, posY }, mobId, static_cast<short>(i), false });
		}
	}

	{
		Xml::XMLElement* Portal = ob->FirstChildElement("Portal");
		lenObj = Portal->IntAttribute("Size");
		for (size_t i = 0; i < lenObj; i++)
		{
			std::string Tag = "pt" + std::to_string(i);
			Xml::XMLElement* pt = Portal->FirstChildElement(Tag.c_str());
			float fromX = pt->FloatAttribute("fromX");
			float fromY = pt->FloatAttribute("fromY");
			float toX = pt->FloatAttribute("toX");
			float toY = pt->FloatAttribute("toY");
			int toMapId = pt->IntAttribute("toMapId");

			portals.push_back(Poratl{ { fromX, fromY }, {toX, toY}, toMapId });
		}
	}


	

	delete doc;
}

