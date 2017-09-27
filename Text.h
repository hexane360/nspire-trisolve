#pragma once

#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "Vector2.h"
#include "constants.h"
#include "font.h"

class Text {
public:
	enum OffsetDirection {
		OffsetLeft,
		OffsetRight,
		OffsetUp,
		OffsetDown
	};
	Text();
	void setDrawPos(const Vector2u&);
	void setOffsetDir(const OffsetDirection&);
	void setText(const std::string&);
	void setColor(const SDL_Color&);
	bool contains(const Vector2f&) const;
	Vector2u getMaxBound() const;
	void drawOn(SDL_Surface*) const;
private:
	void _update();
	Vector2u _pos;
	Vector2u _drawPos;
	Vector2u _size;
	std::string _content;
	SDL_Surface *_textSurface;
	SDL_Color _color;
	OffsetDirection _dir;
	const Uint8 _maxHeight = 10;
	const Uint8 _maxWidth = 100;
};
