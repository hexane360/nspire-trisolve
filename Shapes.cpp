#include "Shapes.h"
using namespace std;

Dimension::Dimension() {
	_text.setColor(DEFAULT_COLOR);
}

void Dimension::select() {
    //_text.setOutlineColor(Color::White);
    _partial = "";
    _update("_");
    _text.setColor(SELECTED_COLOR);
    _selected = true;
}

bool Dimension::selected() const { return _selected; } //whether or not dimension is selected

void Dimension::cancelSelection() { //deselect dimension
    if (_selected) {
        _selected = false;
        if (_driving)
            _text.setColor(SET_COLOR);
        else
            _text.setColor(DEFAULT_COLOR);
        _update();
    }
}

void Dimension::makeDriven() { //if dimension is set, reset it
    _driving = false;
    _text.setColor(DEFAULT_COLOR);
}

bool Dimension::isDriving() const { return _driving; } //getter for _driving

void Dimension::setOffsetDir(const Text::OffsetDirection &dir) {
	_text.setOffsetDir(dir);
}
bool Dimension::contains(const Vector2f &pos) const { //whether click is within dimension text
	return _text.contains(pos);
}
void Dimension::sendChar(char c) {
    if (c == '\n') { //parse and set value
        double newVal = getValue();
        if (_partial != "") { //can't parse an empty string, but we still want to set the dimension
            try {
                newVal = stod(_partial);
            } catch (invalid_argument &) {cout << "error: \"" << _partial << "\" couldn't be parsed\n";}
              catch(out_of_range &) {cout << "out_of_range in parsing\n";}
        }
        if (newVal < 0.0001) { //0 breaks solving
            cancelSelection();
        } else {
            _text.setColor(SET_COLOR);
            _driving = false; //to allow modifying value
            setValue(newVal);
            _selected = false;
            _driving = true;
        }
        return;
    }
    if (c == '\b') { //backspace
    	if (_partial.length())
	        _partial.pop_back(); //delete last character
    } else {
        _partial.push_back(c); //push char to partial entry
    }
    if (_partial == "") //update text shape
        _update("_");
    else
        _update(_partial);
}

Side::Side() {
	//cout << "Side::Side()\n";
    _length = 1;
}
void Side::_update(const string &override) {
    stringstream ss;
    if (override == "") {
        ss << setprecision(5) << _length;
    } else {
        ss << override;
    }
    ss << (unsigned char)34; //'"' symbol
    _text.setText(ss.str());
}
Vector2f Side::midpoint() const { //returns midpoint of line
	return (_pos + _endPos)/static_cast<float>(2.0);
}

void Side::setDrawPos(const Vector2f& newPos, double newAng) { //where to draw side on screen
	//cout << "Side::setDrawPos((" << newPos.x << "," << newPos.y << "), " << newAng << ")\n";
	_pos = newPos;
	_drawAng = newAng;
	_endPos = newPos + static_cast<float>(_length*_scale)*Vector2f(cos(_drawAng*TO_RADS),sin(_drawAng*TO_RADS));
	//cout << "_endPos: (" << _endPos.x << "," << _endPos.y << ")\n";
    _text.setDrawPos((Vector2u)midpoint()); //gets halfway point of line
}
void Side::setValue(double newLength) { //set driven dimension to solved value
    if (!_driving) {
        _length = newLength;
        _update();
    }
}
void Side::setScale(double newScale) { //px/units
	_scale = newScale;
}
double Side::getValue() const { //gets current value (set or solved)
    return _length;
}
void Side::drawOn(SDL_Surface* window) const { //draw all shapes
	lineRGBA(window, _pos.x, _pos.y, _endPos.x, _endPos.y, 255, 255, 255, 255);
	_text.drawOn(window);
}

Angle::Angle() {
	//cout << "Angle::Angle()\n";
    _angle = 60;
}
void Angle::_update(const string &override) {
    stringstream ss;
    if (override == "") {
        ss << setprecision(5) << _angle;
    } else {
        ss << override;
    }
    ss << (unsigned char)248; //deg symbol
	_text.setText(ss.str());

    //bool drawOut = _arc.getAngle() < 33 || _arc.getAngle() > 156; //angle too small or big to draw inside?
    //double xOrigin = (_flipText == drawOut) ? -offset : bounds.width + offset; //offset to left or right of text? --> | offset <texttexttext> offset |
}
void Angle::setDrawPos(const Vector2f& newPos, double newAng) { //pos to draw angle on screen
	_drawAng = newAng;
	_pos = newPos;
    _text.setDrawPos((Vector2u)newPos);
}
void Angle::setValue(double newAng) { //update angle with solved value
    if (!_driving) { //don't override set value
        _angle = newAng;
        _update(); //recalculate text with new value
    }
}
double Angle::getValue() const { //get current value, solved or set
    return _angle;
}
void Angle::drawOn(SDL_Surface *window) const {
	//angles negated so positive is CCW
	arcRGBA(window, _pos.x, _pos.y, 15, -_drawAng - _angle, -_drawAng, 255, 255, 255, 255);
	_text.drawOn(window);
}
