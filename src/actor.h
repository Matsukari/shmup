#pragma once
#define actor_h

#include "entity.h"


namespace Shmup
{
	class Actor : public Entity
	{
	public:
		Actor(const EntityProp& p_prop);
		~Actor();

		int GetHealth() const noexcept { return health; }
		Vec2 GetVel() const noexcept { return vel; }
		FRect GetCollRect() const noexcept { return collrect; }

		void SetHealth(int p_health) noexcept { health = p_health; }
		void SetVel(Vec2 p_vel) noexcept { vel = p_vel; }
		void SetCollRect(FRect p_collrect) noexcept { collrect = p_collrect; }

		bool IsAlive() const noexcept { return is_alive; }

		virtual void Update(float p_dt) override;

	protected:
		bool is_alive;
		int health;
		FRect collrect;

		Vec2 vel;
	};
	using ActorArray = std::vector<Actor>;

	Actor::Actor(const EntityProp& p_prop) :
		Entity(p_prop)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		health = 0;
		vel = Vec2{0, 0};
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
	}

	void Actor::Update(float p_dt)
	{
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;

	}

}