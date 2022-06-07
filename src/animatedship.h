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
			logger("Changed act to '", p_act, "'");
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

		Set_Act("idle");

	}
	AnimatedShip::~AnimatedShip()
	{
		logger("Destructing <AnimatedShip><", id, ">...");
		act_framemap = nullptr;
	}


	void AnimatedShip::Update(float p_dt)
	{
		Ship::Update(p_dt);

		act_framemap->at(curr_act).On_NextFrame();
		
	}
	void AnimatedShip::Render()
	{
		gun->Render();
		//logger("ACtion: ", curr_act, ", Frame(", act_framemap->at(curr_act).Idx(), "): ", act_framemap->at(curr_act).Curr().x, ", ", act_framemap->at(curr_act).Curr().y, ", ", act_framemap->at(curr_act).Curr().w, ", ", act_framemap->at(curr_act).Curr().h);
		texture->Render(rect, &(*act_framemap->at(curr_act)));
		
	}
}