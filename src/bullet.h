#pragma once
#define bullet_h

#include "../lib/include/collisions.h"
#include "actor.h"


namespace Shmup
{
	// fired the moment it's initialized
	class Bullet : public Actor
	{
	public:
		Bullet(const VisualObject& p_obj, ActorArray* p_targets, const RectArray* p_frames, unsigned int p_fspeed, FVec2 p_vel, int p_dmg);
		virtual ~Bullet() override;

		void Set_Targets(ActorArray* p_targets) noexcept { targets = p_targets; }
		void Set_Frames(const RectArray& p_frames) noexcept { frames = &p_frames; }

		virtual void Update(float p_dt) override;
		virtual void Render() override;


	protected:
		const RectArray* frames;
		unsigned int curr_frame;
		ActorArray* targets;
		int dmg;

		unsigned int fspeed;
		Timer move_timer;
		
	};


	Bullet::Bullet(const VisualObject& p_obj, ActorArray* p_targets, const RectArray* p_frames, unsigned int p_fspeed, FVec2 p_vel, int p_dmg) : 
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

		move_timer.Peek();

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
			if (move_timer.SinceLastPeek() > fspeed)
			{
				move_timer.Peek();
				curr_frame = (curr_frame + 1) % frames->size();
			}

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