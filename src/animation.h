#pragma once
#define animation_h

#include "visual_object.h"
#include "framelist.h"
#include "rectman.h"
#include "reaction.h"


namespace Shmup
{

	class Animation : public VisualObject, public FrameList
	{
	public:
		Animation(const VisualObject& p_obj, const FrameList& p_frames, const FRect* p_spawnp, 
			FRect_Manip p_spawnp_man=RectMan::CenterToCenter);
		virtual ~Animation();

		void Set_SpawnPointManip(FRect_Manip p_spawnp_man=RectMan::CenterToCenter) noexcept { spawnp_man = p_spawnp_man; }
		
		virtual void Play();
		virtual void Update(float p_dt);
		virtual void Render();

		bool Is_Alive() const noexcept { return is_alive; }
		void Set_Life(bool p_live) noexcept { is_alive = p_live; }

	protected:
		bool is_alive = true;
		bool is_playing = false;
		const FRect* spawnp;
		FRect_Manip spawnp_man;

		
	};

	class AnimReaction : public Reaction
	{
	public:
		AnimReaction(std::shared_ptr<Animation> p_anim) : anim(std::move(p_anim)) {}
		~AnimReaction() override {}

		virtual void Activate() override { anim->Play(); }

		std::shared_ptr<Animation> Get_Anim() const noexcept { return anim; }

	protected:
		std::shared_ptr<Animation> anim; 

		// this shit will be destroyed along reaction once it's owner class reached destructor, 
		// let outside manage the update and render of this anim
		
	};

	Animation::Animation(const VisualObject& p_obj, const FrameList& p_frames, const FRect* p_spawnp, 
		FRect_Manip p_spawnp_man) : 

		VisualObject(p_obj),
		FrameList(p_frames),
		spawnp(p_spawnp),
		spawnp_man(p_spawnp_man)
	{
		logger("Animtion created.");
	}
	Animation::~Animation() 
	{
		spawnp = nullptr;
		spawnp_man = nullptr;
		logger("Animation destruct");
	}

	void Animation::Play() 
	{ 
		is_playing = true; 


		rect = spawnp_man(rect, *spawnp);

	} // static

	void Animation::Update(float p_dt)
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
	void Animation::Render()
	{
		if (is_playing)
		{
			texture->Render(rect, &Curr());
		}
	}

}