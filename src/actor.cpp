#include "actor.h"


namespace Shmup
{
	Actor::Actor(const VisualObject& p_obj) :
		VisualObject(p_obj)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		health = 0;
		collrect = rect;
		
		vel = FVec2{0.0f, 0.0f};
		move_timer.Peek();
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
	}

	void Actor::Update(float p_dt)
	{
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;

		Rect lvalue = rect;
		// escape screen
		if (!SDL_HasIntersection(&lvalue, screen))
		{
			logger("<Actor><", id, "> escaped from screen! x: ", rect.x, ", y: ", rect.y);
			is_alive = false;
		}

	}
}