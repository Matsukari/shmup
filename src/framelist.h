#pragma once
#define FrameList_h

#include "visual_object.h"

namespace Shmup
{
	#define FRAMELIST_FSPEED 100
	class FrameList : public RectArray
	{
	public:
		FrameList(Uint32 p_fspeed=FRAMELIST_FSPEED) :
			fspeed(p_fspeed)
		{
			timer.Peek();
		}
		FrameList(const RectArray& p_arr, Uint32 p_fspeed=FRAMELIST_FSPEED) : 
			RectArray(p_arr),
			fspeed(p_fspeed)
		{
			logger("Initializing <FrameList>, got size: ", size());
			shrink_to_fit();
			timer.Peek();

		}
		~FrameList()=default;

		const Rect& operator*() const { return Curr(); }

		const Rect& Curr() const { return at(curr); }


		// may you boon
		template<class Func, class... Ts>
		void On_NextFrame(Func&& lambda, Ts... args)
		{
			if (timer.SinceLastPeek() > fspeed)
			{
				timer.Peek();
				lambda(args...);
			}
		}
		void On_NextFrame()
		{
			if (timer.SinceLastPeek() > fspeed)
			{
				timer.Peek();
				curr++;
				if (curr >= size())
				{
					curr = 0;
				}
			}
		}

	public:
		int curr {0};
		Uint32 fspeed;
		Timer timer;

	};



}