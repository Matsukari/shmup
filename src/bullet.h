#pragma once
#define bullet_h

#include "actor.h"
#include "../lib/include/collisions.h"


namespace Shmup
{
	// health translates to total bullets. Once it reached 0, 'is_alive' is false
	// fired the moment it's initialized
	class Bullet : public Actor
	{
	public:
		Bullet(const EntityProp& p_prop, ActorArray* p_targets, Vecf2 p_vel, int p_dmg);
		virtual ~Bullet() override;

		virtual void Update(float p_dt) override;
		virtual void Render() override;


	protected:
		ActorArray* targets;
		int dmg;
		
	};
	

	Bullet::Bullet(const EntityProp& p_prop, ActorArray* p_targets, Vecf2 p_vel, int p_dmg) : 
		Actor(p_prop), 
		targets(p_targets),
		dmg(p_dmg)
	{
		logger("Initializing <bullet><", id, ">...");
		vel = p_vel;
		is_alive = true;

	}
	Bullet::~Bullet()
	{
		logger("Destructing <Bullet><", id, ">...");
		targets = nullptr;
	}


	// preferably called  earlier than other actors so they can adjust to the update made to them,
	// this object is stuck at them so
	void Bullet::Update(float p_dt)
	{
		if (is_alive)
		{
			Actor::Update(p_dt);

			if (rect.y > 570)
			{
				is_alive = false;
				logger("bullet bounds hit");
			}

			if (targets != nullptr)
			{
				for (auto target : *targets)
				{
					logger("checking target collission...");
					if (Collided_Rect(collrect, target->GetCollRect()))
					{
						is_alive = false;
						target->SetHealth(target->GetHealth() - dmg);

						logger("Bullet hit");	
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
			Actor::Render();
		}
	}




}