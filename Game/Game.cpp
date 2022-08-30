
#include "stdafx.h"

#include "Urus.h"
#include "Game.h"
#include "Util.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Monster.h"
#include "UI.h"
#include "MonsterData.h"
#include "Gui.h"



void Game::Start()
{
	//mainCam = new Rendering::Camera();
	uiCam = new Rendering::Camera();
	
	PLAYER.Start();
	WORLDI.Start();
	MonsterData::getIncetance().LoadAllData();

	//urus.AddPart("root");
	urus.Find("Root")->AddPart("body");
	urus.Find("body")->AddPart("bodyToChin");
	urus.Find("bodyToChin")->AddPart("chin");
	urus.Find("body")->AddPart("bodyToHead");
	urus.Find("bodyToHead")->AddPart("head");
	urus.Find("body")->AddPart("bodyToLeg");
	urus.Find("bodyToLeg")->AddPart("leg");
	urus.Find("body")->AddPart("bodyToArm1");
	urus.Find("bodyToArm1")->AddPart("Larm1");
	urus.Find("Larm1")->AddPart("arm1ToArm2");
	urus.Find("arm1ToArm2")->AddPart("Larm2");
	urus.Find("Larm2")->AddPart("claw_1");
	urus.LoadAnimations();

	return;
}

bool Game::Update()
{
	//printf("Delta %f\r\n", Time::Get::Delta());



	CamUpdate();
	MapEdit();

	if (GUI.active) {
		GUI.Update();
		WORLDI.RenderGUI();
	}

	WORLDI.Update();
	PLAYER.Update();
	WORLDI.LateUpdate();


	//urus.Find("Root")->position = PLAYER.position;
	//urus.Update();
	//urus.Render();


	uiCam->Location = { 0.f, 0.f };
	uiCam->Set();


	UI::DrawUI();

	CamUpdate();
	GetMainCam()->Set();
	if (GUI.active) {
		GUI.Render();
	}
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return false;
}

bool Game::LateUpdate()
{
	return false;
}

void Game::End()
{
	return;
}

Rendering::Camera* Engine::Game::GetMainCam()
{
	static Rendering::Camera* instance = nullptr;
	if (instance == nullptr)
		instance = new Rendering::Camera();
	return instance;
}

void Game::CamUpdate()
{
	vector<2> mapBoundary = (CURRENT_MAP->mapSize - vector<2>{ 1280.f, 720.f }) / 2.f;

	GetMainCam()->Location = {
		Util::Lerp(GetMainCam()->Location[0], PLAYER.GetPosition()[0], LERP_CAMERA_SPEED * DELTA),
		Util::Lerp(GetMainCam()->Location[1], PLAYER.GetPosition()[1], LERP_CAMERA_SPEED * DELTA)
	};


	if (GetMainCam()->Location[0] > abs(mapBoundary[0]))
		GetMainCam()->Location[0] = abs(mapBoundary[0]);
	else if (GetMainCam()->Location[0] < -abs(mapBoundary[0]))
		GetMainCam()->Location[0] = -abs(mapBoundary[0]);

	if (GetMainCam()->Location[1] > abs(mapBoundary[1]))
		GetMainCam()->Location[1] = abs(mapBoundary[1]);
	else if (GetMainCam()->Location[1] < -abs(mapBoundary[1]))
		GetMainCam()->Location[1] = -abs(mapBoundary[1]);


	GetMainCam()->Set();
}

void Game::MapEdit()
{
	if (Input::Get::Key::OnceDown('Q'))
		printf("Player Position %f %f\r\n", PLAYER.position[0], PLAYER.position[1]);

	if (!Input::Get::Key::Press(VK_SHIFT)) {
		if (Input::Get::Key::OnceDown(VK_LBUTTON)) {
			LONG mouse_x = Input::Get::Cursor::X() - (1280 / 2.0f);
			LONG mouse_y = (720 - Input::Get::Cursor::Y()) - (720 / 2.0f) - 25.f;
			mouse_x += GetMainCam()->Location[0];
			mouse_y += GetMainCam()->Location[1];
			printf("Mouse point %ld %ld \r\n", mouse_x, mouse_y);
		}
		else if (Input::Get::Key::OnceDown(VK_RBUTTON)) {
			float mouse_x = Input::Get::Cursor::X() - (1280 / 2.0f);
			float mouse_y = (720 - Input::Get::Cursor::Y()) - (720 / 2.0f) - 25.f;
			mouse_x += GetMainCam()->Location[0];
			mouse_y += GetMainCam()->Location[1];
			PLAYER.position = { static_cast<float>(mouse_x), static_cast<float>(mouse_y) };
			PLAYER.ChangeState(PlayerState::FALL);
			//printf("Mouse point %ld %ld \r\n", mouse_x, mouse_y);
		}
		if (Input::Get::Key::OnceDown('I'))
			GUI.active = !GUI.active;
		if (Input::Get::Key::OnceDown('M'))
		{
			//Util::BezierCurve3(PLAYER.GetPosition(), temp, target)
		}
	}

	// Making Map
	if (Input::Get::Key::Press(VK_SHIFT))
	{
		static int prev_vertex = -1;
		static bool isFoot = true;

		if (Input::Get::Key::Press('M') && Input::Get::Key::OnceDown(VK_LBUTTON))
		{
			float mouse_x = Input::Get::Cursor::X() - (1280 / 2.0f) + GetMainCam()->Location[0];
			float mouse_y = (720 - Input::Get::Cursor::Y()) - (720 / 2.0f) - 15.f + GetMainCam()->Location[1];

			CURRENT_MAP->spawnList.push_back(Map::MobSpawn{ {mouse_x, mouse_y}, 1000, static_cast<short>(CURRENT_MAP->spawnList.size()), false });
		}

		if (Input::Get::Key::OnceDown(VK_LBUTTON)) {
			float mouse_x = Input::Get::Cursor::X() - (1280 / 2.0f) + GetMainCam()->Location[0];
			float mouse_y = (720 - Input::Get::Cursor::Y()) - (720 / 2.0f) - 15.f + GetMainCam()->Location[1];

			if (isFoot) {
				CURRENT_MAP->fVertexes.push_back({ mouse_x, mouse_y });
				if (prev_vertex != -1)
					CURRENT_MAP->fIndicies.push_back({ prev_vertex, CURRENT_MAP->fVertexes.size() - 1 });
				prev_vertex = CURRENT_MAP->fVertexes.size() - 1;
			}
			else
			{
				CURRENT_MAP->rVertexes.push_back({ mouse_x, mouse_y });
				if (prev_vertex != -1)
					CURRENT_MAP->rIndicies.push_back({ prev_vertex, CURRENT_MAP->rVertexes.size() - 1 });
				prev_vertex = CURRENT_MAP->rVertexes.size() - 1;
			}
			return;
		}

		if (Input::Get::Key::Press('F'))
		{
			vector<2> mPoint = { Input::Get::Cursor::X() - (1280 / 2.0f) + GetMainCam()->Location[0], (720 - Input::Get::Cursor::Y()) - (720 / 2.0f) - 15.f + GetMainCam()->Location[1] };

			float min = FLT_MAX;
			int idx;
			for (int i = 0; i < CURRENT_MAP->fVertexes.size(); i++)
			{
				if (min > length(CURRENT_MAP->fVertexes[i] - mPoint))
				{
					idx = i;
					min = length(CURRENT_MAP->fVertexes[i] - mPoint);
				}
			}
			printf("Nearst Vertex %d\r\n", idx);
		}

		if (Input::Get::Key::OnceDown('C'))
			isFoot = !isFoot;
		if (Input::Get::Key::OnceDown('R'))
			prev_vertex = -1;
		if (Input::Get::Key::OnceDown('D'))
			CURRENT_MAP->LoadFile();
		if (Input::Get::Key::OnceDown('S'))
		{
			CURRENT_MAP->SaveFile();
			printf("Saved Map File\r\n");
		}
	}
}

Engine::Game* Engine::Initialize()
{
	return new ::Game();
}