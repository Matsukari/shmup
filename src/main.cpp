
#include "../lib/include/logger.h"
#include "../lib/include/sdlut.h"
#include "../lib/include/timer.h"
#include "../lib/include/texture.h"
#include "../lib/include/ttf.h"
#include "../lib/include/window.h"
#include "../lib/include/collisions.h"
#include <vector>

// g++ logger.o color.o timer.o shapes.o window.o texture.o ttf.o collisions.o sdlut.o main.o -std=c++17 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o exe; ./exe


void SDL_SetRenderDrawColor(SDL_Renderer* p_renderer, SDL_Color p_color, Uint8 p_alpha = 255)
{
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_alpha);
}

#define KEY_REPEAT_ENDT 100
#define MOUSE_REPEAT_ENDT 100

class Event
{
public:
	Event();
	~Event();

	void Set_KeyRepeatEndT(Uint32 p_ms)   noexcept { key_repeatendt = p_ms; } 
	void Set_MouseRepeatEndT(Uint32 p_ms) noexcept { mouse_repeatendt = p_ms; }

	bool Set_InputCursor(SDL_Renderer* p_renderer, Texture* p_texture);

	void TextInput_Start();
	void TextInput_End();

	// called after HandleEvents
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

	bool has_quit;

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
	bool is_keydown;
	bool is_mousedown;


};
SDL_Event Event::event;

Event::Event()
{
	// key
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


#include <SDL2/SDL2_gfxPrimitives.h>

class App : public Window, public Event
{
public:
	App();
	~App();

	void Exe(); // run

protected:
	void On_Events();
	void On_Update();
	void On_Render();

private:
	TTF font;

	Timer timer;

	float dt = 0;
	float fps = 0;
	int rendered_frames = 0;


	FCircle a;
	FCircle b;

};

App::App() :
	Window(
		"Ex", Rect{
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		720, 
		550}, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED),
	font(renderer, "assets/fonts/sans.ttf", 24)
{
	//SDL_StartTextInput();
	a = FCircle{100, 100, 60};
	b = FCircle{200, 200, 60};

}
App::~App()
{
	//SDL_StopTextInput();

	//delete shmup;
	logger("Ended App");
}

void App::Exe()
{
	logger("App starting to run");
	while(!Event::Has_Quit())
	{
		On_Events();
		On_Update();
		On_Render();
	}
}

void App::On_Events()
{
	while(SDL_PollEvent(&event))
	{
		Event::HandleEvents();
	}
}
void App::On_Update()
{
	SDL_RenderPresent(renderer);
	dt = timer.SinceLastPeek() / 1000.0f;
	fps = rendered_frames / (timer.Peek() / 1000.0f);

	Event::Update();

	Circle temp = a;
	temp.x = Get_MousePos().x;
	temp.y = Get_MousePos().y;

	if (!Has_Collided(temp, b))
	{
		a = temp;
	}

}

template<class T>
std::string ts(const T& n)
{
	return std::move(std::to_string(n));
}

void App::On_Render()
{
	SDL_SetRenderDrawColor(renderer, BLACK);
	SDL_RenderClear(renderer);


	Rect fontrect(0, 0, 15, 15);
	font.Render(std::to_string(fps), fontrect);


	//SDL_SetRenderDrawColor(renderer, WHITE);
	filledCircleRGBA(renderer, b.x, b.y, b.r,  255, 255, 255, 255);


	//SDL_SetRenderDrawColor(renderer, GREEN);
	filledCircleRGBA(renderer, a.x, a.y, a.r,  0, 255, 0, 255);



	rendered_frames++;
}









int main(int argc, char** argv)
{
	if (!SDLU_Init(SDL_INIT_EVERYTHING))
	{
		return -1;
	}
	App* app = new App();
	app->Exe();
	delete app;
	app = nullptr;

	logger("Closing Audio");
	Mix_CloseAudio();

	SDLU_Quit();

	return 0;
}