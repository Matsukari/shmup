#pragma once
#define rectman_h

#include "../lib/include/shapes.h"
#include "../lib/include/math.h"
#include <functional>

namespace Shmup
{
	using FRect_Manip = std::function<FRect (const FRect&, const FRect&)>;
	namespace RectMan
	{
		FRect CenterToCenter(const FRect& base, const FRect& p_rect)
		{
			FRect rect = base;
			rect.Set_CenterToCenter(p_rect);
			return std::move(rect);
		}
		// rect inside until half of base size offset to outside
		FRect RandomInsideOffMid(const FRect& base, const FRect& p_rect)
		{
			FRect rect = base;
			rect.x = Get_Random(p_rect.Left() - (base.w/2), p_rect.Right() - (base.w/2));
			rect.y = Get_Random(p_rect.Top() - (base.h/2), p_rect.Bottom() - (base.h/2));

			return std::move(rect);
		}
	}

}