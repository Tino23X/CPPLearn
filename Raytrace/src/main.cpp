#include <iostream>
#include "screen.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace std;

void BackgroundDrawing()
{
	int w, h, n;

	unsigned char* data = stbi_load("envmap.jpg", &w, &h, &n, 0);
	cout << "W:" << w << "H:" << h << "N:" << n << endl;


	stbi_image_free(data);
}

int main()
{
	BackgroundDrawing();
	return 0;


	Material ivory(1.0, Vec4f(0.6f, 0.3f, 0.1f, 0.0f), Vec3f(0.4, 0.4, 0.3), 50.);
	Material glass(1.5, Vec4f(0.0f, 0.5f, 0.1f, 0.8f), Vec3f(0.6, 0.7, 0.8), 125.);
	Material redBubber(1.0, Vec4f(0.9f, 0.1f, 0.0f, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
	Material mirror(1.0, Vec4f(0.0f, 10.0f, 0.8f, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

	vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2.0f, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2.0f, glass));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3.0f, redBubber));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4.0f, mirror));

	vector<Light> lights;
	//lights.push_back(Light(Vec3f(-20, 20, 20), 1.5f));
	//x +向右，-向左
	//Y +向上，-向下
	//Z +面向摄像机移动， -与摄像机法向量一致
	lights.push_back(Light(Vec3f(-20, 20, 20), 1.5f));
	lights.push_back(Light(Vec3f(30, 50, -25), 1.8f));
	lights.push_back(Light(Vec3f(30, 20, 30), 1.7f));

	Screen screen(1024, 768);
	screen.SetFrameBuffer(spheres, lights);
	screen.Render();

	return 0;
}