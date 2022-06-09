#pragma once
#define actor_h

#include "../lib/include/timer.h"
#include "visual_object.h"
#include "framelist.h"


namespace Shmup
{
	class Move4
	{
	public:
		virtual ~Move4() {}

		// pixels per second
		virtual void Move_Left(int p_vel) = 0;
		virtual void Move_Right(int p_vel) = 0;
		virtual void Move_Top(int p_vel) = 0;
		virtual void Move_Bottom(int p_vel) = 0;

	};
	class Reaction
	{
	public:
		virtual ~Reaction()=0;
		virtual void Activate()=0;
		
	};
	Reaction::~Reaction(){}


	class Animation : public VisualObject, public FrameList
	{
	public:
		Animation(const VisualObject& p_obj, const FrameList& p_frames, const FRect* p_spawnp) : 
			VisualObject(p_obj),
			FrameList(p_frames),
			spawnp(p_spawnp)
		{

		}
		virtual ~Animation() {}

		virtual void Play() noexcept 
		{ 
			is_playing = true; 
			rect.x = spawnp->x;
			rect.y = spawnp->y;

		} // static

		virtual void Update(float p_dt)
		{
			auto UntilEnd = [&]()
			{
				curr++;
				if (curr >= size())
				{
					curr = 0;
					is_playing = false;
				}
			};
			if (is_playing)
			{
				VisualObject::Update(p_dt);
				FrameList::On_NextFrame(UntilEnd);
			}
		}
		virtual void Render()
		{
			if (is_playing)
			{
				texture->Render(rect, &Curr());
			}
		}

	protected:
		bool is_playing = false;
		const FRect* spawnp;

		
	};

	class AnimReaction : public Reaction
	{
	public:
		AnimReaction(Animation* p_anim) : anim(p_anim) {}
		~AnimReaction() 
		{ 
			anim = nullptr;
		}
		virtual void Activate()
		{
			anim->Play();
		}

	protected:
		Animation* anim;
		
	};
	class Actor : public VisualObject
	{
	public:
		Actor(const VisualObject& p_obj);
		virtual ~Actor() override;

		virtual void Set_DeathReaction(Reaction* p_react) noexcept { deathreaction = p_react; }

		int Get_Health() const noexcept { return health; }
		FVec2 Get_Vel() const noexcept { return vel; }
		const FRect& Get_CollRect() const noexcept { return collrect; }

		void Set_Health(int p_health) noexcept { health = p_health; }
		void Set_Vel(FVec2 p_vel) noexcept { vel = p_vel; }
		void Set_CollRect(const FRect& p_collrect) noexcept { collrect = p_collrect; }

		bool Is_Alive() const noexcept { return is_alive; }
		virtual void Update(float p_dt) override;

		void Kill() noexcept { is_alive = false; }
		void DeathReact() noexcept { is_deathreact = true; }


	protected:
		bool is_alive;
		int health;
		FRect collrect;

		FVec2 vel;

		Reaction* deathreaction;
		bool is_deathreact;

	};
	using ActorArray = std::vector<Actor*>;


	Actor::Actor(const VisualObject& p_obj) :
		VisualObject(p_obj)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		health = 110;
		collrect = rect;
		
		vel = FVec2{0.0f, 0.0f};

		deathreaction = nullptr;
		is_deathreact = false;
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
		if (is_deathreact && deathreaction)
		{
			deathreaction->Activate();
			logger("________Activating Destuctor Reaction_____");
		}
		//std::cout << "~ACTOR" << std::endl;
	}

	void Actor::Update(float p_dt)
	{
		rect.x += vel.x * p_dt;
		rect.y += vel.y * p_dt;

		// escape screen
		Rect lvalue = rect;
		if (!SDL_HasIntersection(&lvalue, screen))
		{
			logger("<Actor><", id, "> escaped from screen! x: ", rect.x, ", y: ", rect.y);
			is_alive = false;
		}
	}

}