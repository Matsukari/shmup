#pragma once
#define actor_h

#include "../lib/include/timer.h"
#include "visual_object.h"


namespace Shmup
{
	class Move4
	{
	public:
		virtual ~Move4() {}

		// pixels per second
		virtual void Move_Left(Uint32 p_vel) = 0;
		virtual void Move_Right(Uint32 p_vel) = 0;
		virtual void Move_Top(Uint32 p_vel) = 0;
		virtual void Move_Bottom(Uint32 p_vel) = 0;

	};
	class Actor : public VisualObject
	{
	public:
		Actor(const VisualObject& p_obj);
		virtual ~Actor() override;

		int Get_Health() const noexcept { return health; }
		FVec2 Get_Vel() const noexcept { return vel; }
		FRect Get_CollRect() const noexcept { return collrect; }

		void Set_Health(int p_health) noexcept { health = p_health; }
		void Set_Vel(FVec2 p_vel) noexcept { vel = p_vel; }
		void Set_CollRect(const FRect& p_collrect) noexcept { collrect = p_collrect; }

		bool Is_Alive() const noexcept { return is_alive; }
		virtual void Update(float p_dt) override;


	protected:
		bool is_alive;
		int health;
		FRect collrect;

		FVec2 vel;

	};
	using ActorArray = std::vector<Actor*>;


	Actor::Actor(const VisualObject& p_obj) :
		VisualObject(p_obj)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		health = 0;
		collrect = rect;
		
		vel = FVec2{0.0f, 0.0f};
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
		//std::cout << "~ACTOR" << std::endl;
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