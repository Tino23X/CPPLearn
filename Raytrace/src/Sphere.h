#pragma once
#include "../../src/RayTrace/geometry.h"

class Sphere
{
public:
	Sphere(const Vec3f& c, const float& r) : center(c), radius(r) {}

	/// <summary>
	/// 射线与圆求交
	/// </summary>
	/// <param name="orig">射线原点</param>
	/// <param name="dir">摄像方向</param>
	/// <param name="intersectPoint">交点位置</param>
	/// <returns></returns>
	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& intersectPoint) const
	{
		//射线起点到圆心向量
		Vec3f originToCenter = center - orig;
		//射线到圆心的距离在射线上的投影
		Vec3f projectionOnRay = originToCenter * (originToCenter * dir);
		//圆心到射线的距离
		float centerToRay = sqrtf(originToCenter * originToCenter - projectionOnRay * projectionOnRay);

		if (centerToRay > radius)
			return false;

		return true;
	}

private:
	Vec3f center;
	float radius;

};

