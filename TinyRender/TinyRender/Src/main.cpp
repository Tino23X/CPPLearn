#include <iostream>
#include "tgaimage.h"
#include "model.h"

using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width = 800;
const int height = 800;

Model* model = NULL;

/// <summary>
///直线算法,直接在两个端点的中间做插值，理解简单，但是效率低
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void baseLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	for (float t = 0; t < 1; t += 0.01f)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;

		image.set(x, y, color);
	}
}

/// <summary>
/// 第二版直线算法，先使用x	求出t，再使用t求出y,但是有问题，因为当斜率是负的时候，x无法减少
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void LinePlus(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	for (int x = x0; x <= x1; x++)
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1.0f - t) + y1 * t;
		image.set(x, y, color);
	}
}

/// <summary>
/// 第三版直线算法,性能开销还是太大，需要优化
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void LinePlus2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}

	//make it left - to - right
	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	for (int x = x0; x <= x1; ++x)
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1.0 - t) + y1 * t;
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);
	}
}

/// <summary>
/// 第四版，优化后的
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void LinePlus3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	float derror = abs(dy / float(dx));
	float error = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}

		error += derror;

		if (error > 0.5f)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= 1.0f;
		}
	}
}

/// <summary>
/// 第五版，最后一般优化
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool steep = false;

	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror = abs(dy) * 2;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}

		error += derror;
		if (error > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}
	}
}

void line(Vec2i pointA, Vec2i pointB, TGAImage& image, TGAColor color)
{
	int x0 = pointA.x;
	int y0 = pointA.y;
	int x1 = pointB.x;
	int y1 = pointB.y;

	bool steep = false;

	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror = abs(dy) * 2;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}

		error += derror;
		if (error > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}
	}
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
	if (t0.y == t1.y && t0.y == t2.y)

	//排序找出y值最低的点,由低到高排列
	if (t2.y < t1.y) swap(t1, t2);
	if (t1.y < t0.y) swap(t0, t1);
	if (t2.y < t1.y) swap(t1, t2);

	int totalHight = t2.y - t0.y;

	for (int i = 0; i < totalHight; ++i)
	{
		//三角形是否是上半部分
		bool secondHalf = i > t1.y - t0.y || t1.y == t0.y;
		int segmentHeight = secondHalf ? t2.y - t1.y : t1.y - t0.y;

		float alpha = (float)i / totalHight;
		float beta = (float)(i - (secondHalf ? t1.y - t0.y : 0)) / secondHalf;
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = secondHalf ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x)
			swap(A, B);
		for (int j = A.x; j <= B.x; ++j)
			image.set(j, t0.y + i, color);
	}

}

void triangleOld(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
	if (t0.y == t1.y && t0.y == t2.y)

	//排序找出y值最低的点,由低到高排列
	if (t2.y < t1.y) swap(t1, t2);
	if (t1.y < t0.y) swap(t0, t1);
	if (t2.y < t1.y) swap(t1, t2);

	int totalHight = t2.y - t0.y;
	for (int y = t0.y; y <= t1.y; ++y)
	{
		int segmentHeight = t1.y - t0.y + 1;
		float alpha = (float)(y - t0.y) / totalHight;
		float beta = (float)(y - t0.y) / segmentHeight;

		//不够优化的连续画线用到的点位置
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = t0 + (t1 - t0) * beta;


		if (A.x > B.x)
			swap(A, B);
		for (int x = A.x; x <= B.x; ++x)
		{
			image.set(x, y, color);
		}
	}

	for (int y = t1.y; y <= t2.y; ++y)
	{
		int segmentHeight = t2.y - t1.y + 1;
		float alpha = (float)(y - t0.y) / totalHight;
		float beta = (float)(y - t1.y) / segmentHeight;

		//不够优化的连续画线用到的点位置
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = t1 + (t2 - t1) * beta;

		if (A.x > B.x)
			swap(A, B);
		for (int x = A.x; x <= B.x; ++x)
		{
			image.set(x, y, color);
		}
	}
}


void RenderTriangle()
{
	TGAImage image(width, height, TGAImage::RGB);

	Vec2i t0[3] = { Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically();
	image.write_tga_file("output.tga");
}

int main(int argc, char** argv)
{
	RenderTriangle();
	return 0;



	if (2 == argc)
	{
		model = new Model(argv[1]);
	}
	else
	{
		model = new Model("obj/african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);

	for (int i = 0; i < model->nfaces(); ++i)
	{
		vector<int> face = model->face(i);
		for (int j = 0; j < 3; ++j)
		{
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.0f) * width / 2.0f;
			int y0 = (v0.y + 1.0f) * height / 2.0f;
			int x1 = (v1.x + 1.0) * width / 2.0f;
			int y1 = (v1.y + 1.0) * height / 2.0f;

			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");
	delete model;
	return 0;

}