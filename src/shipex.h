#pragma once
#define shipex_h

#include "ship.h"

namespace Shmup
{
	using str = std::string;
	template<class Key, class T> using map = std::map<Key, T>;
	template<class T> using vec = std::vector<T>;
	using FrameMap = map<str, vec<vec<int>>>;
	// A ship with actions
	class ShipEx : public ShipEx
	{
	public:
		ShipEx(const EntityProp& p_prop, const FrameMap& p_framemap);
		~ShipEx();


		virtual void HandleEvents(const SDL_Event& p_event) override;
		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		// frames will be for action 'idle'
    	const FrameMap* framemap;
    	// keyed actions, containing: [frame][<-sub-frames][<-frame] RIDICULOUS
		
		
	};
	ShipEx::ShipEx(const EntityProp& p_prop, const FrameMap& p_framemap) : Actor(p_prop, framemap(p_framemap))
	{
		//bullets = new Bullets()
		logger("Initializing <ShipEx><", id, ">...");
	}
	ShipEx::~ShipEx()
	{
		logger("Destructing <ShipEx><", id, ">...");
		frames = nullptr;

		delete bullets;
		bullets = nullptr;
	}


	void ShipEx::HandleEvents(const SDL_Event& p_event)
	{
		if (p_event.type == SDL_KEYDOWN)
		{
			switch(p_event.key.keysym.sym)
			{
				case SDLK_w: vel.y = -100; logger("<ShipEx><", id, "> moved up"); break;
				case SDLK_a: vel.x = -100; logger("<ShipEx><", id, "> moved left"); break;
				case SDLK_s: vel.y = 100; logger("<ShipEx><", id, "> moved down"); break;
				case SDLK_d: vel.x = 100; logger("<ShipEx><", id, "> moved right"); break;
			}
		}
	}
	void ShipEx::Update(float p_dt)
	{
		bullets->Update(p_dt);
		Actor::Update(p_dt);

	}
	void ShipEx::Render()
	{
		bullets->Render();
		Actor::Render();
	}
}