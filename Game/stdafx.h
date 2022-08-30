#pragma once
//
//template<class T>using uPtr = std::unique_ptr<T>;
//template<class T>using sPtr = std::shared_ptr<T>;
//template<class T>using wPtr = std::weak_ptr<T>;



#include "Engine/Collision.h"
#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/input.h"
#include "Engine/matrix.h"
#include "Engine/Pipeline.h"
#include "Engine/Rendering.h"
#include "Engine/Sound.h"
#include "Engine/Time.h"
#include "Engine/vector.h"

struct MonsterStat
{
	float			chaseRange;
	float			attackRange;
	float			movingSpeed;
	short			stateImageSize[10];
	short			maxHp;
	short			physicalAttack;
	short			physicalAmour;
	char			movingType;
	Collision::RectAngle		hitbox;
};


template <typename T>
class Singleton
{
	//static
private:
	static T* instance;
public:
	static T &getIncetance()
	{
		if (!instance)
			instance = new T;

		return *instance;
	};
	//member
public:
	virtual ~Singleton() {};
	void DeleteSingleton()
	{
		delete instance;
		instance = nullptr;
	};
};
template <typename T>
T* Singleton<T>::instance = nullptr;

#include <unordered_map>
#include <queue>
#include <deque>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>


//Imgui
#include "../.lib/ImGui/imgui.h"
#include "../.lib/ImGui/imgui_internal.h"
#include "../.lib/ImGui/imgui_impl_dx11.h"
#include "../.lib/ImGui/imgui_impl_win32.h"
#pragma comment(lib, "../.lib/ImGui/example_win32_directx11.lib")

//Dialog
#include "../.lib/ImGui/dirent.h"
#include "../.lib/ImGui/ImGuiFileDialog.h"
#include "../.lib/ImGui/ImGuiFileDialogConfig.h"

#include "Gui.h"


#define DELTA Time::Get::Delta()
#define ELAPSED Time::Get::Elapsed()

#define WORLDI WORLD::World::getIncetance()
#define CURRENT_MAP WORLD::World::getIncetance().GetCurrentMap()
#define PLAYER Player::getIncetance()
#define	MOBDATA	MonsterData::getIncetance()
#define GUI	Gui::getIncetance()
#define	MAINCAM Engine::Game::GetMainCam()

#define LOOKING_LEFT false
#define LOOKING_RIGHT true
#define MOTION_FRAME 0.3f
#define LERP_CAMERA_SPEED 0.3f
#define BLEND_ANIMATION_TIME 0.3f
#define JUMPING_TIME 0.4f

#define MAP_PATH "./Contents/Map/"
#define MOB_PATH "./Contents/Mob/"
#define URUS_ANIMATION_PATH "./Contents/Urus/"

#define MOB_JUMP_TO_FALL		0b00000001
#define MOB_AVAILABLE_ATTACK	0b00000010
#define MOB_FIRST_ATTACK		0b00000100

#define	TORADIAN			0.0174532f
#define	PI_2				6.2831853f  //360
#define	PI					3.1415926f	//180
#define	PI_DIV2				1.5707963f	//90
#define	PI_DIV4				0.7853981f	//45
#define	PI_DIV8				0.3926991f	//22.5




typedef struct Line {
	vector<2> p1;
	vector<2> p2;
}Line;

struct DamageText
{
	float addedTime;
	vector<2> position;
	char text[8];
	bool monster;
};



//typedef std::shared_ptr<Map::MapleMap> MapSptr;