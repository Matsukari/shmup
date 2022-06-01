#pragma once
#define shipex_h

#include "ship.h"

namespace Shmup
{
	// keyed actions, containing: [frame][x]
	using FrameMap = std::map<std::string, RectArray>;
	// A ship with actions
	class ShipEx : public Ship
	{
	public:
		ShipEx(const EntityProp& p_prop, Gun* p_gun, const FrameMap& p_framemap);
		~ShipEx() override;

		void HandleEvents(const SDL_Event& p_event) override;
		void Update(float p_dt) override;
		void Render() override;

	protected:
		// member frames will be void
    	const FrameMap framemap;
    	bool has_movedleft;
    	bool has_movedright;
    	bool is_inaction;
    	std::string curaction;


		
	};
	ShipEx::ShipEx(const EntityProp& p_prop, Gun* p_gun, const FrameMap& p_framemap) : 
		Ship(p_prop, p_gun), 
		framemap(p_framemap)
	{
		//gun = new Gun()
		logger("Initializing <ShipEx><", id, ">...");
		move_timer.Peek();
		has_movedleft = false;
		has_movedright = false;
		is_inaction = false;
		curaction = "idle";

	}
	ShipEx::~ShipEx()
	{
		logger("Destructing <ShipEx><", id, ">...");
		
	}


	void ShipEx::HandleEvents(const SDL_Event& p_event)
	{

		if (p_event.type == SDL_KEYDOWN)
		{
			switch(p_event.key.keysym.sym)
			{
				// fly my boy! soar high! (TOP of ship)
				case SDLK_j: 
					gun->Fire(Vecf2{0, -250}, 10); 
					break;
				case SDLK_w:  // UP
					vel.y = -100; 
					logger("<ShipEx><", id, "> moved up"); 
					break;
				case SDLK_a: // LEFT
					//has_movedleft = true;
					curaction = "move_left";
					is_inaction = true;
					
					vel.x = -100; 
					logger("<ShipEx><", id, "> moved left"); 
					break;
				case SDLK_s: // BOTTOM
					vel.y = 100; 
					logger("<ShipEx><", id, "> moved down"); 
					break;
				case SDLK_d: // RIGHT
					//has_movedright = true;
					curaction = "move_right";
					is_inaction = true;

					vel.x = 100; 
					logger("<ShipEx><", id, "> moved right"); 
					break;
			}
		}
		else if (p_event.type == SDL_KEYUP && is_inaction)
		{
			switch(p_event.key.keysym.sym)
			{
				case SDLK_a: case SDLK_d:
					is_inaction = false;
					logger("NOT IN ACTION");

					break;

			}
		}

	}
	void ShipEx::Update(float p_dt)
	{
		gun->Update(p_dt);
		auto NextFrame = [&](int add)
		{
			if (move_timer.SinceLastPeek() >= fspeed)
			{
				move_timer.Peek();
				curframe += add;
				logger(curframe);

				if (curframe >= (int)framemap.at(curaction).size())
				{
					curframe = framemap.at(curaction).size() - 2;
					//logger("___________________HOHOHOHO____________________");
				}
				else if (curframe < 0) // when rolling back
				{
					//logger("____________________BACK TO IDLE____________________");
					curframe = 0;
					// back to idle
					curaction = "idle";
					
				}
			}
		};
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;


		if (is_inaction || curaction == "idle")
		{ 
			NextFrame(1); 
			//logger("ADVANCING"); 
		}
		else if (!is_inaction && curaction != "idle") 
		{ 
			NextFrame(-1); 
			//logger("ROLLING BACK"); 
		}


		
		
	}
	void ShipEx::Render()
	{
		gun->Render();
		texture->Render(Rect{rect}, &framemap.at(curaction).at(curframe));
		
	}
}