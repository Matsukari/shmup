#include "bullet.h"

namespace Shmup
{
	Bullet::Bullet(const VisualObject& p_obj, ActorArray* p_targets, const RectArray* p_frames, FVec2 p_vel, int p_dmg) : 
		Actor(p_obj), 
		frames(p_frames),
		targets(p_targets),
		dmg(p_dmg)
	{
		logger("Initializing <bullet><", id, ">...");
		vel = p_vel;
		is_alive = true;
		curr_frame = 0;

		if (!targets)
		{
			logger("warn <bullet><", id, "> no targets set!");
		}
		if (!frames)
		{
			logger("warn <bullet><", id, "> no frames set!");
		}


	}
	Bullet::~Bullet()
	{
		logger("Destructing <Bullet><", id, ">...");
		frames = nullptr;
		targets = nullptr;
	}


	// preferably called  earlier than other actors so they can adjust to the update made to them,
	// this object is stuck at them so
	void Bullet::Update(float p_dt)
	{
		if (is_alive)
		{
			//logger("bullet ACTOR UPDATING...");
			Actor::Update(p_dt);
			curr_frame = (curr_frame + 1) % frames->size();

			//logger("bullet rect.y > ........");

			if (targets)
			{
				//logger("Target : *targets");
				for (auto target : *targets)
				{
					//logger("checking target collission...");
					// need lvalue for SDL_HasIntersection
					Rect a(collrect);
					Rect b(target->Get_CollRect());
					if (SDL_HasIntersection(&a, &b))
					{
						is_alive = false;
						target->Set_Health(target->Get_Health() - dmg);

						logger("<Bullet><", id, " hit an Actor");	
					}
				}	
			}
			
		}
		
	}
	void Bullet::Render()
	{
		// dead men don't move!
		if (is_alive)
		{
			texture->Render(rect, &frames->at(curr_frame));
		}
	}
}