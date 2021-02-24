#include <iostream>
#include "screen.h"

using namespace std;

int main()
{
	Screen screen(1024, 768);
	screen.SetFrameBuffer();
	screen.Render();

	return 0;
}