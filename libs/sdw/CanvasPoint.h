#pragma once

#include "TexturePoint.h"
#include <iostream>

struct CanvasPoint {
	float x{};
	float y{};
	float depth{};
	float brightness{};
	TexturePoint texturePoint{};

	CanvasPoint();
	CanvasPoint(float xPos, float yPos);
	CanvasPoint(float xPos, float yPos, float pointDepth);
	CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness);
	friend std::ostream &operator<<(std::ostream &os, const CanvasPoint &point);
};
