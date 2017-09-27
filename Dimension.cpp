#include "Dimension.h"
using namespace std;

void Dimension::_update(const string &override) {
    stringstream ss;
    if (override == "_") {
        ss << setprecision(5) << _value;
    } else {
        ss << override;
    }
    ss << _suffix;
	_text = ss.str();
	_size = Vector2u(FONT_WIDTH * _text.length(), FONT_HEIGHT);
}

void Dimension::sendChar(char c) {
    if (c == '\n') { //parse and set value
		double newVal = _value;
		if (_partial != "") { //can't parse an empty string, but we still want to set the dimension
            try {
                newVal = stod(_partial);
            } catch (invalid_argument &) {cout << "error: \"" << _partial << "\" couldn't be parsed\n";}
              catch(out_of_range &) {cout << "out_of_range in parsing\n";}
        }
        if (newVal < 0.0001) { //0 breaks solving
            cancelSelection();
        } else {
            _color = SET_COLOR;
            _value = newVal;
            _selected = false;
            _driving = true;
            _update(); //make sure to apply standard formatting
        }
        return;
    }
    if (c == '\b') { //backspace
    	if (_partial.length())
	        _partial.pop_back(); //delete last character
    } else {
        _partial.push_back(c); //push char to partial entry
    }
    _update(_partial);
}

void Dimension::setValue(double val) {
	if (!_driving) {
		_value = val;
		_update();
	}
}

void Dimension::setPos(const Vector2f &pos) {
	_pos = pos;
	_drawPos = (Vector2u)_pos;
	switch (_dir) {
	case OffsetRight:
		_drawPos.x += H_TEXT_OFFSET;
		_drawPos.y -= _size.y / 2;
	break;
	case OffsetLeft:
		_drawPos.x += -_size.x - H_TEXT_OFFSET;
		_drawPos.y -= _size.y / 2;
	break;
	case OffsetDown:
		_drawPos.x -= _size.x / 2;
		_drawPos.y += V_TEXT_OFFSET;
	break;
	case OffsetUp:
		_drawPos.x -= _size.x / 2;
		_drawPos.y += -_size.y - V_TEXT_OFFSET;
	break;
	}
}

void Dimension::drawOn(SDL_Surface *window) const {
	drawString(window, _drawPos, _text, _color);
}

void Dimension::select() {
    _partial = "";
    _update("");
    _color = SELECTED_COLOR;
    _selected = true;
}
void Dimension::cancelSelection() { //deselect dimension
    if (_selected) {
        _selected = false;
        if (_driving)
            _color = SET_COLOR;
        else
            _color = DEFAULT_COLOR;
        _update();
    }
}
void Dimension::makeDriven() {
    _driving = false;
    _color = DEFAULT_COLOR;
}
void Dimension::setOffsetDir(const OffsetDirection &dir) { _dir = dir; }
void Dimension::setSuffix(char suffix) { _suffix = suffix; }

double Dimension::getValue() const { return _value; }
bool Dimension::contains(const Vector2f &point) const { //is point within text?
	Vector2u min = _drawPos - Vector2u(CLICK_PLAY, CLICK_PLAY);
	Vector2u max = _drawPos + _size + Vector2u(CLICK_PLAY, CLICK_PLAY);
	return (min.x <= point.x) && (min.y <= point.y) && (max.x >= point.x) && (max.y >= point.y);
}
bool Dimension::selected() const { return _selected; }
bool Dimension::isDriving() const { return _driving; }
