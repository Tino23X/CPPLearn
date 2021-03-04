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

	//����� snell ϵ��
	float refractiveIndex;

	/// <summary>
	/// 1λ���ǹ���ɫ��ǿ��
	/// 2λ���ƹ�������ǰ��
	/// 3λ��������ǿ��
	/// 4λ�������ǿ��
	/// </summary>
	Vec4f albedo;
	Vec3f diffuseColor;
	/// <summary>
	/// �߹�ϵ��
	/// </summary>
	float specularExponent;


};
