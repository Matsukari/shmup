#pragma once
#define gun_h

#include "bullet.h"
#include <list>
#include <functional>




namespace Shmup
{

	using BulletList = std::list<Bullet*>;
	// collection of bullets
	class Gun : public VisualObject // this visualobject corresponds to each bullet
	{
	public:
		// prop for bullet, spawn_point(only position will be used) the bullet prop rect size will reman
		Gun(const VisualObject& p_bullet, int p_ammo, const FRect* p_spawnp, ActorArray* p_targets, const RectArray* p_frames, unsigned int p_fspeed);
		virtual ~Gun();

		int Get_Ammo() const noexcept { return ammo; }

		void Set_SpawnPoint(const FRect* p_spawnp, FRect_Manip p_man) noexcept { 
			spawnp = p_spawnp; 
			spawnp_man = p_man; // mannipulator
		}
		void Set_RechargeTime(Uint32 p_ms) noexcept { recharget = p_ms; }
		void Set_Ammo(int p_ammo) noexcept { ammo = p_ammo; }
		void Set_Targets(ActorArray* p_targets) noexcept;

		BulletList& Get_Bullets() noexcept { return bullets; }

		bool Is_Recharged() noexcept;

		virtual void Fire(FVec2 p_vel, int p_dmg, Reaction_ptr deathreact);
		virtual void Update(float p_dt);
		virtual void Render();

		
	protected:
		const FRect* spawnp; // where bullets come from; complemented and directed by velocity

		FRect_Manip spawnp_man; // spawnp manipulator
		const RectArray* frames; // bullet frame src

		ActorArray* targets;
		BulletList bullets;

		unsigned int fspeed;
		Timer shot_timer;
		Uint32 recharget;

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
		spawnp_man = RectMan::CenterToCenter;
		recharget = 300;
		shot_timer.Peek();

		//dtreact = nullptr;

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
		spawnp_man =nullptr;
		frames = nullptr;
		targets = nullptr;
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

	bool Gun::Is_Recharged() noexcept 
	{
		// avoid spam in event poll
		if (shot_timer.SinceLastPeek() <= recharget)
		{
			logger("Waiting to recharge, ", recharget - shot_timer.SinceLastPeek());
			return false;
		}
		shot_timer.Peek();
		return true;
	}

	// call is_rechanrged()
	void Gun::Fire(FVec2 p_vel, int p_dmg, Reaction_ptr deathreact)
	{
		if (!ammo)
		{
			logger("No ammo left!");
			return;
		}

		logger("bullet preparing...");


		VisualObject bullet_prop = *this;

		FRect newrect = bullet_prop.Get_Rect();
		newrect = spawnp_man(newrect, *spawnp);

		bullet_prop.Set_Rect(newrect);

		logger("bullet creating bullet...");
		bullets.push_back(new Bullet(bullet_prop, targets, frames, fspeed, p_vel, p_dmg));

		logger("bullet setting reaction...");
		bullets.back()->Set_DeathReaction(std::move(deathreact));
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
		for(auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
		{
			(*bullet)->Render();
		}

	}

}

