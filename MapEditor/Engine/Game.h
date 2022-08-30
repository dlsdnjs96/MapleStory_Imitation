#pragma once
#include "Rendering.h"
#include "Sound.h"
#include "input.h"
#include "Time.h"

namespace Engine
{
	class Game abstract
	{
	public:
		float portal_cooltime = 0.0f;

		virtual void Start() abstract;
		virtual bool Update() abstract;
		virtual void End() abstract;
	};

	Game* Initialize();

}

