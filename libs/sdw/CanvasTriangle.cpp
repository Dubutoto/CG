#include "CanvasTriangle.h"

CanvasTriangle::CanvasTriangle() = default;
CanvasTriangle::CanvasTriangle(const CanvasPoint &v0, const CanvasPoint &v1, const CanvasPoint &v2) :
    vertices({{v0, v1, v2}}) {}

CanvasPoint &CanvasTriangle::v0() {
    return vertices[0];
}

CanvasPoint &CanvasTriangle::v1() {
    return vertices[1];
}

CanvasPoint &CanvasTriangle::v2() {
    return vertices[2];
}

CanvasPoint CanvasTriangle::operator[](size_t i) const {
    return vertices[i];
}

CanvasPoint &CanvasTriangle::operator[](size_t i) {
    return vertices[i];
}

std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle) {
	os << triangle[0] << triangle[1] << triangle[2];
	return os;
}
