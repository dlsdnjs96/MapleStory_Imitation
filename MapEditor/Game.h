#pragma once



class Game final : public Engine::Game
{
public:
	void Start() override;
	bool Update() override;
	void End() override;

};
