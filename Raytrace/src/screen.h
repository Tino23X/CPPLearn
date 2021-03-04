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

			//限制buffer中的最大亮度，
			Vec3f& c = frameBuffer[i];
			float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1)
				c = c * (1. / max);

			for (size_t j = 0; j < 3; j++) {
				//framebuffer[i]返回的是一个Vec3f
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
	/// 场景中的相交检测
	/// </summary>
	/// <param name="orig">射线原点</param>
	/// <param name="dir">射线方向</param>
	/// <param name="spheres">球</param>
	/// <param name="hitPoint"></param>
	/// <param name="hitPointNormal"></param>
	/// <param name="material"></param>
	/// <returns></returns>
	bool SceneIntersect(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, Vec3f& hitPoint, Vec3f& hitPointNormal, Material& material)
	{
		//与圆相交的距离
		float sphereDist = numeric_limits<float>::max();
		for (Sphere sphere : spheres)
		{
			//当前与圆相交的距离
			float tmpDist;
			//如果当前射线与圆相交，且相交距离小于之前的相交距离
			if (sphere.RayIntersect(orig, dir, tmpDist) && tmpDist < sphereDist)
			{
				sphereDist = tmpDist;
				hitPoint = orig + dir * tmpDist;
				hitPointNormal = (hitPoint - sphere.GetCenter()).normalize();
				material = sphere.GetMaterial();
			}
		}

		//检测并绘制一个棋盘格
		float checkerBoardDist = numeric_limits<float>::max();
		//如果方向向量的y轴大于一个阈值，就进行检测，是一种优化
		if (fabs(dir.y) > 0.003f)
		{
			//板子绘制的高度
			float d = -(orig.y - 4) / dir.y; //the checkerboard plane has quiation y = -4;

			Vec3f pt = orig + dir * d;

			//确定板子的长宽
			if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < sphereDist)
			{
				checkerBoardDist = d;
				hitPoint = pt;
				hitPointNormal = Vec3f(0, 1, 0);
				material.GetDiffuseColor() = (int(0.5f * hitPoint.x + 1000) + int(0.5f * hitPoint.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0, 0, 0);
				material.GetDiffuseColor() = material.GetDiffuseColor() * 0.3f;
			}
		}


		return min(sphereDist, checkerBoardDist) < 1000;
	}

	Vec3f CastRay(const Vec3f& orig, const Vec3f& dir, const vector<Sphere>& spheres, const vector<Light>& lights, size_t depth = 0)
	{
		Vec3f hitPoint;
		//光线目标点到Camera的向量
		Vec3f hitPointToCameraNormal;
		Material material;

		if (depth > 4 || !SceneIntersect(orig, dir, spheres, hitPoint, hitPointToCameraNormal, material))
			return Vec3f(0.2, 0.7, 0.8);

		//反射工作,确立反射顶点的时候有疑问
		Vec3f reflectDir = Reflect(dir, hitPointToCameraNormal).normalize();
		Vec3f reflectOrig = reflectDir * hitPointToCameraNormal < 0 ? hitPoint - hitPointToCameraNormal * 1e-3 : hitPoint + hitPointToCameraNormal * 1e-3;
		Vec3f reflectColor = CastRay(reflectOrig, reflectDir, spheres, lights, depth + 1);

		//折射工作
		Vec3f refractDir = Refract(dir, hitPointToCameraNormal, material.GetRefractiveIndes()).normalize();
		Vec3f refractOrig = refractDir * hitPointToCameraNormal < 0 ? hitPoint - hitPointToCameraNormal * 1e-3 : hitPoint + hitPointToCameraNormal * 1e-3;
		Vec3f refractColor = CastRay(refractOrig, refractDir, spheres, lights, depth + 1);

		float diffuseLightIntensity = 0;
		float specularLightIntensity = 0;

		for (auto light : lights)
		{
			//计算光的方向
			Vec3f lightDir = (light.GetPosition() - hitPoint).normalize();

			//计算阴影
			//求出光线到交点的距离
			float lightDistance = (light.GetPosition() - hitPoint).norm();

			//Todo 设置阴影的起点，没有明白
			Vec3f shadowOrig = lightDir * hitPointToCameraNormal < 0 ? hitPoint - hitPointToCameraNormal * 1e-3 : hitPoint + hitPointToCameraNormal * 1e-3;

			//shadow的交点
			Vec3f shadowHitPoint;
			//shadow的表面法线
			Vec3f shadowNormal;

			Material tmpMaterial;
			if (SceneIntersect(shadowOrig, lightDir, spheres, shadowHitPoint, shadowNormal, tmpMaterial) && (shadowHitPoint - shadowOrig).norm() < lightDistance)
				continue;

			diffuseLightIntensity += light.GetIntensity() * max(0.f, lightDir * hitPointToCameraNormal);
			//将目标点反射向量和视线向量做点乘，拿到当前点的夹角（越小值越大）, 然后和高光系数做幂运算。
			specularLightIntensity += pow(max(0.f, -Reflect(-lightDir, hitPointToCameraNormal) * dir), material.GetSpecularExponent()) * light.GetIntensity();
		}

		return material.GetDiffuseColor() * diffuseLightIntensity * material.GetAlbedo()[0] + Vec3f(1, 1, 1) * specularLightIntensity * material.GetAlbedo()[1] + reflectColor * material.GetAlbedo()[2] + refractColor * material.GetAlbedo()[3];
	}

	/// <summary>
	/// Snell‘s loaw 折射方程
	/// </summary>
	/// <param name="I"></param>
	/// <param name="normal"></param>
	/// <param name="refractiveIndex"></param>
	/// <returns></returns>
	Vec3f Refract(const Vec3f& I, const Vec3f& normal, const float& refractiveIndex)
	{
		float cosi = -max(-1.f, min(1.0f, I * normal));
		float etai = 1;
		float etat = refractiveIndex;

		Vec3f n = normal;
		// if the ray is inside the object, swap the indices and invert the normal to get the correct result
		if (cosi < 0)
		{
			cosi = -cosi;
			std::swap(etai, etat);
			n = -normal;
		}

		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);

		return k < 0 ? Vec3f(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));

	}

	/// <summary>
	/// 反射方程
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
