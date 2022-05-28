#pragma once
#define ship_h

#include "actor.h"
#include "bullet.h"

namespace Shmup
{

	class Ship : public Actor
	{
	public:
		Ship(const EntityProp& p_prop);
		~Ship();

		virtual void HandleEvents(const SDL_Event& p_event) override;
		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		Bullets* bullets;

		
	};
	Ship::Ship(const EntityProp& p_prop) : Actor(p_prop)
	{
		//bullets = new Bullets()
		logger("Initializing <Ship><", id, ">...");
	}
	Ship::~Ship()
	{
		logger("Destructing <Ship><", id, ">...");

		delete bullets;
		bullets = nullptr;
	}




	void Ship::HandleEvents(const SDL_Event& p_event)
	{
		if (p_event.type == SDL_KEYDOWN)
		{
			switch(p_event.key.keysym.sym)
			{
				case SDLK_j: bullets->Fire(FRect{
					.x = rect.RectCenterX(),
					.y = rect.Top(),
					.w = 10,
					.h = 10
					},
					Vec2{0, -250}, 10
				);
				break;
				case SDLK_w: vel.y = -100; logger("<Ship><", id, "> moved up"); break;
				case SDLK_a: vel.x = -100; logger("<Ship><", id, "> moved left"); break;
				case SDLK_s: vel.y = 100; logger("<Ship><", id, "> moved down"); break;
				case SDLK_d: vel.x = 100; logger("<Ship><", id, "> moved right"); break;
			}
		}
	}
	void Ship::Update(float p_dt)
	{
		bullets->Update(p_dt);
		Actor::Update(p_dt);

	}
	void Ship::Render()
	{
		bullets->Render();
		Actor::Render();
	}
}