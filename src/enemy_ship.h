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
		
	};
	EnemyShip::EnemyShip(const EntityProp& p_prop, Gun* p_gun) : 
		Ship(p_prop, p_gun)
	{
		//gun = new Bullets()
		logger("Initializing <EnemyShip><", id, ">...");

	}
	EnemyShip::~EnemyShip()
	{
		logger("Destructing <EnemyShip><", id, ">...");
	}



	void EnemyShip::Update(float p_dt)
	{
		Ship::Update(p_dt);

		if (rect.Right() > vel.x*2)
		{
			vel.x *= -1;
		}
		else if (rect.Left() < 10)
		{
			vel.x *= -1;
		}
	}
	void EnemyShip::Render()
	{
		Ship::Render();
	}
}