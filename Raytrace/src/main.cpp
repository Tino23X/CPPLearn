#include <iostream>
#include "screen.h"

using namespace std;

int main()
{
	Material ivory(Vec3f(0.4, 0.4, 0.3));
	Material redBubber(Vec3f(0.3, 0.1, 0.1));

	vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2.0f, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2.0f, redBubber));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3.0f, redBubber));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4.0f, ivory));

	Screen screen(1024, 768);
	screen.SetFrameBuffer(sphere);
	screen.Render();

	return 0;
}