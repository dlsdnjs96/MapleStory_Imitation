#include "stdafx.h"

#include "Map.h"
#include "Xml.h"


void Map::MapleMap::Update()
{
	Rendering::Camera camera;
	camera.Location = {0.0f, 0.0f};
	camera.Set();
	Rendering::Image::Component image;
	size_t lenVector = objects.size();
	std::string path = "Map/Object/";

	for (size_t i = 0; i < lenVector; i++) {
		image.Content = "Map/Object/" + std::to_string(objects[i].objectNum);
		image.Position = objects[i].position;
		image.Length = image.GetLength();
		image.Draw(true);
	}
}


void Map::MapleMap::SaveFile()
{
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	Xml::XMLElement* ob = doc->NewElement("Root");
	doc->LinkEndChild(ob);


	ob->SetAttribute("MapId", mapId);
	ob->SetAttribute("MapSizeX", mapSize[0]);
	ob->SetAttribute("MapSizeY", mapSize[1]);

	Xml::XMLElement* Objects = doc->NewElement("Objects");
	ob->LinkEndChild(Objects);
	Objects->SetAttribute("Size", (int)objects.size());
	int i = 0;
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		std::string temp = "Object" + std::to_string(i++);
		Xml::XMLElement* Object = doc->NewElement(temp.c_str());
		Objects->LinkEndChild(Object);
		Object->SetAttribute("objNum", objects[i].objectNum);
		Object->SetAttribute("posX", objects[i].position[0]);
		Object->SetAttribute("posY", objects[i].position[1]);
	}


	std::string path = "./Contents/Map/" + std::to_string(mapId);
	doc->SaveFile(path.c_str());
	delete doc;
}

void Map::MapleMap::LoadFile()
{
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	std::string path = "./Contents/Map/" + std::to_string(mapId);
	Xml::XMLError result = doc->LoadFile(path.c_str());
	if (result != Xml::XML_SUCCESS) return;


	Xml::XMLElement* ob;
	ob = doc->FirstChildElement();
	mapId = ob->IntAttribute("MapId");
	mapSize[0] = ob->FloatAttribute("MapSizeX");
	mapSize[1] = ob->FloatAttribute("MapSizeY");


	Xml::XMLElement* Objects = ob->FirstChildElement("Objects");
	size_t lenObj = Objects->IntAttribute("Size");
	for (size_t i = 0; i < lenObj; i++)
	{
		std::string Tag = "Object" + std::to_string(i);
		Xml::XMLElement* Object = Objects->FirstChildElement(Tag.c_str());
		int objNum = Object->IntAttribute("objNum");
		int objX = Object->FloatAttribute("posX");
		int objY = Object->FloatAttribute("posY");

		objects.push_back(mapObject(objNum, { objX , objY }));

	}


	delete doc;
}

void Map::MapleMap::RenderHierarchy()
{
	ImGui::Begin("Map");
	if (ImGui::BeginTabBar("Verticies")) {
		//if (ImGui::BeginTabItem("Transform"))
		//{
		//	ImGui::EndTabItem();
		//}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

