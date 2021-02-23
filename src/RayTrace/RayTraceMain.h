#pragma once
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"
#define M_PI 3.14159265358979323846

struct Material
{
	Material(const Vec3f color) :diffuseColor(color) {}
	Material() :diffuseColor() {}
	Vec3f diffuseColor;
};

struct Sphere {

	//Բ��
	Vec3f center;
	//�뾶
	float radius;

	Material material;

	Sphere(const Vec3f& c, const float& r, const Material &m) : center(c), radius(r), material(m) {}

	//Բ�ཻ���
	bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
		Vec3f L = center - orig;
		float tca = L * dir;
		float d2 = L * L - tca * tca;
		if (d2 > radius * radius) return false;
		float thc = sqrtf(radius * radius - d2);
		t0 = tca - thc;
		float t1 = tca + thc;
		if (t0 < 0) t0 = t1;
		if (t0 < 0) return false;
		return true;
	}
};

bool SceneIntersect(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, Vec3f& hit, Vec3f& N, Material& material)
{
	float spheresDist = numeric_limits<float>::max();
	for (size_t i = 0; i < spheres.size(); ++i)
	{
		float dist;
		if (spheres[i].ray_intersect(orig, dir, dist) && dist < spheresDist)
		{
			spheresDist = dist;
			hit = orig + dir * dist;
			N = (hit - spheres[i].center).normalize();
			material = spheres[i].material;
		}
	}

	return spheresDist < 1000;
}

/// <summary>
/// �����ཻ���
/// </summary>
/// <param name="orig"></param>
/// <param name="dir"></param>
/// <param name="sphere"></param>
/// <returns></returns>
Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres)
{
	Vec3f point, N;
	Material material;
	if (!SceneIntersect(orig, dir, spheres, point, N, material))
		return Vec3f(0.2, 0.7, 0.8);

	return material.diffuseColor;
}

void Render(const vector<Sphere>& spheres) {
	const int width = 1024;
	const int height = 768;

	//��ǰfovȡ�������Բ��
	const int fov = M_PI / 2.0;
	std::vector<Vec3f> framebuffer(width * height);

#pragma omp parallel for
	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++) {
			//��Ļ���� = 2 * tan��fov / 2�� * ���������Ļ�ľ���;
			//����Ļ���ص�ת��Ϊ����
			//���������Ļ����
			float cameraToScreenDistance = 1;
			//�ݺ��
			float whRatio = width / (float)height;
			//�����Զ�����泤�ȵ�һ��
			float halfCameranFarPlaneWidth = tan(fov / 2.0) * cameraToScreenDistance;
			//����Ļ������Ϊx���ĵ�ǰ���ص���������
			float screenWidthWorldPosX = (i + 0.5) * (2.0 / (float)width) * halfCameranFarPlaneWidth - halfCameranFarPlaneWidth;
			//��ǰ����xֵ����Ļ����λ��
			float x = screenWidthWorldPosX * whRatio;
			//��ǰ����yֵ����Ļ����λ��,�������ΪʲôҪȡ��ֵ��
			float y = -((j + 0.5) * 2.0 / (float)height) * halfCameranFarPlaneWidth + halfCameranFarPlaneWidth;

			Vec3f dir = Vec3f(x, y, -1).normalize();

			//��ԭ�㷢��һ�����ߣ��ж��Ƿ���Բ�ཻ������׷��
			framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, spheres);
		}
	}

	std::ofstream ofs; // save the framebuffer to file
	ofs.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < height * width; ++i) {
		for (size_t j = 0; j < 3; j++) {
			//framebuffer[i]���ص���һ��Vec3f
			ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
		}
	}
	ofs.close();
}

void RenderStart()
{
	Material ivory(Vec3f(0.4, 0.4, 0.3));
	Material redRubber(Vec3f(0.3, 0.1, 0.1));

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, redRubber));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, redRubber));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, ivory));

	Render(spheres);
}
