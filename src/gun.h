#pragma once
#define effect_h

#include "bullet.h"

namespace Shmup
{
	using BulletArray = std::vector<Bullet>;
	// collection of bullets
	class Gun : public Entity
	{
	public:
		// prop for bullet, spawn_point(only position will be used) the bullet prop rect size will reman
		Gun(const EntityProp& p_bulletprop, const FRect* p_spawnpoint = nullptr);
		virtual ~Gun() override;

		void SetSpawnPoint(const FRect* p_spawnpoint) noexcept { spawnpoint = p_spawnpoint; }
		void SetTargets(ActorArray* p_targets) noexcept { targets = p_targets; }
		void SetCapacity(int p_cap) noexcept { capacity = p_cap; }


		virtual void Fire(Vecf2 p_vel, int p_dmg);
		virtual void Update(float p_dt) override;
		virtual void Render() override;

		
	protected:
		const FRect* spawnpoint; // where bullets come from; complemented and directed by velocity
		ActorArray* targets;
		BulletArray ammo;
		int capacity;

	};

	Gun::Gun(const EntityProp& p_bulletprop, const FRect* p_spawnpoint) : 
		Entity(p_bulletprop),
		spawnpoint(p_spawnpoint),
		capacity(1)
	{
		logger("Initializing <Gun><", id, ">...");

	}
	Gun::~Gun()
	{
		logger("Destructing <Gun><", id, ">...");
		targets = nullptr;
		spawnpoint = nullptr;

	}

	void Gun::Fire(Vecf2 p_vel, int p_dmg)
	{
		EntityProp bulletprop = *this;
		bulletprop.rect.x = spawnpoint->x;
		bulletprop.rect.y = spawnpoint->y;


		logger("bullet preparing...");

		ammo.emplace_back(bulletprop, targets, p_vel, p_dmg);
		logger("Bullet fired");


		if (ammo.size() > capacity)
		{
			logger("too many unhandled bullets left!");
		}

		
	}

	void Gun::Update(float p_dt)
	{
		// auto bullet might access invalid memory
		if (ammo.size() > 0)
		{
			for(auto bullet = ammo.begin(); bullet != ammo.end(); bullet++)
			{
				//logger("BULLET UPDATING");

				bullet->Update(p_dt);
				//logger("BULLET IS ALIVE");
				if ( ! bullet->IsAlive())
				{					
					ammo.erase(bullet);
				}
			}	
		}


	}

	void Gun::Render()
	{
		for(auto& bullet : ammo)
		{
			bullet.Render();
		}

	}

}

