#pragma once
#define event_h

#include "texture.h"
#include "shapes.h"
#include "timer.h"
#include "logger.h"
#include <vector>
#include <string>


#define KEY_REPEAT_ENDT 100
#define MOUSE_REPEAT_ENDT 100

class Event
{
public:
	Event();
	~Event();

	void Set_KeyRepeatEndT(Uint32 p_ms)   noexcept { key_repeatendt = p_ms; } 
	void Set_MouseRepeatEndT(Uint32 p_ms) noexcept { mouse_repeatendt = p_ms; }

	// called outside PollEvent
	virtual void Update();
	// called under SDL_PollEvent()
	virtual void HandleEvents();

	const static SDL_Event& GetEvent() { return event; }

	SDL_Keycode Get_CurrKey() const noexcept { return curr_key; }
	SDL_Keycode Get_PrevKey() const noexcept { return prev_key; }
	Uint32 Get_KeyRepeats()   const noexcept { return key_repeats; }
	Uint32 Get_MouseRepeats() const noexcept { return mouse_repeats; }

	Uint32 Get_KeyHeldT() 	  const noexcept { return key_heldt; }
	Uint32 Get_MouseHeldT()   const noexcept { return mouse_heldt; }
	Uint32 Get_KeyUnheldT()   const noexcept { return key_unheldt; }
	Uint32 Get_MouseUnheldT() const noexcept { return mouse_unheldt; }

	SDL_Point Get_MousePos() 	const noexcept { return mouse_pos; }
	SDL_Point Get_MouseStart() 	const noexcept { return mouse_startpos; }
	SDL_Point Get_MouseEnd() 	const noexcept { return mouse_endpos; }
	SDL_Point Get_Wheel() 		const noexcept { return wheel; }

	const std::string& Get_TextInput() const noexcept { return textinput; }


	const std::vector<SDL_Point>&   Get_MousePosRec() const noexcept { return mouse_posrec; }
	const std::vector<Uint8>&       Get_MouseButRec() const noexcept { return mouse_butrec; }
	const std::vector<SDL_Keycode>& Get_KeyRec() const noexcept { return key_rec; }


	bool Has_Quit() const noexcept { return has_quit; }

protected:
	static SDL_Event event;

private:
	SDL_Keycode curr_key; // the key got after keydown, not fading
	SDL_Keycode prev_key;
	Uint32 		key_repeats;
	Uint32 		mouse_repeats;

	// time
	Uint32 key_heldt;
	Uint32 mouse_heldt;

	Uint32 key_unheldt;
	Uint32 mouse_unheldt;

	// position
	SDL_Point mouse_pos;
	SDL_Point mouse_startpos; // Position of mouse within pressing.  This will remain until you hold agin.//
	SDL_Point mouse_endpos; // Position of mouse after pressing. This will remain until you hold agin.//
	SDL_Point wheel;  // WHEEL_DOWN or WHEEL_UP
	Rect mouse_resrect; // resulting rect after holding mouse button

	// text input
	std::string textinput;
	//unsigned int curr_line;
	//bool   has_newline;

	// history cache
	std::vector<SDL_Point> mouse_posrec; // where mouse is hold in position, record
	std::vector<Uint8> 	   mouse_butrec; // mouse button pressed, record
	std::vector<SDL_Keycode> key_rec; // If current index is in limits, reset(roll back) and overwite the beginning //

private:
	unsigned int key_rec_idx;
	unsigned int mouse_rec_idx;

	unsigned int key_rec_max;
	unsigned int mouse_rec_max;

	Uint32 key_repeatendt;
	Uint32 mouse_repeatendt;


	Timer key_heldtimer;
	Timer mouse_heldtimer;

private:
	bool has_quit;
	bool is_keydown;
	bool is_mousedown;


};
