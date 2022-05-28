#pragma once
#define bullet_h

#include "actor.h"
#include "../lib/include/collisions.h"


namespace Shmup
{
	// health translates to total bullets. Once it reached 0, 'is_alive' is false
	class Bullets : public Actor
	{
	public:
		Bullets(const EntityProp& p_prop, ActorArray& p_targets);
		~Bullets();

		virtual void Fire(FRect p_start, Vec2 p_vel, int p_dmg);
		virtual void Update(float p_dt) override;
		virtual void Render() override;


	protected:
		ActorArray* targets;
		bool is_fired;
		int dmg;
		
	};
	Bullets::Bullets(const EntityProp& p_prop, ActorArray& p_targets) : 
		Actor(p_prop), 
		targets(&p_targets)
	{
		logger("Initializing <bullets><", id, ">...");
		is_fired = false;
	}
	Bullets::~Bullets()
	{
		logger("Initializing <Bullets><", id, ">...");
		targets = nullptr;
	}

	void Bullets::Fire(FRect p_start, Vec2 p_vel, int p_dmg)
	{
		is_fired = true;
		vel = p_vel;
		dmg = p_dmg;
		rect = p_start;

		logger("Bullet fired");
	}


	// preferably called  earlier than other actors so they can adjust to the update made to them
	void Bullets::Update(float p_dt)
	{
		if (is_fired)
		{
			Actor::Update(p_dt);
			for (auto target : *targets)
			{
				if (Collided_Rect(collrect, target.GetCollRect()))
				{
					is_fired = false;
					health--; // bullet
					target.SetHealth(target.GetHealth() - dmg);

					if (health == 0)
					{
						//logger("Bullet died");
						is_alive = false;
						return;
					}

					logger("Bullet hit");
				}
			}
		}
		
	}
	void Bullets::Render()
	{
		if (is_fired)
		{
			Actor::Render();
		}
	}

}