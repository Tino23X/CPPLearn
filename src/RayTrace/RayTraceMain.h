#pragma once
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"

struct Light
{
	Vec3f position;
	float intensity;
	Light(const Vec3f& p, const float& i) : position(p), intensity(i) {}

};

struct Material
{
	Material(const float& r, const Vec4f& a, const Vec3f& color, const float& spec) :refractiveIndex(r), albedo(a), diffuseColor(color), specularExponent(spec) {}
	Material() :refractiveIndex(1), albedo(1, 0, 0, 0), diffuseColor(), specularExponent() {}
	float refractiveIndex;
	Vec4f albedo;
	Vec3f diffuseColor;
	//高光指数
	float specularExponent;
};


struct Sphere {

	//圆心
	Vec3f center;
	//半径
	float radius;

	Material material;

	Sphere(const Vec3f& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}

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

Vec3f Reflect(const Vec3f& I, const Vec3f& N) {
	return I - N * 2.f * (I * N);
}

Vec3f Reflect(const Vec3f& I, const Vec3f& N, const float& refractiveIndex)
{
	float cosi = -max(-1.f, min(1.f, I * N));
	float etai = 1;
	float etat = refractiveIndex;
	Vec3f n = N;
	if (cosi < 0)
	{
		cosi = -cosi;
		swap(etai, etat);
		n = -N;
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);

	return k < 0 ? Vec3f(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
}

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

	float checkerboardDist = numeric_limits<float>().max();
	if (fabs(dir.y) > 1e-3)
	{
		float d = -(orig.y + 4) / dir.y;

		Vec3f pt = orig + dir * d;

		if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < spheresDist)
		{
			checkerboardDist = d;
			hit = pt;
			N = Vec3f(0, 1, 0);
			material.diffuseColor = (int(0.5 * hit.x + 1000) + int(0.5f * hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, 0.7, 0.3);
			material.diffuseColor = material.diffuseColor * 0.3f;
		}

	}

	return min(spheresDist, checkerboardDist) < 1000;

}

/// <summary>
/// 射线相交检测
/// </summary>
/// <param name="orig"></param>
/// <param name="dir"></param>
/// <param name="sphere"></param>
/// <returns></returns>
Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, const std::vector<Light>& lights, size_t depth = 0)
{

	Vec3f point, N;
	Material material;
	if (depth > 4 || !SceneIntersect(orig, dir, spheres, point, N, material))
		return Vec3f(0.2, 0.7, 0.8);

	Vec3f reflectDir = Reflect(dir, N).normalize();
	Vec3f reflactDir = Reflect(dir, N, material.refractiveIndex).normalize();
	//offset the original point to avoid occlusion by the object itself
	Vec3f reflectOrig = reflectDir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
	Vec3f reflactOrig = reflactDir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;

	Vec3f reflectColor = cast_ray(reflectOrig, reflectDir, spheres, lights, depth + 1);
	Vec3f reflactColor = cast_ray(reflactOrig, reflactDir, spheres, lights, depth + 1);

	float diffuseLightIntensity = 0;
	float specularLIghtIntensity = 0;
	for (size_t i = 0; i < lights.size(); ++i)
	{
		Vec3f lightDir = (lights[i].position - point).normalize();
		float lightDistance = (lights[i].position - point).norm();

		//checking if the point lies in the shadow of the lights[i]
		Vec3f shadowOrig = lightDir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;

		Vec3f shadowPt;
		Vec3f shadowN;
		Material tmpMaterial;
		if (SceneIntersect(shadowOrig, lightDir, spheres, shadowPt, shadowN, tmpMaterial) && (shadowPt - shadowOrig).norm() < lightDistance)
			continue;

		diffuseLightIntensity += lights[i].intensity * max(0.0f, lightDir * N);
		specularLIghtIntensity += powf(max(0.f, -Reflect(-lightDir, N) * dir), material.specularExponent) * lights[i].intensity;
	}

	return material.diffuseColor * diffuseLightIntensity * material.albedo[0] + Vec3f(1.0, 1.0, 1.0) * specularLIghtIntensity * material.albedo[1] + reflectColor * material.albedo[2] + reflactColor * material.albedo[3];
}

void Render(const vector<Sphere>& spheres, const vector<Light>& lights) {
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
			framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, spheres, lights);
		}
	}

	std::ofstream ofs; // save the framebuffer to file
	ofs.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < height * width; ++i) {
		Vec3f& c = framebuffer[i];
		float maxi = max(c[0], max(c[1], c[2]));
		if (maxi > 1) c = c * (1.0 / maxi);
		for (size_t j = 0; j < 3; j++) {
			//framebuffer[i]返回的是一个Vec3f
			ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
		}
	}
	ofs.close();
}

void RenderStart()
{
	Material ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.f);
	Material glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.f);
	Material redRubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.f);
	Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.f);

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, glass));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, redRubber));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, mirror));

	vector<Light> lights;
	lights.push_back(Light(Vec3f(-20, 20, 20), 1.5f));
	lights.push_back(Light(Vec3f(30, 50, -25), 1.8f));
	lights.push_back(Light(Vec3f(30, 20, 30), 1.7f));

	Render(spheres, lights);
}
