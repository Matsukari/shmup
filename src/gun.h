#pragma once
#define gun_h

#include "bullet.h"
#include <list>

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

		void Set_SpawnPoint(const FRect* p_spawnp) noexcept { spawnp = p_spawnp; }
		void Set_Ammo(int p_ammo) noexcept { ammo = p_ammo; }
		void Set_Targets(ActorArray* p_targets) noexcept;


		virtual void Fire(FVec2 p_vel, int p_dmg);
		virtual void Update(float p_dt);
		virtual void Render();

		
	protected:
		const FRect* spawnp; // where bullets come from; complemented and directed by velocity
		const RectArray* frames; // bullet frame src

		ActorArray* targets;
		BulletList bullets;

		unsigned int fspeed;

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
	}

	void Gun::Set_Targets(ActorArray* p_targets) noexcept
	{
		for(auto& bullet : bullets)
		{
			bullet->Set_Targets(p_targets);
		}
	}


	void Gun::Fire(FVec2 p_vel, int p_dmg)
	{
		if (!ammo)
		{
			logger("No ammo left!");
			return;
		}

		//logger("bullet preparing...");

		VisualObject bullet_prop = *this;

		FRect newrect = bullet_prop.Get_Rect();
		newrect.x = spawnp->x;
		newrect.y = spawnp->y;

		bullet_prop.Set_Rect(newrect);


		bullets.push_back(new Bullet(bullet_prop, targets, frames, fspeed, p_vel, p_dmg));
		--ammo;

		logger("Bullet fired, ammo left: ", ammo);

	}

	void Gun::Update(float p_dt)
	{
		// auto bullet might access invalid memory
		if (!bullets.empty())
		{
			//logger("bullet ammo.begin()...");

			for(auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
			{
				//logger("bullet UpDATING...");

				(*bullet)->Update(p_dt);

				//logger("bullet ISALIVE...");

				if ( ! (*bullet)->Is_Alive())
				{
					logger("\nBULLET ERASING");					
					bullets.erase(bullet);
					bullet--; // bullet will point to the same index
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

