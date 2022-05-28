#pragma once
#define effect_h

#include "entity.h"

namespace Shmup
{

	class Effect : public Entity
	{
	public:
		Effect(const EntityProp& p_prop);
		~Effect();

		virtual void Play(int p_encore=1);
		virtual void Update(float p_dt) override;
		
	protected:
		int encore;
		bool is_playing;
	};

	Effect::Effect(const EntityProp& p_prop) : 
		Entity(p_prop)
	{
		logger("Initializing <Effect><", id, ">...");

		is_playing = false;
	}
	Effect::~Effect()
	{
		logger("Destructing <Effect><", id, ">...");

	}

	void Effect::Play(int p_encore)
	{
		logger("Playing <Effect><", id, "> ", p_encore, " times");

		if (p_encore < 1)
		{
			p_encore = 1;
			logger("At <Effect><", id, "> encore must not be lower than 1!");

		}
		encore = p_encore;
		is_playing = true;
	}

	void Effect::Update(float p_dt)
	{
		if (is_playing)
		{
			curframe += fps * p_dt;
			if (curframe >= frames->size())
			{
				curframe = 0;
				encore--;
				if (encore == 0)
				{
					is_playing = false;
				}
			}
		}
	}

}