#pragma once
#define timer_h
#include <SDL2/SDL.h>

class Timer
{
public:
	Timer() noexcept;
	~Timer() noexcept;

	Uint32 Pause() noexcept;
	Uint32 Resume() noexcept;

	Uint32 Peek() noexcept;
	Uint32 SinceLastPeek() const noexcept;

private:

	Uint32 startt;
	Uint32 curt;
	Uint32 pausedst; // paused start time
	Uint32 pausedt; // lapsed time since start of pause
	Uint32 lastpeekt;

	//bool is_paused;

	
};

