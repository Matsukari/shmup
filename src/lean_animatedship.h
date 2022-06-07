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

		void DoneMoving() noexcept { 
			logger("Going BACK TO IDLE"); 
			is_donemoving = true;
			
		}

		void Move_Left(int p_vel) override;
		void Move_Right(int p_vel) override;
		void Move_Top(int p_vel) override;
		void Move_Bottom(int p_vel) override;

		
		void Update(float p_dt) override;

	protected:
		bool is_donemoving;
		bool is_moving;

		
	};
	LeanAnimatedShip::LeanAnimatedShip(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap) : 
		AnimatedShip(p_obj, p_gun, p_act_framemap)
	{
		//gun = new Gun()
		logger("Initializing <LeanAnimatedShip><", id, ">...");

		is_moving = false;
		is_donemoving = false;

	}
	LeanAnimatedShip::~LeanAnimatedShip()
	{
		logger("Destructing <LeanAnimatedShip><", id, ">...");
		
	}


	void LeanAnimatedShip::Move_Left(int p_vel)
	{
		AnimatedShip::Move_Left(p_vel);
		if (!is_moving)
		{
			Set_Act("move_left");
			is_moving = true;

		}

	}
	void LeanAnimatedShip::Move_Right(int p_vel)
	{
		AnimatedShip::Move_Right(p_vel);
		if (!is_moving)
		{
			Set_Act("move_right");
			is_moving = true;
		}
	}
	void LeanAnimatedShip::Move_Top(int p_vel)
	{
		AnimatedShip::Move_Top(p_vel);
		//is_moving = true;
	}
	void LeanAnimatedShip::Move_Bottom(int p_vel)
	{
		AnimatedShip::Move_Bottom(p_vel);
		//is_moving = true;
	}

	void LeanAnimatedShip::Update(float p_dt)
	{
		//AnimatedShip::Update(p_dt);
		Ship::Update(p_dt);

		auto& frames = act_framemap->at(curr_act);

		
		auto Loop = [&]()
		{
			//logger("IDLE LOOPING");
			frames.curr++;
			if (frames.curr >= frames.size())
			{
				frames.curr = 0;
			}
		};
		auto SuspendEnd = [&]()
		{
			//logger("SUSPENDING");
			frames.curr++;
			if (frames.curr >= frames.size())
			{
				frames.curr = frames.size() - 2;
			}
		};	
		auto Reverse = [&]()
		{
			//logger("REVERSE_____");
			frames.curr--;
			if (frames.curr < 0)
			{
				frames.curr = 0;
				is_donemoving = false;
				is_moving = false;
				Set_Act("idle");
			}
		};	


		if (curr_act == "idle")
		{
			frames.On_NextFrame(Loop);
		}
		else if (is_donemoving) // priotize before moving
		{
			frames.On_NextFrame(Reverse);
		}
		else if (is_moving)
		{
			frames.On_NextFrame(SuspendEnd);
		}





		
	}

}