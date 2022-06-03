#include "../include/collisions.h"



bool Has_Collided(const FCircle& base, const FRect& obj) noexcept
{
	SDL_FPoint c{0, 0};

	if (base.x < obj.Left()) 
	{
		c.x = obj.Left();
	}
	else if (base.x > obj.Right()) 
	{
		c.x = obj.Right();
	}
	else
		c.x = base.x; // in center;

	if (base.y < obj.Top()) 
	{
		c.y = obj.Top();
	}
	else if (base.y > obj.Bottom()) 
	{
		c.y = obj.Bottom();
	}
	else
		c.y = base.y; // in center;

	return (Get_DistanceSquared(base, c)  <  Get_Squared(base.r));

}