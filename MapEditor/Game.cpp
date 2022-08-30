
#include "stdafx.h"

#include "Game.h"
#include "World.h"
#include "Map.h"



void Game::Start()
{
	World::LoadMapData();


	return;
}

bool Game::Update()
{
	ImGui::Begin("Hierarchy");
	World::Update();
	ImGui::End();

	return false;
}

void Game::End()
{
	return;
}

Engine::Game* Engine::Initialize()
{
	return new ::Game();
}