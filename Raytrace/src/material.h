#pragma once
#include "../../src/RayTrace/geometry.h"
class Material
{
public:
	Material(const Vec3f& color) : diffuseColor(color) {}
	Material() : diffuseColor() {}
	Vec3f GetDiffuseColor() const { return diffuseColor; }

private:
	Vec3f diffuseColor;


};
