#pragma once
#include "../../src/RayTrace/geometry.h"
class Material
{
public:
	Material(const float& r, const Vec4f& a, const Vec3f& color, const float& spec) :refractiveIndex(r), albedo(a), diffuseColor(color), specularExponent(spec) {}
	Material() : refractiveIndex(1), albedo(1, 0, 0, 0), diffuseColor(), specularExponent() {}
	Vec3f& GetDiffuseColor() { return diffuseColor; }
	float GetSpecularExponent() const { return specularExponent; }
	Vec4f GetAlbedo() const { return albedo; }
	float GetRefractiveIndes() const { return refractiveIndex; }

private:

	//折射光 snell 系数
	float refractiveIndex;

	/// <summary>
	/// 1位，是固有色的强度
	/// 2位，灯光照射后的前度
	/// 3位，反射光的强度
	/// 4位，折射光强度
	/// </summary>
	Vec4f albedo;
	Vec3f diffuseColor;
	/// <summary>
	/// 高光系数
	/// </summary>
	float specularExponent;


};
