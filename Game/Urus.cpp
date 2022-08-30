#include "stdafx.h"
#include "Xml.h"
#include "Urus.h"
#include "Util.h"
#include "Gui.h"



namespace Urus {

	UrusPart::~UrusPart()
	{
		for (auto& iter : children)
			iter->~UrusPart();
		delete this;
	}

	void UrusPart::AddPart(std::string partName)
	{
		UrusPart* temp = new UrusPart();
		temp->name = partName;
		temp->root = root;
		temp->root->AddList(partName, temp);
		temp->parent = this;
		this->children.push_back(temp);
	}

	const char* UrusPart::GetName()
	{
		return name.c_str();
	}

	void UrusPart::SetAllActivating(bool act)
	{
		for (auto& iter : children)
			iter->SetAllActivating(act);
	}

	void UrusPart::Update()
	{
		if (parent)
			this->position = parent->position + Util::Rotate(parent->transform, parent->rotation);
		//else
		//	position = Util::Rotate(transform, parent->rotation);


		for (auto& iter : children)
			iter->Update();
	}

	void UrusPart::Render()
	{
		if (isActivating) {
			Rendering::Image::Component image;
			image.Content = "Urus/" + name.substr(0, name.find_first_of('_'));
			if (image.IsExist()) {
				image.Location = position;
				image.Angle = rotation;
				image.Length = image.GetLength();
				image.Draw(true);
			}
		}

		for (auto& iter : children)
			iter->Render();
	}

	void UrusPart::Hierarchy()
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			//if (ImGui::IsItemToggledOpen() or ImGui::IsItemClicked())
			//GUI->target = this;

			ImGui::DragFloat("posX", &transform[0], 0.1f);
			ImGui::DragFloat("posY", &transform[1], 0.1f);
			ImGui::DragFloat("Rotation", &rotation, 0.1f);
			ImGui::Checkbox("isActivating", &isActivating);

			for (auto iter : children)
				iter->Hierarchy();

			ImGui::TreePop();
		}
	}


	void UrusPart::LoadPart(Xml::XMLElement* This)
	{
		Xml::XMLElement* component;


		component = This->FirstChildElement("Children");
		int size = component->IntAttribute("Size");

		for (int i = 0; i < size; i++)
		{
		}
	}

	void UrusPart::SavePart(Xml::XMLElement* This, Xml::XMLDocument* doc)
	{
		This->SetAttribute("Name", name.c_str());
		This->SetAttribute("TransX", transform[0]);
		This->SetAttribute("TransY", transform[1]);
		This->SetAttribute("Rotation", rotation);

		Xml::XMLElement* Children = doc->NewElement("Children");
		This->LinkEndChild(Children);
		Children->SetAttribute("Size", (int)children.size());
		for (int i = 0; i < children.size(); i++)
		{
			Xml::XMLElement* child = doc->NewElement(children[i]->GetName());
			Children->LinkEndChild(child);
			SavePart(child, doc);
		}
	}

	UrusPart* UrusPart::Find(std::string name)
	{
		return root->Find(name);
	}

	void UrusPart::AddMotion()
	{
		if (isActivating)
			root->urusMotion[name.c_str()].push_back(FrameT{ transform, rotation, root->motionT });

		for (auto iter : children)
			iter->AddMotion();
	}




	void UrusRoot::AddList(std::string name, UrusPart* ptr)
	{
		partList[name] = ptr;
	}

	UrusRoot::UrusRoot()
	{
		root = this;
		name = "Root";
		parent = nullptr;
		AddList(this->name, this);
	}

	UrusRoot::~UrusRoot()
	{
		//for (auto& iter : children)
		//	iter->~UrusPart();
		delete this;
	}

	void UrusRoot::Update()
	{
		changedState += DELTA;
		switch (state)
		{
		case UrusState::MOTION:
			//Motion();
			break;
		case UrusState::IDLE:
			Idle();
			break;
		case UrusState::WALK:
			Walk();
			break;
		}

		ImGui::Begin("Hierarchy");
		ImGui::PushID(root);

		for (auto iter : children)
			iter->Update(); 
		Hierarchy();

		ImGui::PopID();
		ImGui::End();
	}

	void UrusRoot::Render()
	{
		for (auto& iter : children)
			iter->Render();
	}

	void UrusRoot::Hierarchy()
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			//if (ImGui::IsItemToggledOpen() or ImGui::IsItemClicked())
			//GUI->target = this;

			ImGui::DragFloat("posX", &transform[0], 0.2f);
			ImGui::DragFloat("posY", &transform[1], 0.2f);
			ImGui::DragFloat("Rotation", &rotation, 0.2f);
			ImGui::DragFloat("MotionT", &motionT, 0.2f);

			if (ImGui::Button("AddMotion"))
				AddMotion();
			if (ImGui::Button("SaveMotions"))
				SaveMotions();

			for (auto iter : children)
				iter->Hierarchy();

			ImGui::TreePop();
		}
	}

	void UrusRoot::Motion()
	{
		if (changedState > animations["Motion"]->totalDuration) {
			changedState = 0.f;
			return;
		}
		changedState = 0.f;


		for (auto& it : animations["Motion"]->Frames)
		{
			Find(it.first)->transform = it.second[(int)(animations["Motion"]->framePerSec * changedState)].transform;
			Find(it.first)->rotation = it.second[(int)(animations["Motion"]->framePerSec * changedState)].rotation;
		}
	}

	void UrusRoot::Idle()
	{
		if (changedState > animations["Idle"]->totalDuration) {
			changedState = 0.f;
			return;
		}


		for (auto& it : animations["Idle"]->Frames)
		{
			Find(it.first)->transform = it.second[(int)(animations["Idle"]->framePerSec * changedState)].transform;
			Find(it.first)->rotation = it.second[(int)(animations["Idle"]->framePerSec * changedState)].rotation;
		}
	}

	void UrusRoot::Walk()
	{
	}

	void UrusRoot::SetAllActivating(bool act)
	{
		for (auto& iter : children)
			iter->SetAllActivating(act);
	}

	void UrusRoot::LoadRoot(std::string file)
	{
	}

	void UrusRoot::SaveRoot(std::string file)
	{
		Xml::XMLDocument* doc = new Xml::XMLDocument();
		Xml::XMLElement* ob = doc->NewElement("Root");
		doc->LinkEndChild(ob);


		for (int i = 0; i < children.size(); i++)
		{
			Xml::XMLElement* child = doc->NewElement(children[i]->GetName());
			ob->LinkEndChild(child);
			SavePart(child, doc);
		}


		std::string path = "./Contents/Urus/" + file;
		doc->SaveFile(path.c_str());
		delete doc;
	}

	UrusPart* UrusRoot::Find(std::string name)
	{
		return partList[name];
	}

	Animations* UrusRoot::LoadAni(std::string name)
	{
		Xml::XMLDocument* doc = new Xml::XMLDocument();
		std::string path = "./Contents/Urus/" + name + ".xml";
		Xml::XMLError result = doc->LoadFile(path.c_str());
		if (result != Xml::XML_SUCCESS) return nullptr;

		Animations* temp = new Animations();
		Xml::XMLElement* ob;
		ob = doc->FirstChildElement();
		temp->totalDuration = ob->FloatAttribute("tDuration");
		temp->framePerSec = ob->FloatAttribute("fPerSec");
		size_t len = ob->IntAttribute("Size");

		std::vector<FrameT> frames;

		for (size_t i = 0; i < len; i++)
		{
			std::string Tag = "Animation" + std::to_string(i);
			Xml::XMLElement* Animation = ob->FirstChildElement(Tag.c_str());
			size_t lenAni = Animation->FloatAttribute("Size");
			std::string partName = Animation->Attribute("PartName");

			for (size_t i = 0; i < lenAni; i++)
			{
				std::string Tag2 = "Ani" + std::to_string(i);
				Xml::XMLElement* Ani = Animation->FirstChildElement(Tag2.c_str());
				float time = Ani->FloatAttribute("time");
				float posX = Ani->FloatAttribute("posX");
				float posY = Ani->FloatAttribute("posY");
				float rotation = Ani->FloatAttribute("rotation");

				frames.push_back(FrameT{ {posX, posY}, rotation, time });
			}

			temp->Frames[partName] = new Frame[temp->totalDuration * temp->framePerSec];

			lenAni = frames.size() - 1;
			if (lenAni == 0)
			{
				temp->Frames[partName][0].transform = frames[0].transform;
				temp->Frames[partName][0].rotation = frames[0].rotation;
			}
			for (size_t i = 0; i < lenAni; i++)
			{
				float lerp = frames[i + 1].time * temp->framePerSec - frames[i].time * temp->framePerSec;
				for (size_t j = frames[i].time * temp->framePerSec; j < frames[i + 1].time * temp->framePerSec; j++)
				{
					temp->Frames[partName][j].transform[0] = Util::Lerp(frames[i].transform[0], frames[i + 1].transform[0], j / lerp);
					temp->Frames[partName][j].transform[1] = Util::Lerp(frames[i].transform[1], frames[i + 1].transform[1], j / lerp);
					temp->Frames[partName][j].rotation = Util::Lerp(frames[i].rotation, frames[i + 1].rotation, j / lerp);
				}
			}
		}

		return temp;
	}

	void UrusRoot::LoadAnimations()
	{
		if (std::filesystem::exists(URUS_ANIMATION_PATH))
		{
			for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(URUS_ANIMATION_PATH))
			{
				std::string file = path.string();
				if (std::filesystem::is_regular_file(path) && file.substr(file.length() - 3, file.length()).compare("xml") == 0)
				{
					std::replace(file.begin(), file.end(), '\\', '/');
					file = file.substr(file.find_last_of('/') + 1, file.length() - file.find_last_of('/') - 5);

					animations[file.c_str()] = LoadAni(file.c_str());
				}
			}
		}

		for (auto& it : animations["Motion"]->Frames)
		{
			Find(it.first)->transform = it.second[0].transform;
			Find(it.first)->rotation = it.second[0].rotation;
		}
	}

	void UrusRoot::AddMotion()
	{
		urusMotion[name.c_str()].push_back(FrameT{transform, rotation, root->motionT});

		for (auto iter : children)
			iter->AddMotion();
	}

	void UrusRoot::SaveMotions()
	{
		Xml::XMLDocument* doc = new Xml::XMLDocument();
		Xml::XMLElement* ob = doc->NewElement("Root");
		doc->LinkEndChild(ob);


		ob->SetAttribute("tDuration", motionT);
		ob->SetAttribute("fPerSec", 30);
		ob->SetAttribute("Size", (int)urusMotion.size());


		size_t idx = 0;
		for (auto& it : urusMotion)
		{
			std::string temp = "Animation" + std::to_string(idx);
			Xml::XMLElement* Animation = doc->NewElement(temp.c_str());
			ob->LinkEndChild(Animation);
			Animation->SetAttribute("Size", (int)it.second.size());
			Animation->SetAttribute("PartName", it.first.c_str());

			for (size_t i = 0; i < it.second.size(); i++)
			{
				temp = "Ani" + std::to_string(i);
				Xml::XMLElement* Ani = doc->NewElement(temp.c_str());
				Animation->LinkEndChild(Ani);
				Ani->SetAttribute("time", it.second[i].time);
				Ani->SetAttribute("posX", it.second[i].transform[0]);
				Ani->SetAttribute("posY", it.second[i].transform[1]);
				Ani->SetAttribute("rotation", it.second[i].rotation);
			}
			idx++;
		}


		std::string path = "./Contents/Urus/Motion.xml";
		doc->SaveFile(path.c_str());
		delete doc;
	}

}