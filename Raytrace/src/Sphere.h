#pragma once
#include "../../src/RayTrace/geometry.h"
#include "material.h"
#define M_PI 3.14159265358979323846

class Sphere
{
public:
	Sphere(const Vec3f& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}

	/// <summary>
	/// 射线与圆求交
	/// 查看文件夹中的"射线和圆相交示意图"
	/// </summary>
	/// <param name="orig">射线原点</param>
	/// <param name="dir">摄像方向</param>
	/// <param name="intersectPoint">射线起点与圆交点的长度</param>
	/// <returns></returns>
	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& t0) const
	{
		Vec3f L = center - orig;
		float tca = L * dir;
		float dSquare = L * L - tca * tca;
		if (dSquare > radius * radius)
			return false;
		float thc = sqrtf(radius * radius - dSquare);
		t0 = tca - thc;
		float t1 = tca + thc;

		if (t0 < 0)
			t0 = t1;
		if (t0 < 0) 
			return false;

		return true;
	}

	Vec3f GetCenter() { return center; }
	Material GetMaterial() { return material; }

private:
	Vec3f center;
	float radius;
	Material material;

};

