#pragma once

enum class PlayerState {
	IDLE = 0,
	WALK = 1,
	JUMP = 2,
	FALL = 3,
	LIEDOWN = 4,
	ROPE = 5,
	ATTACK_STAND = 6,
	ATTACK_DOWN = 7,
	ATTACK_UP = 8,
	ALERT = 9
};


class Player : public Singleton<Player>
{
private:
	vector<2>	movingForce;
	float		jumpingForce;
	float		movingSpeed;
	float		jumpingSpeed;
	float		jumpingTime;
	float		changedState;
	float		invincibleTime;

	int			steppingFoot;
	int			keepingRope;

	short		stateImageSize[10];
	PlayerState state = PlayerState::IDLE;
	bool		looking;
	bool		isRopping;
	bool		isAttacked;

	Collision::RectAngle	hitBox;


public:
	vector<2>	position;
	int			job = 1400, level = 1;
	int			hp = 3000, maxHp = 3000, mp = 2000, maxMp = 2000;
	int			needExp[100] = { 15, 49, 106, 198, 333, 705, 1265, 2105, 3347, 4589, 5831, 7073, 8315, 9557, 11047, 12835, 14980, 17554, 20642, 24347, 28793, 34128, 40530, 48212, 57430 };
	int			exp = 0;

public:

	void Start();
	void Update();
	void End();


	void Idle();
	void Walk();
	void Jump();
	void Fall();
	void LieDown();
	void Rope();
	void AttackStand();
	void AttackUp();
	void AttackDown();
	void Alert();

	void Render();

	vector<2> GetPosition();

	void ChangeState(PlayerState cState);
	void FindPortal();
	void LookingFoot();
	bool LookingRope();
	int FindFoot();
	int FindRope();
	std::string GetStateName(PlayerState state);
	std::string GetStateStr(PlayerState state);
	void CheckMapArea();
	void CheckCollision();
};



