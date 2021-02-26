#pragma once
#include "../../src/RayTrace/geometry.h"
#include <fstream>
#include "Sphere.h"
using namespace std;

class Screen
{
public:
	Screen(int w, int h) : width(w), height(h), frameBuffer(w* h),fov(M_PI / 3.0), originPoint(0, 0, 0) {}
	void Render() {

		ofstream ofs; // save the framebuffer to file
		ofs.open("./out.ppm", ofstream::out | ofstream::binary);
		ofs << "P6\n" << width << " " << height << "\n255\n";
		for (size_t i = 0; i < height * width; ++i) {
			for (size_t j = 0; j < 3; j++) {
				//framebuffer[i]���ص���һ��Vec3f
				ofs << (char)(255 * max(0.f, min(1.f, frameBuffer[i][j])));
			}
		}
		ofs.close();
	}

	void SetFrameBuffer(vector<Sphere>& spheres) {

#pragma omp parallel for
		for (size_t j = 0; j < height; ++j)
		{
			for (size_t i = 0; i < width; ++i)
			{
				float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.0) * width / (float)height;
				float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.0);
				Vec3f dir = Vec3f(x, y, -1).normalize();
				frameBuffer[i + j * width] = CastRay(originPoint, dir, spheres);
			}
		}
	}

	/// <summary>
	/// �����е��ཻ���
	/// </summary>
	/// <param name="orig">����ԭ��</param>
	/// <param name="dir">���߷���</param>
	/// <param name="spheres">��</param>
	/// <param name="hitPoint"></param>
	/// <param name="hitPointNormal"></param>
	/// <param name="material"></param>
	/// <returns></returns>
	bool SceneIntersect(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, Vec3f& hitPoint, Vec3f& hitPointNormal, Material& material)
	{
		//��Բ�ཻ�ľ���
		float sphereDist = numeric_limits<float>::max();
		for (Sphere sphere : spheres)
		{
			//��ǰ��Բ�ཻ�ľ���
			float tmpDist;
			//�����ǰ������Բ�ཻ�����ཻ����С��֮ǰ���ཻ����
			if (sphere.RayIntersect(orig, dir, tmpDist) && tmpDist < sphereDist)
			{
				sphereDist = tmpDist;
				hitPoint = orig + dir * tmpDist;
				hitPointNormal = (hitPoint - sphere.GetCenter()).normalize();
				material = sphere.GetMaterial();
			}
		}

		return sphereDist < 1000;
	}

	Vec3f CastRay(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres)
	{
		Vec3f hitPoint;
		Vec3f normal;
		Material material;

		if (!SceneIntersect(orig, dir, spheres, hitPoint, normal, material))
			return Vec3f(0.2, 0.7, 0.8);

		return material.GetDiffuseColor();

	}

	vector<Vec3f>& GetFrameBuffer() { return frameBuffer; }


private:
	vector<Vec3f> frameBuffer;
	int width;
	int height;
	const float fov;
	const Vec3f originPoint;

};
