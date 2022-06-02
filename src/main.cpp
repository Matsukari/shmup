
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
class Event
{
public:
	Event();
	~Event();

	void Set_KeyHoldST(Uint32 p_ms); // ms to start hold
	void Set_KeyRepeatFT(Uint32 p_ms); // repeat fade time
	bool Set_InputCursor(SDL_Renderer* p_renderer, Texture* p_texture);

	void TextInput_Start();
	void TextInput_End();

	// Occurs non-continiously, only after MOUSEBUTTONUP, safe to say, only once after button is up. Also within up.
	virtual void Handle_Mouse() {}
	virtual void Handle_Keyboard() {}
	virtual void Handle_Text_Input() {}

	virtual void Update();

	const static SDL_Event& GetEvent() { return event; }

	SDL_Point Get_MousePos() const noexcept;
	SDL_Point Get_MouseHeldStartPos() const noexcept;
	SDL_Point Get_MouseHeldEndPos() const noexcept;


	SDL_Keycode Get_CurrKey() const noexcept;
	SDL_Keycode Get_PrevKey() const noexcept;
	Uint32 Get_KeyRepeats() const noexcept;
	Uint32 Get_MOuseRepeats() const noexcept;

	Uint32 Get_KeyHeldT() const noexcept;
	Uint32 Get_MouseHeldT() const noexcept;

	Uint32 Get_KeyUnheldT() const noexcept;
	Uint32 Get_MouseUnheldT() const noexcept;

	bool Has_Quit() const noexcept { return has_quit; }

protected:
	static SDL_Event event;
	SDL_Keycode current_key; // the key got after keydown, not fading
	SDL_Keycode prev_key;
	Uint32 		key_repeats;

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
	std::vector<std::string> textstr;
	size_t cur_line;
	bool   has_newline;

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


	Timer key_heldtimer;
	Timer key_unheldtimer;

};
SDL_Event Event::event;

Event::Event()
{
	current_key = SDLK_BACKSPACE;
	prev_key = SDLK_BACKSPACE;
	key_repeats = 0;

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

	mouse_rec_max = 30;
	key_rec_max = 30;

	// history cache
	mouse_posrec.resize(mouse_rec_max);
	mouse_butrec.resize(mouse_rec_max);
	key_rec.resize(key_rec_max);

	has_quit = false;

}
Event::~Event()
{
	
}
void Event::Update()
{
	// get mouse position periodicallt
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	// polymorph pointer to SDL_Event
	while(SDL_PollEvent(&event))
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
			mouse_startpos = mouse_pos;

			// record
			mouse_posrec[mouse_rec_idx] = mouse_pos;
			mouse_butrec[mouse_rec_idx] = event.button.button;

			// index for next record
			mouse_rec_idx = (mouse_rec_idx + 1) % mouse_rec_max; // roll back when exceeding max capacity
		}
		case SDL_MOUSEBUTTONUP:
		{
			mouse_endpos = mouse_pos;
		}

		case SDL_KEYDOWN:
		{
			key_heldtimer.Peek();
			key_unheldt = key_unheldtimer.SinceLastPeek();
			
		}
		case SDL_KEYUP:
		{
			key_unheldtimer.Peek();
			key_heldt = key_heldtimer.SinceLastPeek();
		}

		}
	}

}



class App : public Window
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
	Event event;
	TTF font;

	Timer timer;

	bool has_quit = false;
	float dt = 0;
	float fps = 0;
	int rendered_frames = 0;
};

App::App() :
	Window(
		"Ex", Rect{
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		720, 
		550}, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
		SDL_RENDERER_ACCELERATED),
	font(renderer, "assets/fonts/sans.ttf", 24)
{
	has_quit = false;

}
App::~App()
{
	//delete shmup;
	logger("Ended App");
}

void App::Exe()
{
	logger("App starting to run");
	while(!has_quit)
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
		if(event.type == SDL_QUIT ||
		  (event.type == SDL_WINDOWEVENT && event.window.event   == SDL_WINDOWEVENT_CLOSE) ||
		  (event.type == SDL_KEYDOWN     && event.key.keysym.sym == SDLK_ESCAPE))
		{
			has_quit = true;
			
			logger(" Quitt");
		}

	}
}
void App::On_Update()
{
	SDL_RenderPresent(renderer);
	dt = timer.SinceLastPeek() / 1000.0f;
	fps = rendered_frames / (timer.Peek() / 1000.0f);


}
void App::On_Render()
{
	SDL_SetRenderDrawColor(renderer, BLACK);
	SDL_RenderClear(renderer);


	Rect fontrect(0, 0, 15, 15);
	font.Render(std::to_string(fps), fontrect);
	
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