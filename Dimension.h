#pragma once

#include <math.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "constants.h"
#include "Vector2.h"
#include "font.h"

class Dimension { //abstract class
public:
	enum OffsetDirection {
		OffsetLeft,
		OffsetRight,
		OffsetUp,
		OffsetDown
	};
	void sendChar(char); //type char to entry window
	void setValue(double); //called by Triangle to update with solved values
	void setPos(const Vector2f&); //update shapes, one corner of line
	void drawOn(SDL_Surface*) const; //called by draw loop

	void select(); //show cursor and wait for entry
	void cancelSelection();
	void makeDriven();		//reset value
	void setOffsetDir(const OffsetDirection&);
	void setSuffix(char);

	double getValue() const; //called by Triangle on d
	//double distFrom(const Vector2f&) const;
	bool contains(const Vector2f&) const; //point inside dimension?
	bool selected() const;
	bool isDriving() const; //whether value has been set by hand
private:
	void _update(const std::string& = "_"); //called whenever dimension text needs to be updated
	//double _euclidDist(const Vector2f&, const Vector2f&) const;
	std::string _partial;
	bool _driving = false;
	bool _selected = false;
	double _value;
	char _suffix;

	std::string _text;
	Vector2f _pos;
	OffsetDirection _dir;
	uint32_t _color = DEFAULT_COLOR;
	Vector2u _drawPos;
	Vector2u _size;
};
