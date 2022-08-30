#pragma once


namespace MONSTER {
	enum class MonsterState {
		IDLE = 0,
		WALK = 1,
		JUMP = 2,
		FALL = 3,
		CHASE = 4,
		ATTACK = 5,
		DIE = 6,
		HIT = 7
	};

	class Monster
	{
	private:
		float			leftDuration;
		float			stateTime;
		bool			looking;
		int				steppingFoot;
		MonsterState	state;
		short			hp;

	public:
		int				mobId;
		vector<2>		position;
		bool			isAlive;
		short			spawnIndex;



	public:
		void Start(int mobId);
		void Update();
		void LateUpdate();
		void End();

		void Idle();
		void Walk();
		void Jump();
		void Fall();
		void Chase();
		void Attack();
		void Hit();
		void Die();

		void Render();


		void ChangeState(MonsterState state);
		void HitByPlayer(short damage);
		bool LookingForTarget();
		int FindFoot();
		std::string GetStateName(MonsterState state);
		std::string GetStateStr(MonsterState state);
	};
}

