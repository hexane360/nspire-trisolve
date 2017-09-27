#pragma once
#include "SDL/SDL_config_tinspire.h"
#include "SDL/SDL_video.h"

static const int WINDOW_WIDTH = 320;
static const int WINDOW_HEIGHT = 240;

static const int BOX_WIDTH = 270; //Message box dimensions
static const int BOX_HEIGHT = 120;

static const double PI = 3.1415926;
static const double TO_RADS = PI / 180.0;
static const double TO_DEGS = 180.0 / PI;

static const int CLICK_PAD = 5; //clicks can select text from this distance
static const int H_TEXT_OFFSET = 8; //how far to space text from points on triangle
static const int V_TEXT_OFFSET = 4;

static const SDL_Color DEFAULT_COLOR = (SDL_Color){0, 153, 255, 0};    //blue, for unset dimensions
static const SDL_Color SET_COLOR = (SDL_Color){255, 51, 51, 0};        //red, for set dimension
static const SDL_Color SELECTED_COLOR = (SDL_Color){255, 255, 51, 0};  //yellow, for dimensions mid-entry
