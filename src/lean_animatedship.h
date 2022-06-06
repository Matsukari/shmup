#pragma once
#define lean_LeanAnimatedShip_h

#include "animatedship.h"

namespace Shmup
{
	// A ship with actions
	class LeanAnimatedShip : public AnimatedShip
	{
	public:
		LeanAnimatedShip(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap);
		~LeanAnimatedShip() override;

		void DoneMoving() noexcept { done_moving = true; }

		void Move_Left(Uint32 p_vel) override;
		void Move_Right(Uint32 p_vel) override;
		
		void Update(float p_dt) override;

	protected:
		bool is_mvtrans; // move transition
		bool done_moving;

		
	};
	LeanAnimatedShip::LeanAnimatedShip(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap) : 
		AnimatedShip(p_obj, p_gun, p_act_framemap)
	{
		//gun = new Gun()
		logger("Initializing <LeanAnimatedShip><", id, ">...");

		is_mvtrans = false;
		done_moving = false;


	}
	LeanAnimatedShip::~LeanAnimatedShip()
	{
		logger("Destructing <LeanAnimatedShip><", id, ">...");
		
	}


	void LeanAnimatedShip::Move_Left(Uint32 p_vel)
	{
		Move_Left(p_vel);
		Set_Act("move_left");

	}
	void LeanAnimatedShip::Move_Right(Uint32 p_vel)
	{
		Move_Right(p_vel);
		Set_Act("move_right");
	}

	void LeanAnimatedShip::Update(float p_dt)
	{
		auto& frames = act_framemap->at(curr_act);

		bool is_actmove = curr_act == "move_right" || curr_act == "move_left";

		// go back to idle
		if (is_actmove && done_moving)
		{
			frames.Set_Jump(-1);
			is_mvtrans = true;

			done_moving = false;

		}
		// move frame is nearing the end frame yet there is no change in event move. 
		// Pertain boost animation that is within last 2
		else if (!is_mvtrans && is_actmove && frames.Idx() >= frames.size() - 2)
		{
			frames.Set_Start(frames.size() - 2);
		}

		AnimatedShip::Update(p_dt);

		if (is_mvtrans && frames.Idx() > 0)
		{
			Set_Act("idle");
			act_framemap->at(curr_act).Set_Jump(1);
		}
		
	}

}