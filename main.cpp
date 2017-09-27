#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
using namespace std;

#include "Triangle.h"
#include "constants.h"
#include "font.h"

int main() {
	if (!hwtype() || !has_colors) { //non-CX nspire
		cout << "Unsupported calculator" << endl;
		return 1;
	}
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16, SDL_SWSURFACE);
	gfxPrimitivesSetFont(fontdata, FONT_WIDTH, FONT_HEIGHT);
	//SDL_Cursor *cursor = SDL_GetCursor();
	SDL_ShowCursor(1);

	SDL_Event event;
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

	Triangle triangle;
	triangle.message("Instructions:");

	bool open = true;
    while (open) { //main draw loop
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0)); //clear screen
		triangle.drawOn(screen);
		SDL_SetCursor(NULL); //triggers cursor redraw
		SDL_Flip(screen);

		idle();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_KP0: case SDLK_0: triangle.sendChar('0'); break;
				case SDLK_KP1: case SDLK_1: triangle.sendChar('1'); break;
				case SDLK_KP2: case SDLK_2: triangle.sendChar('2'); break;
				case SDLK_KP3: case SDLK_3: triangle.sendChar('3'); break;
				case SDLK_KP4: case SDLK_4: triangle.sendChar('4'); break;
				case SDLK_KP5: case SDLK_5: triangle.sendChar('5'); break;
				case SDLK_KP6: case SDLK_6: triangle.sendChar('6'); break;
				case SDLK_KP7: case SDLK_7: triangle.sendChar('7'); break;
				case SDLK_KP8: case SDLK_8: triangle.sendChar('8'); break;
				case SDLK_KP9: case SDLK_9: triangle.sendChar('9'); break;
				case SDLK_KP_ENTER: case SDLK_RETURN: triangle.sendChar('\n'); break;
				case SDLK_ESCAPE: triangle.cancelSelection(); break;
				case SDLK_PERIOD: triangle.sendChar('.'); break;
				case SDLK_q: open = false; break; //quit
				case SDLK_r: triangle.reset(); break; //resets triangle to default state
				case SDLK_c: triangle.clear(); break; //clears set dimensions
				case SDLK_BACKSPACE:
					if (event.key.keysym.mod & KMOD_CTRL)
						triangle.clear();
					else
						triangle.sendChar('\b');
					break;
				}
			break;
			case SDL_MOUSEBUTTONUP:
				//if (event.button.button == SDLK_LEFT)
					//cout << "Mouse click at: (" << event.button.x << "," << event.button.y << ")\n";
				triangle.clickAt(Vector2f(event.button.x, event.button.y));
			break;
			}
		}
    }
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}
