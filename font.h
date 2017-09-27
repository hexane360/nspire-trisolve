#pragma once
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "Vector2.h"

Vector2u textSize(const std::string &s);
void drawString(SDL_Surface*, const Vector2u&, const std::string&, Uint32);
void drawStringRGB(SDL_Surface*, const Vector2u&, const std::string&, Uint8 = 255, Uint8 = 255, Uint8 = 255);

//#include "font_6x8.h"
#include "font_8x14.h"
