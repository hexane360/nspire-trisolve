#pragma once

static const int WINDOW_WIDTH = 320;
static const int WINDOW_HEIGHT = 240;

static const int BOX_WIDTH = 270; //Message box dimensions
static const int BOX_HEIGHT = 120;

static const double PI = 3.1415926;
static const double TO_RADS = PI / 180.0;
static const double TO_DEGS = 180.0 / PI;

static const int CLICK_PLAY = 5; //count mouse clicks this far from text
static const int H_TEXT_OFFSET = 8; //how far to space text from points on triangle
static const int V_TEXT_OFFSET = 4;

#define COLOR(r,g,b) (r << 24) | (g << 16) | (b << 8) | 255

static const Uint32 DEFAULT_COLOR = COLOR(0, 153, 255);   //blue, for unset dimensions
static const Uint32 SET_COLOR = COLOR(255, 51, 51);       //red, for set dimension
static const Uint32 SELECTED_COLOR = COLOR(255, 255, 51); //yellow, for dimensions mid-entry
static const Uint32 SHAPE_COLOR = COLOR(255, 255, 255);   //white, for everything else
static const Uint32 AMBIG_COLOR = COLOR(255, 255, 51);    //yellow, for ambiguous triangle entry
