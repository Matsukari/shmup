#pragma once
#define enemy_ship_h

#include "actor.h"
#include "gun.h"

namespace Shmup
{
	// pass a big fat toy gun to this baby, he won't forget to clean it after so..! 
	class EnemyShip : public Ship
	{
	public:
		EnemyShip(const EntityProp& p_prop, Gun* p_gun);
		virtual ~EnemyShip() override;

		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		Timer attack_timer;
		Vecf2 true_vel;
	};
	EnemyShip::EnemyShip(const EntityProp& p_prop, Gun* p_gun) : 
		Ship(p_prop, p_gun)
	{
		//gun = new Bullets()
		logger("Initializing <EnemyShip><", id, ">...");
		attack_timer.Peek();
	}
	EnemyShip::~EnemyShip()
	{
		logger("Destructing <EnemyShip><", id, ">...");
	}



	void EnemyShip::Update(float p_dt)
	{
		
		gun->Update(p_dt);
		if (move_timer.SinceLastPeek() >= fspeed)
		{
			move_timer.Peek();
			curframe++;
			if (curframe >= frames.size())
			{
				curframe = 0;
			}
		}


		if (attack_timer.SinceLastPeek() >= 400)
		{
			attack_timer.Peek();
			gun->Fire(Vecf2{0, 250}, 10);
		}


		true_vel = vel;

		if (rect.Right() > abs(vel.x*2))
		{
			true_vel.x = -vel.x;
		}
		else if (rect.Left() < 10)
		{
			true_vel.x = vel.x;
		}

		rect.x += true_vel.x * p_dt;
		rect.y += true_vel.y * p_dt;
	}
	void EnemyShip::Render()
	{
		Ship::Render();
	}
}