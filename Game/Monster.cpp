#include "stdafx.h"
#include "Monster.h"
#include "Util.h"
#include "Player.h"
#include "Map.h"
#include "World.h"
#include "MonsterData.h"



void MONSTER::Monster::Start(int mobId)
{
	this->mobId = mobId;
	state = MonsterState::FALL;
	leftDuration = Util::randTime(3.f, 5.f);
	isAlive = true;
	hp = MOBDATA.GetData(mobId)->maxHp;

	switch (mobId)
	{
	case 1000:
		//chaseRange = 1500.f;
		//attackRange = 1000.f;
		//movingSpeed = 100.f;
		//steppingFoot = 0;
		//isAlive = true;

		//stateImageSize[static_cast<int>(MonsterState::IDLE)] = 2;
		//stateImageSize[static_cast<int>(MonsterState::WALK)] = 3;
		//stateImageSize[static_cast<int>(MonsterState::JUMP)] = 1;
		//stateImageSize[static_cast<int>(MonsterState::FALL)] = 1;
		//stateImageSize[static_cast<int>(MonsterState::CHASE)] = 3;
		//stateImageSize[static_cast<int>(MonsterState::ATTACK)] = 3;
		//stateImageSize[static_cast<int>(MonsterState::DIE)] = 3;
		//stateImageSize[static_cast<int>(MonsterState::HIT)] = 1;

		//hitrange = Collision::RectAngle{ {63.f, 58.f}, 0.f, {0.f, 29.f} };
		//hitbox.Length = hitrange.Length;

		//movingType = MOB_AVAILABLE_ATTACK;
		break;
	}
}

void MONSTER::Monster::Update()
{
	stateTime += DELTA;

	Render();
	switch (state)
	{
	case MonsterState::IDLE:
		Idle();
		break;
	case MonsterState::WALK:
		Walk();
		break;
	case MonsterState::JUMP:
		Jump();
		break;
	case MonsterState::FALL:
		Fall();
		break;
	case MonsterState::CHASE:
		Chase();
		break;
	case MonsterState::ATTACK:
		Attack();
		break;
	case MonsterState::HIT:
		Hit();
		break;
	case MonsterState::DIE:
		Die();
		break;
	}

	//printf("%s\r\n", GetStateStr(state).c_str());
}

void MONSTER::Monster::LateUpdate()
{
}

void MONSTER::Monster::End()
{
}

void MONSTER::Monster::Idle()
{
	if (LookingForTarget())
		return;

	leftDuration -= DELTA;

	if (leftDuration <= 0.f)
	{
		leftDuration = Util::randTime(3.f, 5.f);
		looking = static_cast<bool>(rand() % 2);
		ChangeState(MonsterState::WALK);

	}
}

void MONSTER::Monster::Walk()
{
	if (LookingForTarget())
		return;
	
	leftDuration -= DELTA;

	if (leftDuration <= 0.f)
	{
		leftDuration = Util::randTime(3.f, 5.f);
		if (static_cast<bool>(rand() % 2))
			looking = !looking;
		else 
			ChangeState(MonsterState::IDLE);

		return;
	}

	if (looking) 
		position[0] += (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
	else 
		position[0] -= (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
	

	// 발판을 안 벗어났을 경우
	if (!Util::OutOfLineX(CURRENT_MAP->GetFoot(steppingFoot), position[0]))
		// 발판에 맞게 높이 조정
		position[1] = Util::GetHeightOnFoot(CURRENT_MAP->GetFoot(steppingFoot), position[0]);
	// 발판을 벗어날 경우
	else {
		// 현재위치에 새로운 발판 찾기
		int findFoot = FindFoot();

		// 발판이 있을경우 
		if (findFoot != -1)
		{
			// 해당 발판으로 이동후 높이 조정
			steppingFoot = findFoot;
			position[1] = Util::GetHeightOnFoot(CURRENT_MAP->GetFoot(steppingFoot), position[0]);
		}
		// 발판이 없을경우 추락
		else {
			if ((MOBDATA.GetData(mobId)->movingType & MOB_JUMP_TO_FALL) != 0) {
				steppingFoot = -1;
				ChangeState(MonsterState::FALL);
				return;
			}
			else {
				looking = !looking;
				if (looking)
					position[0] += (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
				else
					position[0] -= (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
				return;
			}
		}
	}
}

void MONSTER::Monster::Jump()
{
}

void MONSTER::Monster::Fall()
{
	position[1] -= (MOBDATA.GetData(mobId)->movingSpeed * DELTA) * 1.5f;

	// 현재위치에 새로운 발판 찾기
	int findFoot = FindFoot();

	// 발판이 있을경우 
	if (findFoot != -1)
	{
		// 해당 발판으로 이동후 높이 조정
		steppingFoot = findFoot;
		position[1] = Util::GetHeightOnFoot(CURRENT_MAP->GetFoot(steppingFoot), position[0]);
		ChangeState(MonsterState::IDLE);
		leftDuration = Util::randTime(1.f, 2.f);
	}
}

void MONSTER::Monster::Chase()
{
	static float chasingTime = 0.f;
	chasingTime += DELTA;

	if (length(PLAYER.position - position) > MOBDATA.GetData(mobId)->chaseRange)
	{
		ChangeState(MonsterState::IDLE);
		return;
	}

	if (chasingTime > 2.f) {
		if (PLAYER.position[0] > position[0])
			looking = LOOKING_RIGHT;
		else
			looking = LOOKING_LEFT;
		chasingTime = 0.f;
	}

	if (looking)
		position[0] += (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
	else
		position[0] -= (MOBDATA.GetData(mobId)->movingSpeed * DELTA);


	// 발판을 안 벗어났을 경우
	if (!Util::OutOfLineX(CURRENT_MAP->GetFoot(steppingFoot), position[0]))
		// 발판에 맞게 높이 조정
		position[1] = Util::GetHeightOnFoot(CURRENT_MAP->GetFoot(steppingFoot), position[0]);
	// 발판을 벗어날 경우
	else {
		// 현재위치에 새로운 발판 찾기
		int findFoot = FindFoot();

		// 발판이 있을경우 
		if (findFoot != -1)
		{
			// 해당 발판으로 이동후 높이 조정
			steppingFoot = findFoot;
			position[1] = Util::GetHeightOnFoot(CURRENT_MAP->GetFoot(steppingFoot), position[0]);
		}
		// 발판이 없을경우 추락
		else {
			if ((MOBDATA.GetData(mobId)->movingType & MOB_JUMP_TO_FALL) != 0) {
				steppingFoot = -1;
				ChangeState(MonsterState::FALL);
				return;
			}
			else {
				looking = !looking;
				if (looking)
					position[0] += (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
				else
					position[0] -= (MOBDATA.GetData(mobId)->movingSpeed * DELTA);
				return;
			}
		}
	}
}

void MONSTER::Monster::Attack()
{
}

void MONSTER::Monster::Hit()
{

	if (stateTime > MOTION_FRAME)
		ChangeState(MonsterState::FALL);
}

void MONSTER::Monster::Die()
{
	if (stateTime > MOTION_FRAME * MOBDATA.GetData(mobId)->stateImageSize[static_cast<int>(state)]) {
		isAlive = false;
	}
}

void MONSTER::Monster::Render()
{
	Rendering::Image::Component image;


	//changedState -= stateImageSize[static_cast<int>(state)] * MOTION_FRAME;
	image.Content = "Mob/" + std::to_string(mobId) + "/" + GetStateName(state)+"/"+std::to_string(static_cast<int>(stateTime / MOTION_FRAME) % MOBDATA.GetData(mobId)->stateImageSize[static_cast<int>(state)]);
	image.Length = image.GetLength();
	image.Location = { position[0], position[1] + (image.Length[1] / 2.f) };
	image.Draw(looking);
}



void MONSTER::Monster::ChangeState(MonsterState state)
{
	stateTime = 0.f;
	this->state = state;
}

void MONSTER::Monster::HitByPlayer(short damage)
{
	Util::PlayerSound(std::to_string(mobId) + "_hit");
	ChangeState(MonsterState::HIT);
	hp -= damage;
	if (hp <= 0)
		ChangeState(MonsterState::DIE);
}

bool MONSTER::Monster::LookingForTarget()
{
	if (!(MOBDATA.GetData(mobId)->movingType & MOB_FIRST_ATTACK))
		return false;
	if (length(PLAYER.position - position) < MOBDATA.GetData(mobId)->chaseRange)
	{
		state = MonsterState::CHASE;
		return true;
	}
	return false;
}

int MONSTER::Monster::FindFoot()
{
	size_t lenIdc = CURRENT_MAP->fIndicies.size();
	for (size_t i = 0; i < lenIdc; i++)
	{
		vector<2> hit = { position[0], position[1] + 5.f };

		if (Util::GetCrossPoint(hit, position,
			CURRENT_MAP->GetFoot(i).p1, CURRENT_MAP->GetFoot(i).p2)) {
			return i;
		}
	}
	return -1;
}

std::string  MONSTER::Monster::GetStateName(MonsterState state)
{
	switch (state)
	{
	case MonsterState::IDLE:
		return "stand";
	case MonsterState::WALK:
		return "move";
	case MonsterState::JUMP:
	case MonsterState::FALL:
		return "jump";
	case MonsterState::DIE:
		return "die";
	case MonsterState::CHASE:
		return "move";
	case MonsterState::HIT:
		return "hit";
	}
	return "";
}

std::string MONSTER::Monster::GetStateStr(MonsterState state)
{
	switch (state)
	{
	case MonsterState::IDLE:
		return "IDLE";
	case MonsterState::WALK:
		return "WALK";
	case MonsterState::JUMP:
		return "JUMP";
	case MonsterState::FALL:
		return "FALL";
	case MonsterState::CHASE:
		return "CHASE";
	case MonsterState::HIT:
		return "HIT";
	case MonsterState::DIE:
		return "DIE";
	case MonsterState::ATTACK:
		return "ATTACK";
	}
	return "";
}
