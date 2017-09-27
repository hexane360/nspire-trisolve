#include "font.h"
using namespace std;

Vector2u textSize(const string &s) {
	Vector2u size(0, FONT_HEIGHT);
	unsigned int rowWidth = 0;
	for (size_t i = 0; i < s.length(); i++) {
		if (s[i] == '\n') {
			size.y += V_PADDING + FONT_HEIGHT;
			if (rowWidth > size.x) size.x = rowWidth;
			rowWidth = 0;
		} else {
			rowWidth += FONT_WIDTH + H_PADDING;
		}
	}
	if (rowWidth > size.x) size.x = rowWidth;
	size.x -= H_PADDING;
	return size;
}

void drawString(SDL_Surface* surface, const Vector2u &pos, const string &s, Uint32 color) {
	Vector2u drawPos = pos;
	for (size_t i = 0; i < s.length(); i++) {
		if (s[i] == '\n') {
			drawPos.x = pos.x;
			drawPos.y += FONT_HEIGHT + V_PADDING;
		} else {
			characterColor(surface, drawPos.x, drawPos.y, s[i], color);
			drawPos.x += FONT_WIDTH + H_PADDING;
		}
	}
}

void drawStringRGB(SDL_Surface* surface, const Vector2u &pos, const string &s, Uint8 r, Uint8 g, Uint8 b) {
	Uint32 color = (r << 24) + (g << 16) + (b << 8) + 0xFF;
	drawString(surface, pos, s, color);
}
