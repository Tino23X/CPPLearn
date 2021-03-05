#include <iostream>
#include "tgaimage.h"
#include "model.h"

using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800;

Model* model = NULL;

/// <summary>
///ֱ���㷨,ֱ���������˵���м�����ֵ�����򵥣�����Ч�ʵ�
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
/// �ڶ���ֱ���㷨����ʹ��x	���t����ʹ��t���y,���������⣬��Ϊ��б���Ǹ���ʱ��x�޷�����
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
/// ������ֱ���㷨,���ܿ�������̫����Ҫ�Ż�
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
/// ���İ棬�Ż����
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
/// ����棬���һ���Ż�
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


int main(int argc, char** argv)
{
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