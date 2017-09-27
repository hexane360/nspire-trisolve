#include "Message.h"
using namespace std;

const string Message::_helpMsg = "Press R to reset triangle\nPress C to clear all dimensions\nPress ESC to clear selected dim.\n\nPress enter to continue";
const Vector2u Message::_minPos((WINDOW_WIDTH - BOX_WIDTH)/2.0, (WINDOW_HEIGHT - BOX_HEIGHT)/2.0);
const Vector2u Message::_maxPos(_minPos.x + BOX_WIDTH, _minPos.y + BOX_HEIGHT);
//SDL_Rect Message::_msgRect{_minPos.x, _minPos.y, BOX_WIDTH, BOX_HEIGHT};

Message::Message() {
	//Vector2u helpMsgSize = Vector2u(nSDL_GetStringWidth(_font, _helpMsg), nSDL_GetStringHeight(_font, _helpMsg));
	Vector2u helpMsgSize = textSize(_helpMsg);
	_helpMsgPos = Vector2u((WINDOW_WIDTH - helpMsgSize.x)/2, _maxPos.y - helpMsgSize.y - 7);
}

void Message::display(const string &msg) {
	_msg = msg;
	//_msgPos = Vector2u((WINDOW_WIDTH - nSDL_GetStringWidth(_font, _msg.c_str()))/2, _minPos.y + 10);
	Vector2u msgSize = textSize(_msg);
	_msgPos = Vector2u((WINDOW_WIDTH - msgSize.x)/2, _minPos.y + 7);
	displayed = true;
}

bool Message::contains(const Vector2f &pos) const {
	return (pos.x >= _minPos.x) && (pos.y >= _minPos.y) && (pos.x <= _maxPos.x) && (pos.y <= _maxPos.y);
}

void Message::drawOn(SDL_Surface *surface) const {
	boxRGBA(surface, _minPos.x, _minPos.y, _maxPos.x, _maxPos.y, 0, 0, 0, 255); //filled black rectangle
	rectangleRGBA(surface, _minPos.x, _minPos.y, _maxPos.x, _maxPos.y, 255, 255, 255, 255); //white outline
	
	drawStringRGB(surface, _msgPos, _msg);
	drawStringRGB(surface, _helpMsgPos, _helpMsg);
	//nSDL_DrawString(surface, _font, _msgPos.x, _msgPos.y, _msg.c_str());
	//nSDL_DrawString(surface, _font, _helpMsgPos.x, _helpMsgPos.y, _helpMsg);
}
