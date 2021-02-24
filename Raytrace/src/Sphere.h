#pragma once
#include "../../src/RayTrace/geometry.h"

class Sphere
{
public:
	Sphere(const Vec3f& c, const float& r) : center(c), radius(r) {}

	/// <summary>
	/// ������Բ��
	/// </summary>
	/// <param name="orig">����ԭ��</param>
	/// <param name="dir">������</param>
	/// <param name="intersectPoint">����λ��</param>
	/// <returns></returns>
	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& intersectPoint) const
	{
		//������㵽Բ������
		Vec3f originToCenter = center - orig;
		//���ߵ�Բ�ĵľ����������ϵ�ͶӰ
		Vec3f projectionOnRay = originToCenter * (originToCenter * dir);
		//Բ�ĵ����ߵľ���
		float centerToRay = sqrtf(originToCenter * originToCenter - projectionOnRay * projectionOnRay);

		if (centerToRay > radius)
			return false;

		return true;
	}

private:
	Vec3f center;
	float radius;

};

