#pragma once
#define enemy_ship_h

#include "animatedship.h"

namespace Shmup
{
	// pass a big fat toy gun to this baby, he won't forget to clean it after so..! 
	class ShipEnemyBoss : public AnimatedShip
	{
	public:
		ShipEnemyBoss(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap);
		virtual ~ShipEnemyBoss() override;

		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		Timer attack_timer;
	};
	ShipEnemyBoss::ShipEnemyBoss(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap) : 
		AnimatedShip(p_prop, p_gun, p_act_framemap)
	{
		//gun = new Bullets()
		logger("Initializing <ShipEnemyBoss><", id, ">...");
		attack_timer.Peek();
	}
	ShipEnemyBoss::~ShipEnemyBoss()
	{
		logger("Destructing <ShipEnemyBoss><", id, ">...");
	}



	void ShipEnemyBoss::Update(float p_dt)
	{
		if (attack_timer.SinceLastPeek() >= 400)
		{
			attack_timer.Peek();
			gun->Fire(Vecf2{0, 250}, 10);
			logger("ENEMY FIRED");
		}


	}
	void ShipEnemyBoss::Render()
	{
		Ship::Render();
	}
}