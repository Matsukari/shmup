#pragma once
#define actor_h

#include "../lib/include/timer.h"
#include "visual_object.h"
#include "framelist.h"
#include "rectman.h"
#include "animation.h"


namespace Shmup
{
	class Move4
	{
	public:
		virtual ~Move4() {}

		// pixels per second
		virtual void Move_Left(int p_vel) = 0;
		virtual void Move_Right(int p_vel) = 0;
		virtual void Move_Top(int p_vel) = 0;
		virtual void Move_Bottom(int p_vel) = 0;

	};

	// create a pointer to some kind of music? this shit owns a pretty good ammount of *** already, so...
	class Actor : public VisualObject
	{
	public:
		Actor(const VisualObject& p_obj);
		virtual ~Actor() override;

		// this reaction is only used to trigger activate(), output is left outside
		virtual void Set_DeathReaction(Reaction_ptr p_react) { dtreact = std::move(p_react); }

		int Get_MaxHealth() const noexcept { return maxhealth; }
		int Get_Health() const noexcept { return health; }

		FVec2 Get_Vel() const noexcept { return vel; }
		const FRect& Get_CollRect() const noexcept { return collrect; }

		void Set_MaxHealth(int p_health) noexcept { maxhealth = p_health; }
		void Set_Health(int p_health) { health = std::min(p_health, this->maxhealth); }

		void Set_Vel(FVec2 p_vel) noexcept { vel = p_vel; }
		void Set_CollRect(const FRect& p_collrect) noexcept { collrect = p_collrect; }

		bool Is_Alive() const noexcept { return is_alive; }
		virtual void Update(float p_dt) override;

		void Kill() noexcept { is_alive = false; }
		void DeathReact() noexcept { is_dtreact = true; }


	protected:
		bool is_alive;
		int maxhealth;
		int health;
		FRect collrect;

		FVec2 vel;

		Reaction_ptr dtreact;
		bool is_dtreact;

	};
	using ActorArray = std::vector<Actor*>;


	Actor::Actor(const VisualObject& p_obj) :
		VisualObject(p_obj)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		Set_MaxHealth(100);
		Set_Health(100);
		
		collrect = rect;
		
		vel = FVec2{0.0f, 0.0f};

		is_dtreact = false;
		//dtreact = nullptr;
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
		if (is_dtreact && dtreact)
		{
			dtreact->Activate();
			logger("________Activating Destuctor Reaction_____");
			//dtreact = nullptr;
		}
	}

	void Actor::Update(float p_dt)
	{
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;

		// escape screen
		Rect lvalue = rect;
		if (!SDL_HasIntersection(&lvalue, screen))
		{
			logger("<Actor><", id, "> escaped from screen! x: ", rect.x, ", y: ", rect.y);
			is_alive = false;
		}
	}

}