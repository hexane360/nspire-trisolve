#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <SDL/SDL.h>

#include "Dimension.h"
#include "constants.h"
#include "Message.h"

class Triangle {
public:
	Triangle();
	void drawOn(SDL_Surface*) const;
	void clickAt(const Vector2f&);
	void sendChar(char);
	void clear();
	void reset();
	void cancelSelection();
	void update();
	void message(const std::string&);
private:
	Dimension _angles[3];
	Dimension _sides[3];
	Vector2f _drawPos[3];
	double _drawAng[3];
	Message _messageBox;
	std::string _msg;
	bool _ignoreEvent = false;

	bool _checkConstraints(double side[3], double angle[3], int &sideCount, int &angleCount);
	bool _lawOfCosines(double side[3], double angle[3], int &sideCount, int &angleCount);
	void _drawSolved(double side[3], double angle[3]);
};
