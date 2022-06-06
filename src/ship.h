#pragma once
#define ship_h

#include "actor.h"
#include "gun.h"

namespace Shmup
{
	// pass a big fat toy gun to this baby, he won't forget to clean it after so..! 
	class Ship : public Actor, public Move4
	{
	public:
		Ship(const VisualObject& p_obj, Gun* p_gun);
		virtual ~Ship() override;

		const Gun* Get_Gun() const noexcept { return gun; }
		Gun* Get_Gun() noexcept { return gun; }

		virtual void Move_Left(Uint32 p_vel) 	override { vel.x = -p_vel; }
		virtual void Move_Right(Uint32 p_vel) 	override { vel.x =  p_vel; }
		virtual void Move_Top(Uint32 p_vel) 	override { vel.y = -p_vel; }
		virtual void Move_Bottom(Uint32 p_vel) 	override { vel.y =  p_vel; }


		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		Gun* gun;
		
	};
	Ship::Ship(const VisualObject& p_obj, Gun* p_gun) : 
		Actor(p_obj),
		gun(p_gun)
	{
		//gun = new Bullets()
		logger("Initializing <Ship><", id, ">...");
	}
	Ship::~Ship()
	{
		logger("Destructing <Ship><", id, ">...");

		delete gun; // handsome, I've my own manager now
		gun = nullptr;
	}



	void Ship::Update(float p_dt)
	{
		gun->Update(p_dt);
		Actor::Update(p_dt);

	}
	void Ship::Render()
	{
		gun->Render();
		Actor::Render();
	}
}