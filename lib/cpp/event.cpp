#include "../include/event.h"


// static
SDL_Event Event::event;


Event::Event()
{
	logger("Event Initializing...");
	curr_key = SDLK_BACKSPACE;
	prev_key = SDLK_BACKSPACE;
	key_repeats = 0;
	mouse_repeats = 0;


	// time
	key_heldt = 0;
	mouse_heldt = 0;

	key_unheldt = 0;
	mouse_unheldt = 0;

	// position
	mouse_pos 		= SDL_Point{0, 0};
	mouse_startpos  = SDL_Point{0, 0};
	mouse_endpos	= SDL_Point{0, 0};
	wheel 		  = SDL_Point{0, 0};
	mouse_resrect = Rect{0, 0, 0, 0};

	// record max
	key_rec_max = 30;
	mouse_rec_max = 30;

	// history cache
	mouse_posrec.resize(mouse_rec_max);
	mouse_butrec.resize(mouse_rec_max);
	key_rec.resize(key_rec_max);
	mouse_posrec.shrink_to_fit();
	mouse_butrec.shrink_to_fit();
	key_rec.shrink_to_fit();

	// array index
	key_rec_idx = 0;
	mouse_rec_idx = 0;

	key_repeatendt = KEY_REPEAT_ENDT;
	mouse_repeatendt = MOUSE_REPEAT_ENDT;

	// timer
	key_heldtimer.Peek();
	mouse_heldtimer.Peek();

	// bool
	has_quit = false;
	is_keydown = false;
	is_mousedown = false;


}
Event::~Event()
{
	logger("Event Destrcuted");
}


void Event::HandleEvents()
{
	if(event.type == SDL_QUIT ||
	  (event.type == SDL_WINDOWEVENT && event.window.event   == SDL_WINDOWEVENT_CLOSE) ||
	  (event.type == SDL_KEYDOWN     && event.key.keysym.sym == SDLK_ESCAPE))
	{
		has_quit = true;
		logger("In Event; has_quit");
	}

	switch(event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		is_mousedown = true;
		mouse_startpos = mouse_pos;
		mouse_heldtimer.Peek();

		if (mouse_unheldt && mouse_unheldt < mouse_repeatendt)
			mouse_repeats++;
		else
			mouse_repeats = 0;

		// record
		mouse_posrec[mouse_rec_idx] = mouse_pos;
		mouse_butrec[mouse_rec_idx] = event.button.button;

		// index for next record
		mouse_rec_idx = (mouse_rec_idx + 1) % mouse_rec_max; // roll back when exceeding max capacity
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		is_mousedown = false;
		mouse_endpos = mouse_pos;
		mouse_heldtimer.Peek();

		break;
	}
	case SDL_MOUSEWHEEL:
	{
		wheel.x = event.wheel.x;
		wheel.y = event.wheel.y;
		break;
	}
	case SDL_KEYDOWN:
	{
		is_keydown = true;
		prev_key = curr_key;
		curr_key = event.key.keysym.sym;

		// record
		key_rec[key_rec_idx] = curr_key;

		// check repeat
		if (!event.key.repeat) key_heldtimer.Peek();
		
		// key must be unheld must be active (not zero) before comparing
		if (prev_key == curr_key && key_unheldt && key_unheldt < key_repeatendt)
			key_repeats++;
		else
			key_repeats = 0;

		// erase char in text input
		if (curr_key == SDLK_BACKSPACE && textinput.size() > 0)
		{
			textinput.pop_back();
		}
		else if (curr_key == SDLK_KP_ENTER)
		{
			textinput += '\n';
		}
	
		break;
	}
	case SDL_KEYUP:
	{
		is_keydown = false;
		key_heldtimer.Peek();
		break;
	}
	case SDL_TEXTINPUT:
	{
		textinput += event.text.text;
		break;
	}
	default:
	{
		wheel.x = 0;
		wheel.y = 0;
		break;
	}
	}
}

void Event::Update()
{
	// get mouse position periodicallt
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);


	key_heldt = key_heldtimer.SinceLastPeek() * is_keydown; // active when key is pressed
	key_unheldt = key_heldtimer.SinceLastPeek() * !is_keydown; // active when key is NOT pressed

	mouse_heldt = mouse_heldtimer.SinceLastPeek() * is_mousedown;
	mouse_unheldt = mouse_heldtimer.SinceLastPeek() * !is_mousedown;

	key_repeats *= (key_unheldt < key_repeatendt); // active until 'unheld time' is within 'endtime'
	mouse_repeats *= (mouse_unheldt < mouse_repeatendt);


}
