#pragma once
#define ship_h

#include "actor.h"
#include "gun.h"

namespace Shmup
{
	// pass gun to this baby, he won't make short of your precious pearls...He'll clean them after so..! 
	class Ship : public Actor
	{
	public:
		Ship(const EntityProp& p_prop, Gun* p_gun);
		virtual ~Ship() override;

		virtual void HandleEvents(const SDL_Event& p_event) override;
		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		Gun* gun;
		
	};
	Ship::Ship(const EntityProp& p_prop, Gun* p_gun) : 
		Actor(p_prop),
		gun(p_gun)
	{
		//gun = new Bullets()
		logger("Initializing <Ship><", id, ">...");
	}
	Ship::~Ship()
	{
		logger("Destructing <Ship><", id, ">...");

		delete gun; // handsome, I've my own manager now
		gun = nullptr;
	}




	void Ship::HandleEvents(const SDL_Event& p_event)
	{
		if (p_event.type == SDL_KEYDOWN)
		{
			switch(p_event.key.keysym.sym)
			{
				// fly my boy! soar high! (TOP of ship)
				case SDLK_j: gun->Fire(Vecf2{0, -250}, 10); break;
				case SDLK_w: vel.y = -100; logger("<Ship><", id, "> moved up"); break;
				case SDLK_a: vel.x = -100; logger("<Ship><", id, "> moved left"); break;
				case SDLK_s: vel.y = 100; logger("<Ship><", id, "> moved down"); break;
				case SDLK_d: vel.x = 100; logger("<Ship><", id, "> moved right"); break;
			}
		}
	}
	void Ship::Update(float p_dt)
	{
		gun->Update(p_dt);
		Actor::Update(p_dt);

	}
	void Ship::Render()
	{
		gun->Render();
		Actor::Render();
	}
}