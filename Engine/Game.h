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
		virtual void Start() abstract;
		virtual bool Update() abstract;
		virtual bool LateUpdate() abstract;
		virtual void End() abstract;
	};

	Game* Initialize();

}

