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
#include "Text.h"

class Dimension { //abstract class
public:
	Dimension();
	virtual void setDrawPos(const Vector2f &pos, double angle) = 0; //update shapes, one corner of line
	virtual void drawOn(SDL_Surface*) const = 0; //called each frame
	virtual void setValue(double val) = 0; //called by Triangle to update with solved values; updates shapes
	virtual double getValue() const = 0; //called by Triangle on d
	void select(); //show cursor and wait for entry
	void cancelSelection();
	//double distFrom(const Vector2f&) const;
	bool contains(const Vector2f&) const;
	bool selected() const;
	void sendChar(char c); //type char to entry window
	bool isDriving() const; //whether value has been set by hand
	void makeDriven();		//reset value
	void setOffsetDir(const Text::OffsetDirection&);
protected:
	virtual void _update(const std::string& = "") = 0; //called whenever dimension text needs to be updated
	//double _euclidDist(const Vector2f&, const Vector2f&) const;
	std::string _partial;
	Text _text;
	Vector2f _pos;
	double _drawAng;
	bool _driving = false;
	bool _selected = false;
};

class Side : public Dimension {
public:
	Side();
	virtual void setDrawPos(const Vector2f &pos, double angle); //update shapes, one corner of line
	virtual void drawOn(SDL_Surface*) const; //called each frame
	virtual void setValue(double); //called by Triangle to update with solved values; updates shapes
	virtual double getValue() const; //called by Triangle on d
	void setScale(double); //px/units
	Vector2f midpoint() const;
private:
	virtual void _update(const std::string& = "");
	Vector2f _endPos;
	double _length; //units
	double _scale; //px/units
};

class Angle : public Dimension {
public:
	Angle();
	virtual void setDrawPos(const Vector2f &pos, double angle); //update shapes, one corner of line
	virtual void drawOn(SDL_Surface*) const; //called each frame
	virtual void setValue(double val); //called by Triangle to update with solved values; updates shapes
	virtual double getValue() const; //called by Triangle on d
private:
	virtual void _update(const std::string& = "");
	double _angle; //angle
	double _drawAng;
	Vector2f _pos;
};
