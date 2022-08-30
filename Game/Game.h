#pragma once
#include "Monster.h"
#include "ObjectPool.h"
#include "Urus.h"


class Game final : public Engine::Game
{
public:
	void Start() override;
	bool Update() override;
	bool LateUpdate() override;
	void End() override;

	void CamUpdate();
	void MapEdit();

	Urus::UrusRoot urus;
	bool imguiSwitch = false;
	//Rendering::Camera camera;

};
