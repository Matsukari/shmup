#pragma once
#define collisions_h

#include "math.h"
#include "shapes.h"

// SDL_IntersectRect, SDL_HasIntersection

// only work if A is smaller than B. decrepit at right to left }
template<class Ta, class Tb>
inline bool Is_InsideRect(const Ta& A, const Tb& B) noexcept
{
	bool intersect_x = (A.Right() > B.Left() && A.Right()  <  B.Right())  || 
					   (A.Left() < B.Right() && A.Left() > B.Left());

	bool intersect_y = (A.Bottom() > B.Top()  && A.Bottom() <  B.Bottom()) || 
					   (A.Top()  < B.Bottom() && A.Top()  > B.Top());


	return intersect_x && intersect_y; 
}

// circles, all float
bool Has_Collided(const FCircle& base, const FRect& obj) noexcept; // circle / rect
inline bool Has_Collided(const FCircle& base, const FCircle& obj) noexcept // circle / circle
{
	return (Get_DistanceSquared(base, obj) < Get_Squared(base.r + obj.r));

}
