#pragma once
#include "../../src/RayTrace/geometry.h"
class Light
{
public:
	Light(const Vec3f& p, const float& i) : position(p), intensity(i) {}

	Vec3f GetPosition() const { return position; }
	float GetIntensity() const { return intensity; }

private:

	Vec3f position;
	float intensity;

};
