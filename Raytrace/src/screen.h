#pragma once
#include "../../src/RayTrace/geometry.h"
#include <fstream>
#include "Sphere.h"
#include "light.h"
using namespace std;

class Screen
{
public:
	Screen(int w, int h) : width(w), height(h), frameBuffer(w* h), fov(M_PI / 3.0), originPoint(0, 0, 0) {}
	void Render() {

		ofstream ofs; // save the framebuffer to file
		ofs.open("./out.ppm", ofstream::out | ofstream::binary);
		ofs << "P6\n" << width << " " << height << "\n255\n";
		for (size_t i = 0; i < height * width; ++i) {

			//����buffer�е�������ȣ�
			Vec3f& c = frameBuffer[i];
			float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1)
				c = c * (1. / max);

			for (size_t j = 0; j < 3; j++) {
				//framebuffer[i]���ص���һ��Vec3f
				ofs << (char)(255 * std::max(0.f, min(1.f, frameBuffer[i][j])));
			}
		}
		ofs.close();
	}

	void SetFrameBuffer(vector<Sphere>& spheres, const vector<Light>& lights) {

#pragma omp parallel for
		for (size_t j = 0; j < height; ++j)
		{
			for (size_t i = 0; i < width; ++i)
			{
				float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.0) * width / (float)height;
				float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.0);
				Vec3f dir = Vec3f(x, y, -1).normalize();
				frameBuffer[i + j * width] = CastRay(originPoint, dir, spheres, lights);
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

	Vec3f CastRay(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, const vector<Light>& lights)
	{
		Vec3f hitPoint;
		//����Ŀ��㵽Camera������
		Vec3f hitPointToCameraNormal;
		Material material;

		if (!SceneIntersect(orig, dir, spheres, hitPoint, hitPointToCameraNormal, material))
			return Vec3f(0.2, 0.7, 0.8);

		float diffuseLightIntensity = 0;
		float specularLightIntensity = 0;

		for (auto light : lights)
		{
			//�����ķ���
			Vec3f lightDir = (light.GetPosition() - hitPoint).normalize();

			diffuseLightIntensity += light.GetIntensity() * max(0.f, lightDir * hitPointToCameraNormal);
			//��Ŀ��㷴��������������������ˣ��õ���ǰ��ļнǣ�ԽСֵԽ��, Ȼ��͸߹�ϵ���������㡣
			specularLightIntensity += pow(max(0.f, -Reflect(-lightDir, hitPointToCameraNormal) * dir), material.GetSpecularExponent()) * light.GetIntensity();
		}

		return material.GetDiffuseColor() * diffuseLightIntensity * material.GetAlbedo()[0] + Vec3f(1, 1, 1) * specularLightIntensity * material.GetAlbedo()[1];

	}

	/// <summary>
	/// ���䷽��
	/// </summary>
	/// <param name="I"></param>
	/// <param name="N"></param>
	/// <returns></returns>
	Vec3f Reflect(const Vec3f& I, const Vec3f& N)
	{
		return I - N * 2.f * (I * N);
	}

	vector<Vec3f>& GetFrameBuffer() { return frameBuffer; }


private:
	vector<Vec3f> frameBuffer;
	int width;
	int height;
	const float fov;
	const Vec3f originPoint;

};
