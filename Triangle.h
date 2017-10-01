#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <SDL/SDL.h>

#include "Dimension.h"
#include "constants.h"
#include "Message.h"

struct Ambiguous {
	size_t sideIndex; //index of ambiguous side
	size_t angleIndex; //index of ambiguous angle not opposite ambiguous side
	double side; //alternate length of side
	Vector2f pos; //alternate triangle position (replaces _drawPos[angleIndex])
	bool choice; //chosen resolution
};

class Triangle {
public:
	enum State {
		DEFAULT,
		MESSAGE,
		AMBIGUOUS,
		RESOLVED
	};
	Triangle();
	void drawOn(SDL_Surface*) const;
	void clickAt(const Vector2f&);
	void sendChar(char);
	void clear();
	void reset();
	void cancelSelection();
	void chooseSolution(bool);
	void update();
	void message(const std::string&);
private:
	State _state = State::DEFAULT;
	Dimension _angles[3];
	Dimension _sides[3];
	Ambiguous _ambiguous;
	Vector2f _drawPos[3];
	double _drawAng[3];
	Message _messageBox;
	void _drawArc(SDL_Surface*, const Vector2f &center, double, double, double, uint32_t) const;

	bool _checkConstraints(double side[3], double angle[3], int &sideCount, int &angleCount);
	bool _lawOfCosines(double side[3], double angle[3], int &sideCount, int &angleCount);
	void _drawSolved(double side[3], double angle[3]);
};
