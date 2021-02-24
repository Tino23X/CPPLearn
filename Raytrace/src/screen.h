#pragma once
#include "../../src/RayTrace/geometry.h"
#include <fstream>
#include "Sphere.h"
using namespace std;

class Screen
{
public:
	Screen(int w, int h) : width(w), height(h), frameBuffer(w* h) {}
	void Render() {

		ofstream ofs; // save the framebuffer to file
		ofs.open("./out.ppm", ofstream::out | ofstream::binary);
		ofs << "P6\n" << width << " " << height << "\n255\n";
		for (size_t i = 0; i < height * width; ++i) {
			for (size_t j = 0; j < 3; j++) {
				//framebuffer[i]返回的是一个Vec3f
				ofs << (char)(255 * max(0.f, min(1.f, frameBuffer[i][j])));
			}
		}
		ofs.close();
	}

	void SetFrameBuffer() {

		const int fov = M_PI / 2.0f;
#pragma omp parallel for
		for (size_t j = 0; j < height; ++j)
		{
			for (size_t i = 0; i < width; ++i)
			{
				float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.0) * width / (float)height;
			}
		}
	}

	Vec3f CastRay(const Vec3f& orig, const Vec3f& dir, const Sphere& sphere)
	{
		float sphereDist = numeric_limits<float>::max();
		if (!sphere.RayIntersect(orig, dir, sphereDist))
			return Vec3f(0.2, 0.7, 0.8);

		return Vec3f(0.4, 0.4, 0.3);

	}

	vector<Vec3f>& GetFrameBuffer() { return frameBuffer; }


private:
	vector<Vec3f> frameBuffer;
	int width;
	int height;

};
