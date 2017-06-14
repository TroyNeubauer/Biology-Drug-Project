#pragma once

#include "src\graphics\renderable2d.h"
using namespace troyberry;
using namespace graphics;
using namespace maths;

class Sprite : public Renderable2D
{
	private:
	public:
	Sprite(float x, float y, float width, float height, const vec4& color);
};

