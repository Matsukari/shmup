#pragma once
#define gun_h

#include "bullet.h"
#include <list>
#include <functional>




namespace Shmup
{
	FRect RectItsCenter(const FRect& base, const FRect& p_rect)
	{
		FRect rect = base;
		rect.Set_CenterToCenter(p_rect);
		return std::move(rect);
	}
	using BulletList = std::list<Bullet*>;
	// collection of bullets
	class Gun : public VisualObject // this visualobject corresponds to each bullet
	{
	public:
		// prop for bullet, spawn_point(only position will be used) the bullet prop rect size will reman
		Gun(const VisualObject& p_bullet, int p_ammo, const FRect* p_spawnp, ActorArray* p_targets, const RectArray* p_frames, unsigned int p_fspeed);
		virtual ~Gun();

		int Get_Ammo() const noexcept { return ammo; }

		void Set_SpawnPoint(const FRect* p_spawnp, std::function<FRect (const FRect&, const FRect&)> p_man) noexcept { 
			spawnp = p_spawnp; 
			spawnp_man = p_man; // mannipulator
		}
		void Set_RechargeTime(Uint32 p_ms) noexcept { recharget = p_ms; }
		void Set_Ammo(int p_ammo) noexcept { ammo = p_ammo; }
		void Set_Targets(ActorArray* p_targets) noexcept;
		void Set_DeathReaction(Reaction* p_react) noexcept { deathreaction = p_react; }

		BulletList& Get_Bullets() noexcept { return bullets; }


		virtual void Fire(FVec2 p_vel, int p_dmg);
		virtual void Update(float p_dt);
		virtual void Render();

		
	protected:
		const FRect* spawnp; // where bullets come from; complemented and directed by velocity
		std::function<FRect (const FRect&, const FRect&)> spawnp_man;
		const RectArray* frames; // bullet frame src

		ActorArray* targets;
		BulletList bullets;

		unsigned int fspeed;
		Timer shot_timer;
		Uint32 recharget;

		Reaction* deathreaction;

		int ammo;

	};


	Gun::Gun(const VisualObject& p_bullet, int p_ammo, const FRect* p_spawnp, ActorArray* p_targets, const RectArray* p_frames, unsigned int p_fspeed) : 
		VisualObject(p_bullet),
		ammo(p_ammo),
		spawnp(p_spawnp),
		targets(p_targets),
		frames(p_frames),
		fspeed(p_fspeed)
	{
		logger("Initializing <Gun><", id, ">...");
		spawnp_man = RectItsCenter;
		recharget = 300;
		shot_timer.Peek();

		deathreaction = nullptr;

	}
	Gun::~Gun()
	{
		logger("Destructing <Gun><", id, ">...");
		for(auto& bullet : bullets)
		{
			delete bullet;
			bullet = nullptr;
		}
		spawnp = nullptr;
		frames = nullptr;
		targets = nullptr;
		deathreaction = nullptr;
	}

	void Gun::Set_Targets(ActorArray* p_targets) noexcept
	{
		targets = p_targets;
		if (!targets)
		{
			return;
		}
		logger("\nSetting actor targets...\n");
		for(auto& bullet : bullets)
		{
			bullet->Set_Targets(p_targets);
		}
	}


	void Gun::Fire(FVec2 p_vel, int p_dmg)
	{
				// avoid spam in event poll
		if (shot_timer.SinceLastPeek() <= recharget)
		{
			logger("Waiting to recharge, ", recharget - shot_timer.SinceLastPeek());
			return;
		}
		shot_timer.Peek();

		if (!ammo)
		{
			logger("No ammo left!");
			return;
		}

		//logger("bullet preparing...");


		VisualObject bullet_prop = *this;

		FRect newrect = bullet_prop.Get_Rect();
		newrect = spawnp_man(newrect, *spawnp);

		bullet_prop.Set_Rect(newrect);



		bullets.push_back(new Bullet(bullet_prop, targets, frames, fspeed, p_vel, p_dmg));
		bullets.back()->Set_DeathReaction(deathreaction);
		--ammo;

		logger("Bullet fired, ammo left: ", ammo);

	}

	void Gun::Update(float p_dt)
	{
		// auto bullet might access invalid memory
		if (!bullets.empty())
		{
			//logger("bullet ammo.begin()...");

			for(auto bullet = bullets.begin(); bullet != bullets.end(); )
			{
				//logger("bullet UpDATING...");

				(*bullet)->Update(p_dt);

				//logger("bullet ISALIVE...");

				if ( ! (*bullet)->Is_Alive())
				{
					logger("\nBULLET ERASING");
					delete *bullet;					
					bullet = bullets.erase(bullet);

					 // THIS WORKED BEFORE ATTACHING THIS TO SHIP
					//bullet--; // bullet will point to the same index			
				}
				else
				{
					bullet++;
				}
			}	
		}


	}

	void Gun::Render()
	{
		for(auto bullet : bullets)
		{
			bullet->Render();
		}

	}

}

