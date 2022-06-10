#pragma once
#define bg_h

#include "actor.h"

namespace Shmup
{
	class BGSprite : public Actor
	{
	public:
		BGSprite(const VisualObject& p_obj, Texture_ptr p_cloud, float p_speed) : Actor(p_obj)
		{
			vel.y = p_speed;
			
			buffer_rect = rect;
			buffer_rect.y -= rect.h;

			end = rect.Bottom();

		}
		//BGSprite::BGSprite(const VisualObject& p_obj, Texture_ptr p_cloud, float p_speed) : Actor(p_obj) {}

		~BGSprite() override
		{}



		void Update(float p_dt) override
		{
			buffer_rect.y += vel.y * p_dt;
			rect.y += vel.y * p_dt;

			if (rect.Top() > end)
			{
				rect.y = buffer_rect.Top() - rect.h;
			}
			else if (buffer_rect.Top() > end)
			{
				buffer_rect.y = rect.Top() - buffer_rect.h;
			}


		}
		void Render() override
		{
			texture->Render(buffer_rect);
			texture->Render(rect);
		}


	private:

		float end;
		FRect buffer_rect;
		FVec2 vel;


	};
}