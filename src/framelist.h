#pragma once
#define FrameList_h

#include "visual_object.h"

namespace Shmup
{

	#define FRAMELIST_SPEED 100
	class FrameList : public RectArray
	{
	public:
		FrameList() {}
		FrameList(const RectArray& p_arr, int p_fspeed, int jump = 1);
		virtual ~FrameList();

		const Rect& Curr() const { return at(curr_frame); }
		unsigned int Idx() const noexcept { return curr_frame; }

		void Start() noexcept { is_playing = true; }
		void Stop() noexcept { is_playing = false; }

		void Set_Jump(int p_jump) noexcept { jump = p_jump; }
		void Set_Start(unsigned int p_idx) noexcept { 
			start = (p_idx < 0 || p_idx >= size()) ? 0 : p_idx; 
		}
		void Set_End(unsigned int p_idx) noexcept { 
			end = (p_idx < 0 || p_idx >= size()) ? 0 : size(); 
		}
		virtual void Run(float p_dt);
		
	protected:
		bool is_playing {false};
		unsigned int start {0};
		unsigned int end {0};
		int fspeed { FRAMELIST_SPEED };
		int jump;

		Timer timer;
		unsigned int curr_frame;

	};

	FrameList::FrameList(const RectArray& p_arr, int p_fspeed, int p_jump) : 
		RectArray(p_arr),
		fspeed(p_fspeed),
		jump(p_jump)
	{
		logger("Initializing <FrameList>");
		
		Set_Start(0);
		Set_End(size());

		shrink_to_fit();
		timer.Peek();
	}
	FrameList::~FrameList()
	{
		logger("Destructing <FrameList>");

	}

	void FrameList::Run(float p_dt)
	{
		jump *= is_playing; // active if is_playing

		if (timer.SinceLastPeek() > fspeed)
		{
			timer.Peek();
			curr_frame += jump;
			if (curr_frame >= end)
			{
				curr_frame = start;
			}
		}
	
	}

}