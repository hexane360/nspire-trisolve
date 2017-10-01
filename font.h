#pragma once
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "Vector2.h"

Vector2u textSize(const std::string &s);
void drawString(SDL_Surface*, const Vector2u&, const std::string&, uint32_t);
void drawStringRGB(SDL_Surface*, const Vector2u&, const std::string&, uint8_t = 255, uint8_t = 255, uint8_t = 255);

//#include "font_6x8.h"
#include "font_8x14.h"
