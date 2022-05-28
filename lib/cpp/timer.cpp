#include "../include/timer.h"


Timer::Timer() noexcept
{
	startt = SDL_GetTicks();
	curt = 0;
	pausedt = 0;
	pausedst = 0;
	lastpeekt = 0;

	//is_paused = true;

}
Timer::~Timer() noexcept
{
}

Uint32 Timer::Pause() noexcept
{	
	//is_paused = true;
	pausedst = (SDL_GetTicks() - startt) - pausedst; // nullify time from previous pause(case consecutively)
	return pausedst; // start time
}
Uint32 Timer::Resume() noexcept
{
	pausedt += (SDL_GetTicks() - startt) - pausedst; // time lapsed
	pausedst = 0;
	//is_paused = false;
	return pausedt;
}

Uint32 Timer::Peek() noexcept
{
	lastpeekt = (SDL_GetTicks() - startt) - pausedt;
	return lastpeekt;
}
Uint32 Timer::SinceLastPeek() const noexcept
{
	return (SDL_GetTicks() - startt) - lastpeekt;
}