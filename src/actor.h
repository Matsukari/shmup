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
	using Reaction_ptr = std::unique_ptr<Reaction>;
	using FRect_Manip = std::function<FRect (const FRect&, const FRect&)>;

	namespace RectMan
	{
		FRect CenterToCenter(const FRect& base, const FRect& p_rect)
		{
			FRect rect = base;
			rect.Set_CenterToCenter(p_rect);
			return std::move(rect);
		}
		// within p_rect revolving in its center
		FRect RandomInsideOffMid(const FRect& base, const FRect& p_rect)
		{
			FRect rect = base;
			rect.x = Get_Random(p_rect.Left() - (base.w/2), p_rect.Right() - (base.w/2));
			rect.y = Get_Random(p_rect.Top() - (base.h/2), p_rect.Bottom() - (base.h/2));

			return std::move(rect);
		}
	}


	class Animation : public VisualObject, public FrameList
	{
	public:
		Animation(const VisualObject& p_obj, const FrameList& p_frames, const FRect* p_spawnp, 
			FRect_Manip p_spawnp_man=RectMan::CenterToCenter) : 

			VisualObject(p_obj),
			FrameList(p_frames),
			spawnp(p_spawnp),
			spawnp_man(p_spawnp_man)
		{
			logger("Animtion created.");
		}

		void Set_SpawnPointManip(FRect_Manip p_spawnp_man=RectMan::CenterToCenter) noexcept { spawnp_man = p_spawnp_man; }
		virtual ~Animation() 
		{
			spawnp = nullptr;
			spawnp_man = nullptr;
			logger("Animation destruct");
		}

		virtual void Play() noexcept 
		{ 
			is_playing = true; 


			rect = spawnp_man(rect, *spawnp);

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
		FRect_Manip spawnp_man;

		
	};

	class AnimReaction : public Reaction
	{
	public:
		AnimReaction(std::shared_ptr<Animation> p_anim) : anim(std::move(p_anim)) {}
		~AnimReaction() 
		{ 
		}
		virtual void Activate()
		{
			anim->Play();
		}

		std::shared_ptr<Animation> Get_Anim() const { return anim; }

	protected:
		std::shared_ptr<Animation> anim; // this shit will be destroyed along reaction once it reached estructor, 
		// let outside manage the update of render of this anim
		
	};
	class Actor : public VisualObject
	{
	public:
		Actor(const VisualObject& p_obj);
		virtual ~Actor() override;

		// this reaction is only used to trigger activate(), output is left outside
		virtual void Set_DeathReaction(Reaction_ptr p_react) { dtreact = std::move(p_react); }

		int Get_Health() const noexcept { return health; }
		FVec2 Get_Vel() const noexcept { return vel; }
		const FRect& Get_CollRect() const noexcept { return collrect; }

		void Set_Health(int p_health) noexcept { health = p_health; }
		void Set_Vel(FVec2 p_vel) noexcept { vel = p_vel; }
		void Set_CollRect(const FRect& p_collrect) noexcept { collrect = p_collrect; }

		bool Is_Alive() const noexcept { return is_alive; }
		virtual void Update(float p_dt) override;

		void Kill() noexcept { is_alive = false; }
		void DeathReact() noexcept { is_dtreact = true; }


	protected:
		bool is_alive;
		int health;
		FRect collrect;

		FVec2 vel;

		Reaction_ptr dtreact;
		bool is_dtreact;

	};
	using ActorArray = std::vector<Actor*>;


	Actor::Actor(const VisualObject& p_obj) :
		VisualObject(p_obj)
	{
		logger("Initializing <Actor><", id, ">...");

		is_alive = true;
		health = 100;
		collrect = rect;
		
		vel = FVec2{0.0f, 0.0f};

		is_dtreact = false;
		//dtreact = nullptr;
	}
	Actor::~Actor()
	{
		logger("Destructing <Actor><", id, ">...");
		if (is_dtreact && dtreact)
		{
			dtreact->Activate();
			logger("________Activating Destuctor Reaction_____");
			//dtreact = nullptr;
		}
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