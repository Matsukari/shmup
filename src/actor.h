#pragma once
#define actor_h

#include "../lib/include/timer.h"
#include "visual_object.h"


namespace Shmup
{
	class Actor : public VisualObject
	{
	public:
		Actor(const VisualObject& p_obj);
		virtual ~Actor() override;

		int Get_Health() const noexcept { return health; }
		FVec2 Get_Vel() const noexcept { return vel; }
		FRect Get_CollRect() const noexcept { return collrect; }

		void Set_Health(int p_health) noexcept { health = p_health; }
		void Set_Vel(FVec2 p_vel) noexcept { vel = p_vel; }
		void Set_CollRect(const FRect& p_collrect) noexcept { collrect = p_collrect; }

		bool Is_Alive() const noexcept { return is_alive; }
		virtual void Update(float p_dt) override;

	protected:
		bool is_alive;
		int health;
		FRect collrect;

		FVec2 vel;

		Timer move_timer;
	};
	using ActorArray = std::vector<Actor*>;


}