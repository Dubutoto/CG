#include "CanvasPoint.h"

CanvasPoint::CanvasPoint() :
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(float xPos, float yPos) :
		x(xPos),
		y(yPos),
		depth(0.0),
		brightness(1.0),
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(float xPos, float yPos, float pointDepth) :
		x(xPos),
		y(yPos),
		depth(pointDepth),
		brightness(1.0),
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness) :
		x(xPos),
		y(yPos),
		depth(pointDepth),
		brightness(pointBrightness),
		texturePoint(-1, -1) {}

std::ostream &operator<<(std::ostream &os, const CanvasPoint &point) {
	os << "(" << point.x << ", " << point.y << ", " << point.depth << ") " << point.brightness;
	return os;
}
