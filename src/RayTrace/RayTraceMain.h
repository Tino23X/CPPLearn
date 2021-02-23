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

	//圆心
	Vec3f center;
	//半径
	float radius;

	Material material;

	Sphere(const Vec3f& c, const float& r, const Material &m) : center(c), radius(r), material(m) {}

	//圆相交检测
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
/// 射线相交检测
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

	//当前fov取了这个半圆。
	const int fov = M_PI / 2.0;
	std::vector<Vec3f> framebuffer(width * height);

#pragma omp parallel for
	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++) {
			//屏幕长度 = 2 * tan（fov / 2） * 摄像机到屏幕的距离;
			//将屏幕像素点转换为方向
			//摄像机到屏幕距离
			float cameraToScreenDistance = 1;
			//纵横比
			float whRatio = width / (float)height;
			//摄像机远剪裁面长度的一半
			float halfCameranFarPlaneWidth = tan(fov / 2.0) * cameraToScreenDistance;
			//以屏幕中轴线为x起点的当前像素的世界坐标
			float screenWidthWorldPosX = (i + 0.5) * (2.0 / (float)width) * halfCameranFarPlaneWidth - halfCameranFarPlaneWidth;
			//当前像素x值的屏幕比例位置
			float x = screenWidthWorldPosX * whRatio;
			//当前像素y值的屏幕比例位置,还不清楚为什么要取负值。
			float y = -((j + 0.5) * 2.0 / (float)height) * halfCameranFarPlaneWidth + halfCameranFarPlaneWidth;

			Vec3f dir = Vec3f(x, y, -1).normalize();

			//从原点发送一条射线，判断是否与圆相交，光线追踪
			framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, spheres);
		}
	}

	std::ofstream ofs; // save the framebuffer to file
	ofs.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < height * width; ++i) {
		for (size_t j = 0; j < 3; j++) {
			//framebuffer[i]返回的是一个Vec3f
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
