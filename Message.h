#pragma once

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "Vector2.h"
#include "constants.h"
#include "font.h"

class Message {
public:
	Message();
	void display(const std::string&);
	void drawOn(SDL_Surface*) const;
	bool contains(const Vector2f&) const;
    bool displayed = false;
private:
	std::string _msg;
	Vector2u _msgPos;
	static const Vector2u _minPos;
	static const Vector2u _maxPos;
	static const std::string _helpMsg;
	Vector2u _helpMsgPos;
};
