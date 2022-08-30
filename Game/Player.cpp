#include "stdafx.h"
#include "Player.h"
#include "Util.h"
#include "World.h"
#include "Camera.h"
#include "Game.h"
#include "MonsterData.h"
#include "DamageSystem.h"




void Player::Start()
{
	changedState = 0;
	movingSpeed = 160;
	jumpingSpeed = 140;
	invincibleTime = 0.f;

	position = { 0, 100 };
	movingForce = { 0, 0 };
	state = PlayerState::IDLE;
	isAttacked = false;

	stateImageSize[static_cast<int>(PlayerState::IDLE)] = 1;
	stateImageSize[static_cast<int>(PlayerState::WALK)] = 4;
	stateImageSize[static_cast<int>(PlayerState::JUMP)] = 1;
	stateImageSize[static_cast<int>(PlayerState::FALL)] = 1;
	stateImageSize[static_cast<int>(PlayerState::LIEDOWN)] = 1;
	stateImageSize[static_cast<int>(PlayerState::ROPE)] = 2;
	stateImageSize[static_cast<int>(PlayerState::ATTACK_STAND)] = 3;
	stateImageSize[static_cast<int>(PlayerState::ATTACK_UP)] = 3;
	stateImageSize[static_cast<int>(PlayerState::ATTACK_DOWN)] = 2;
	stateImageSize[static_cast<int>(PlayerState::ALERT)] = 3;
}

void Player::Update()
{
	//printf("%.4f %.4f\r\n", position[0], position[1]);
	static PlayerState prev = PlayerState::IDLE;
	prev = state;
	//state = PlayerState::IDLE;
	switch (state)
	{
	case PlayerState::IDLE:
	case PlayerState::ALERT:
		Idle();
		break;
	case PlayerState::WALK:
		Walk();
		break;
	case PlayerState::JUMP:
		Jump();
		break;
	case PlayerState::FALL:
		Fall();
		break;
	case PlayerState::LIEDOWN:
		LieDown();
		break;
	case PlayerState::ROPE:
		Rope();
		break;
	case PlayerState::ATTACK_STAND:
		AttackStand();
		break;
	case PlayerState::ATTACK_UP:
		AttackUp();
		break;
	case PlayerState::ATTACK_DOWN:
		AttackDown();
		break;
	}
	position += movingForce * DELTA * 100;
	//movingForce[0] = movingForce[0] < 0 ? movingForce[0] + DELTA : movingForce[0] - DELTA;
	//movingForce[1] = movingForce[1] < 0 ? movingForce[1] + DELTA : movingForce[1] - DELTA;
	//if (prev != state)
	//	printf("%d\r\n", state);

	if (Input::Get::Key::OnceDown('A'))
	{
		CURRENT_MAP->activeMonsters.front()->Die();
	}

	CheckCollision();

	changedState += DELTA;
	while (changedState >= stateImageSize[static_cast<UINT>(state)] * MOTION_FRAME)
		changedState -= stateImageSize[static_cast<UINT>(state)] * MOTION_FRAME;
	CheckMapArea();
	Render();
}
void Player::End()
{
}
void Player::Idle()
{
	FindPortal();
	if (invincibleTime > ELAPSED)
		state = PlayerState::ALERT;
	else
		state = PlayerState::IDLE;

	// 방향키로 이동시 WALK로 변경
	if (Input::Get::Key::Press(VK_LEFT) || Input::Get::Key::Press(VK_RIGHT))
		ChangeState(PlayerState::WALK);

	// 제자리 점프
	if (Input::Get::Key::OnceDown(VK_MENU)) {
		ChangeState(PlayerState::JUMP);
		if (Input::Get::Key::Press(VK_LEFT))
			jumpingForce = -movingSpeed;
		else if (Input::Get::Key::Press(VK_RIGHT))
			jumpingForce = +movingSpeed;
		else
			jumpingForce = 0.f;
		jumpingTime = 0.0f;
	}

	// 엎드린 자세로 변경
	else if (Input::Get::Key::Press(VK_DOWN))
		ChangeState(PlayerState::LIEDOWN);
	else if ((Input::Get::Key::Press(VK_UP) || Input::Get::Key::Press(VK_DOWN)) && LookingRope())
		return;
	else if (Input::Get::Key::OnceDown(VK_CONTROL))
		ChangeState(PlayerState::ATTACK_STAND);
}
void Player::Walk()
{
	FindPortal();

	// 좌우 방향키를 누르고 있을 경우
	if (Input::Get::Key::Press(VK_LEFT) || Input::Get::Key::Press(VK_RIGHT)) {

		// 왼쪽으로 이동
		if (Input::Get::Key::Press(VK_LEFT)) {
			looking = LOOKING_LEFT;
			position[0] -= (movingSpeed * DELTA);
		}
		// 오른쪽으로 이동
		else {
			looking = LOOKING_RIGHT;
			position[0] += (movingSpeed * DELTA);
		}
		if ((Input::Get::Key::Press(VK_UP) || Input::Get::Key::Press(VK_DOWN)) && LookingRope())
			return;
			

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
				steppingFoot = -1;
				ChangeState(PlayerState::FALL);
				return;
			}
		}
	}
	// 방향키를 안 누르고 있을경우 멈춤
	else 
		ChangeState(PlayerState::IDLE);
		
	// 점프키를 누를시
	if (Input::Get::Key::OnceDown(VK_MENU)) {
		ChangeState(PlayerState::JUMP);
		if (Input::Get::Key::Press(VK_LEFT))
			jumpingForce = -movingSpeed;
		else if (Input::Get::Key::Press(VK_RIGHT))
			jumpingForce = +movingSpeed;
		else
			jumpingForce = 0.f;
		jumpingTime = 0.0f;
	} 
}
void Player::Jump()
{
	//printf("jumpingTime %f\r\n", jumpingTime);
	jumpingTime += DELTA;
	position[1] += (Util::Slerp(0.f, JUMPING_TIME, jumpingTime) - Util::Slerp(0.f, JUMPING_TIME, jumpingTime - DELTA)) * jumpingSpeed * 3.f;

	if (Input::Get::Key::Press(VK_LEFT)) {
		looking = LOOKING_LEFT;
	}
	else if (Input::Get::Key::Press(VK_RIGHT)) {
		looking = LOOKING_RIGHT;
	}
	position += { jumpingForce * DELTA, 0.f };

	if (jumpingTime >= JUMPING_TIME) {
		ChangeState(PlayerState::FALL);
	}
	if (Input::Get::Key::Press(VK_UP) && LookingRope())
		return;
}
void Player::Fall()
{
	movingForce[0] = 0;
	movingForce[1] = -2.5f * jumpingSpeed / 100.f;


	if (Input::Get::Key::Press(VK_LEFT)) {
		looking = LOOKING_LEFT;
		position -= { movingSpeed* DELTA, 0.f };
	}
	else if (Input::Get::Key::Press(VK_RIGHT)) {
		looking = LOOKING_RIGHT;
		position += { movingSpeed* DELTA, 0.f };
	}

	if (Input::Get::Key::Press(VK_UP) && LookingRope())
		return;
		

	size_t lenIdc = CURRENT_MAP->fIndicies.size();
	for (size_t i = 0; i < lenIdc; i++)
	{
		vector<2> hit = { position[0], position[1] + 5.f };
		if (Util::GetCrossPoint( hit, position,
			CURRENT_MAP->GetFoot(i).p1, CURRENT_MAP->GetFoot(i).p2)) {

			position = hit;
			steppingFoot = i;
			movingForce = { 0, 0 };
			ChangeState(PlayerState::IDLE);
			return;
		}
	}
}
void Player::LieDown()
{
	if (!Input::Get::Key::Press(VK_DOWN)) {
		ChangeState(PlayerState::IDLE);
	}
	else if (Input::Get::Key::OnceDown(VK_MENU)) {
		position[1] -= DELTA * jumpingSpeed;
		ChangeState(PlayerState::FALL);
	}
	else if (Input::Get::Key::OnceDown(VK_CONTROL)) {
		ChangeState(PlayerState::ATTACK_DOWN);
	}
}
void Player::Rope()
{
	if (Input::Get::Key::Press(VK_UP)) {
		position[1] += DELTA * 100.f;
	} else if (Input::Get::Key::Press(VK_DOWN)) {
		position[1] -= DELTA * 100.f;
	}
	if (static_cast<int>(position[1] / 20.f) % 2 == 0)
		changedState = 0.f;
	else
		changedState = MOTION_FRAME;

	if (Input::Get::Key::OnceDown(VK_MENU)) {

		if (Input::Get::Key::Press(VK_LEFT)) {
			looking = LOOKING_LEFT;
			jumpingForce = -movingSpeed;
			ChangeState(PlayerState::JUMP);
			jumpingTime = 0.4f;
		}
		else if (Input::Get::Key::Press(VK_RIGHT)) {
			looking = LOOKING_RIGHT;
			jumpingForce = +movingSpeed;
			ChangeState(PlayerState::JUMP);
			jumpingTime = 0.4f;
		}
		Util::PlayerSound("Jump");
	}
	if (Util::OutOfLineY(CURRENT_MAP->GetRope(keepingRope), position[1]))
	{
		ChangeState(PlayerState::FALL);
	}
	//if (Util::OutOfLineY(CURRENT_MAP->GetRope(keepingRope), position[1]))
}
void Player::AttackStand()
{
	if (changedState + DELTA >= MOTION_FRAME * 3.f)
	{
		ChangeState(PlayerState::IDLE);
		isAttacked = false;
		return;
	}
	if (!isAttacked && changedState > MOTION_FRAME * 2.f)
	{
		isAttacked = true;
		if (looking)
			DamageSystem::NormalAttack(Collision::RectAngle{ {50.f, 70.f}, 0.f, {position + vector<2>{15.f, 30.f}} });
		else
			DamageSystem::NormalAttack(Collision::RectAngle{ {50.f, 70.f}, 0.f, {position + vector<2>{-15.f, 30.f}} });
	}
}
void Player::AttackUp()
{
	if (changedState + DELTA >= MOTION_FRAME * 3.f)
	{
		ChangeState(PlayerState::FALL);
		return;
	}
}
void Player::AttackDown()
{
	if (changedState + DELTA >= MOTION_FRAME * 2.f)
	{
		ChangeState(PlayerState::LIEDOWN);
		return;
	}
}
void Player::Render()
{
	Rendering::Image::Component image;

		
	//changedState -= stateImageSize[static_cast<int>(state)] * MOTION_FRAME;
	image.Content = "Player/base/avatar_"+ GetStateName(state)+"("+std::to_string(static_cast<UINT>(changedState/MOTION_FRAME))+")_default(0)";
	image.Length = image.GetLength();
	image.Location = { position[0], position[1] + (image.Length[1] / 2.f) };
	image.Draw(looking);
}

vector<2> Player::GetPosition()
{
	return position;
}


void Player::ChangeState(PlayerState cState)
{
	changedState = 0.f;
	state = cState;
}

void Player::FindPortal()
{
	if (!Input::Get::Key::OnceDown(VK_UP))
		return;

	size_t lenIdc = CURRENT_MAP->portals.size();
	for (size_t i = 0; i < lenIdc; i++)
	{
		if (length(PLAYER.position - CURRENT_MAP->portals[i].from) < 30.f)
		{
			WORLDI.Warp(i);
			return;
		}
	}
}

void Player::LookingFoot()
{
}

bool Player::LookingRope()
{
	int findRope = FindRope();

	if (findRope != -1)
	{
		keepingRope = findRope;
		position[0] = CURRENT_MAP->GetRope(keepingRope).p1[0];
		movingForce = { 0.f, 0.f };
		ChangeState(PlayerState::ROPE);
		return true;
	}
	return false;
}

int Player::FindFoot()
{
	size_t lenIdc = CURRENT_MAP->fIndicies.size();
	for (size_t i = 0; i < lenIdc; i++)
	{
		vector<2> hit = { position[0], position[1] + 5.f };

		if (Util::GetCrossPoint(hit, position,
			CURRENT_MAP->GetFoot(i).p1, CURRENT_MAP->GetFoot(i).p2)) {
			printf("position %f %f\r\n", position[0], position[1]);
			printf("DELTA %f\r\n", DELTA);
			printf("FindFoot %f %f, %f %f > %f %f, %f %f\r\n", hit[0], hit[1], position[0], position[1] + (movingSpeed * DELTA * 2.f), CURRENT_MAP->GetFoot(i).p1[0], CURRENT_MAP->GetFoot(i).p1[1], CURRENT_MAP->GetFoot(i).p2[0], CURRENT_MAP->GetFoot(i).p2[1]);
			return i;
		}
	}
	printf(" - position %f %f\r\n", position[0], position[1]);
	return -1;
}

int Player::FindRope()
{
	size_t lenIdc = CURRENT_MAP->rIndicies.size();
	for (size_t i = 0; i < lenIdc; i++)
	{
		vector<2> hit = { position[0] - (movingSpeed * DELTA * 100.f), position[1] };

		if (Util::GetCrossPoint(hit, { position[0] + (movingSpeed * DELTA * 100.f), position[1] },
			CURRENT_MAP->GetRope(i).p1, CURRENT_MAP->GetRope(i).p2)) {

			return i;
		}
	}
	return -1;
}

std::string Player::GetStateName(PlayerState state)
{
	switch (state)
	{
	case PlayerState::IDLE:
		return "stand1";
	case PlayerState::WALK:
		return "walk1";
	case PlayerState::JUMP:
	case PlayerState::FALL:
		return "jump";
	case PlayerState::LIEDOWN:
		return "prone";
	case PlayerState::ROPE:
		return "rope";
	case PlayerState::ATTACK_STAND:
	case PlayerState::ATTACK_UP:
		return "swingO1";
	case PlayerState::ATTACK_DOWN:
		return "proneStab";
	case PlayerState::ALERT:
		return "alert";
	}
	return "";
}
std::string Player::GetStateStr(PlayerState state)
{
	switch (state)
	{
	case PlayerState::IDLE:
		return "IDLE";
	case PlayerState::WALK:
		return "WALK";
	case PlayerState::JUMP:
		return "JUMP";
	case PlayerState::FALL:
		return "FALL";
	case PlayerState::LIEDOWN:
		return "LIEDOWN";
	case PlayerState::ROPE:
		return "ROPE";
	case PlayerState::ATTACK_STAND:
		return "ATTACK_STAND";
	case PlayerState::ATTACK_UP:
		return "ATTACK_UP";
	case PlayerState::ATTACK_DOWN:
		return "ATTACK_DOWN";
	case PlayerState::ALERT:
		return "ALERT";
	}
	return "";
}
void Player::CheckMapArea()
{
	if (position[0] > CURRENT_MAP->mapSize[0] / 2.f)
		position[0] = CURRENT_MAP->mapSize[0] / 2.f;
	else if (position[0] < -CURRENT_MAP->mapSize[0] / 2.f)
		position[0] = -CURRENT_MAP->mapSize[0] / 2.f;

	if (position[1] > CURRENT_MAP->mapSize[1] / 2.f)
		position[1] = CURRENT_MAP->mapSize[1] / 2.f;
	else if (position[1] < -CURRENT_MAP->mapSize[1] / 2.f)
		position[1] = -CURRENT_MAP->mapSize[1] / 2.f;
}

void Player::CheckCollision()
{
	if (invincibleTime > ELAPSED)
		return;

	auto iter = CURRENT_MAP->activeMonsters.begin();
	while (iter != CURRENT_MAP->activeMonsters.end())
	{
		if (Collision::Collide({
			MOBDATA.GetData((*iter)->mobId)->hitbox.Length,
			MOBDATA.GetData((*iter)->mobId)->hitbox.Angle,
			(*iter)->position + MOBDATA.GetData((*iter)->mobId)->hitbox.Location }, {
				hitBox.Length,
				hitBox.Angle,
				hitBox.Location + position
			}))
		{
			invincibleTime = ELAPSED + 2.f;
			int damage = Util::Rand(30, 50);
			DamageSystem::AddDamageText({ PLAYER.GetPosition()[0], PLAYER.GetPosition()[1] + 80.f }, damage, false);
			hp -= damage;
			break;
		}
		iter++;
	}
	return;
}

