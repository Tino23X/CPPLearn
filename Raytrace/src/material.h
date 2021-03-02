#pragma once
#include "../../src/RayTrace/geometry.h"
class Material
{
public:
	Material(const Vec2f& a, const Vec3f& color, const float& spec) :albedo(a), diffuseColor(color), specularExponent(spec) {}
	Material() : albedo(1, 0), diffuseColor(), specularExponent() {}
	Vec3f GetDiffuseColor() const { return diffuseColor; }
	float GetSpecularExponent() const { return specularExponent; }
	Vec2f GetAlbedo() const { return albedo; }

private:
	Vec2f albedo;
	Vec3f diffuseColor;
	/// <summary>
	/// 高光系数
	/// </summary>
	float specularExponent;


};
