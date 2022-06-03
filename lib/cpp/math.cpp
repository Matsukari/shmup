#include "../include/math.h"





float Get_PointDistance(SDL_FPoint A, SDL_FPoint B) noexcept
{
     return sqrtf(  Get_Squared(A.x - B.x) + Get_Squared(A.y - B.y) );
}
float Get_DistanceSquared(SDL_FPoint A, SDL_FPoint B) noexcept
{
	float delta_x = (B.x - A.x);
	float delta_y = (B.y - A.y);

	return Get_Squared(delta_x) + Get_Squared(delta_y);
}


float Get_AngleFrom(SDL_FPoint src, SDL_FPoint dst) noexcept
{
	return Get_Degree(atan2(dst.y - src.y, dst.x - src.x));
}

void WaitTime(float seconds) noexcept
{
	clock_t start_time      = clock();
	float   seconds_to_wait = seconds * CLOCKS_PER_SEC;

	// Wait until current time passes seconds given;
	while (clock() < start_time + seconds_to_wait);
}

float Get_Random(float min, float max)
{
	timeval t;

	WaitTime(0.0001);

	gettimeofday(&t, NULL);
	srand(t.tv_usec * t.tv_sec);

	return min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(max-min)));
}


