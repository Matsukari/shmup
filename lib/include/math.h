#pragma once
#define math_h
#define TURN_ANGLE_DIR 90 // how many angle to turn a 4 direction. Add this on Get_Angle_From()

#include <ctime>
#include <sys/time.h>
#include <cmath>
#include <SDL2/SDL.h>

#define PI 3.1415926535

// all float

inline float Get_Radian(float degree) noexcept
{
	return degree * (180.0f / PI);
}
inline float Get_Degree(float radian) noexcept
{
	return radian * (PI / 180.0f);
}
inline float Get_Squared(float n) noexcept
{
	return n * n;
}

inline float Get_PointDistance(SDL_FPoint  A, SDL_FPoint B) noexcept;
float Get_DistanceSquared(SDL_FPoint  A, SDL_FPoint  B) noexcept;
inline float Get_AngleFrom(SDL_FPoint src, SDL_FPoint dst) noexcept;

void WaitTime(float seconds) noexcept;
float Get_Random(float min, float max);

