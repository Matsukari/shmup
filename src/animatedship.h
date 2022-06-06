#pragma once
#define animatedship_h

#include "ship.h"
#include "framelist.h"

namespace Shmup
{
	// keyed actions, containing: [frame][x]
	using FrameMap = std::map<std::string, FrameList>;
	// A ship with actions
	class AnimatedShip : public Ship
	{
	public:
		AnimatedShip(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap);
		virtual ~AnimatedShip() override;

		void Set_Act(std::string p_act) noexcept 
		{ 
			assert(act_framemap->find(p_act) != act_framemap->end());
			curr_act = p_act;
		}

		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		// member frames will be void
    	FrameMap* act_framemap;
    	std::string curr_act;

		
	};
	AnimatedShip::AnimatedShip(const VisualObject& p_obj, Gun* p_gun, FrameMap* p_act_framemap) : 
		Ship(p_obj, p_gun), 
		act_framemap(p_act_framemap)
	{
		//gun = new Gun()
		logger("Initializing <AnimatedShip><", id, ">...");

		curr_act = "idle";

	}
	AnimatedShip::~AnimatedShip()
	{
		logger("Destructing <AnimatedShip><", id, ">...");
		act_framemap = nullptr;
	}


	/*void AnimatedShip::HandleEvents(const SDL_Event& p_event)
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
					logger("<AnimatedShip><", id, "> moved up"); 
					break;
				case SDLK_a: // LEFT
					//has_movedleft = true;
					curaction = "move_left";
					is_inaction = true;
					
					vel.x = -100; 
					logger("<AnimatedShip><", id, "> moved left"); 
					break;
				case SDLK_s: // BOTTOM
					vel.y = 100; 
					logger("<AnimatedShip><", id, "> moved down"); 
					break;
				case SDLK_d: // RIGHT
					//has_movedright = true;
					curaction = "move_right";
					is_inaction = true;

					vel.x = 100; 
					logger("<AnimatedShip><", id, "> moved right"); 
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

	}*/
	void AnimatedShip::Update(float p_dt)
	{
		Ship::Update(p_dt);

		act_framemap->at(curr_act).Run(p_dt);
		
	}
	void AnimatedShip::Render()
	{
		gun->Render();
		texture->Render(rect, &act_framemap->at(curr_act).Curr());
		
	}
}