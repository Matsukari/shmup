#pragma once
#define bullet_h

#include "../lib/include/collisions.h"
#include "actor.h"


namespace Shmup
{
	// fired the moment it's initialized
	class Bullet : public Actor
	{
	public:
		Bullet(const VisualObject& p_obj, ActorArray* p_targets, const RectArray* p_frames, FVec2 p_vel, int p_dmg);
		virtual ~Bullet() override;

		void Set_Frames(const RectArray& p_frames) noexcept { frames = &p_frames; }

		virtual void Update(float p_dt) override;
		virtual void Render() override;


	protected:
		const RectArray* frames;
		unsigned int curr_frame;
		ActorArray* targets;
		int dmg;
		
	};
	

}