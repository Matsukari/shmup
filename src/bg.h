#pragma once
#define bg_h

#include "actor.h"

namespace Shmup
{
	// A ship with actions
	class BG : public Actor
	{
	public:
		BG(const EntityProp& p_prop, Vecf2 p_vel);
		~BG() override;

		void StopAdvance() noexcept {}
		void ScatterClouds() noexcept {}

		void Update(float p_dt) override;
		void Render() override;

	protected:
		FRect buffer_rect;
		float bottom;
		
	};
	BG::BG(const EntityProp& p_prop, Vecf2 p_vel) : 
		Actor(p_prop)
	{
		logger("Initializing <BG><", id, ">...");
		buffer_rect = rect;
		buffer_rect.y -= rect.h;
		bottom = rect.Bottom();

		SetVel(p_vel);
		SetVel(p_vel);

	}
	BG::~BG()
	{
		logger("Destructing <BG><", id, ">...");
		
	}



	void BG::Update(float p_dt)
	{
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;

		buffer_rect.x += vel.x * p_dt;
		buffer_rect.y += vel.y * p_dt;

		if (rect.y > bottom)
		{
			rect.y = buffer_rect.y - rect.h;
		}
		else if (buffer_rect.y > bottom)
		{
			buffer_rect.y = rect.y - buffer_rect.h;
		}
	}
	void BG::Render()
	{
		texture->Render(Rect{buffer_rect});
		texture->Render(Rect{rect});

	}
}