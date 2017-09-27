#include "Text.h"

using namespace std;

Text::Text() : _dir(OffsetLeft) {
	//cout << "Text::Text()\n";
	//_textSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, _maxWidth, _maxHeight, 16, 0b1111100000000000, 0b0000011111100000, 0b0000000000011111, 0);
}
void Text::_update() {
	//cout << "Text::_update()\n";
	_drawPos = _pos;
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
void Text::setColor(const SDL_Color &color) {
	//cout << "Text::setColor(" << hex << color.r << color.g << color.b << dec << ")\n";
	_color = color;
}
void Text::setDrawPos(const Vector2u &pos) {
	_pos = pos;
	_update();
}
void Text::setOffsetDir(const OffsetDirection &dir) {
	_dir = dir;
	_update();
}
void Text::setText(const string &text) {
	//cout << "Text::setText(" << text << ")\n";
	_content = text;
	_size = Vector2u(FONT_WIDTH * _content.length(), FONT_HEIGHT);
	//_size = Vector2i(nSDL_GetStringWidth(_font, cstr), nSDL_GetStringHeight(_font, cstr));
	//SDL_FillRect(_textSurface, NULL, SDL_MapRGB(_textSurface->format, 0, 0, 0));
	//nSDL_DrawString(_textSurface, _font, 0, 0, cstr);
	_update();
}

Vector2u Text::getMaxBound() const {
	return _drawPos + _size;
}
void Text::drawOn(SDL_Surface *window) const {
	/*
	for (Uint8 i = 0; i < _size.x; i++) {
		for (Uint8 j = 0; j < _size.y; j++) {
			if (nSDL_GetPixel(_textSurface, i, j)) { //text pixel nonzero
				nSDL_SetPixel(window, i + _drawPos.x, j + _drawPos.y, _pixel); //fill with colored pixel
			} else {
				nSDL_SetPixel(window, i + _drawPos.x, j + _drawPos.y, 0); //draw black otherwise
			}
		}
	}
	*/
	drawStringRGB(window, _drawPos, _content, _color.r, _color.g, _color.b);
}
bool Text::contains(const Vector2f &point) const {
	Vector2u max = getMaxBound();
	return (_drawPos.x - CLICK_PAD <= point.x) && (_drawPos.y - CLICK_PAD <= point.y) && (max.x + CLICK_PAD >= point.x) && (max.y + CLICK_PAD >= point.y);
}
