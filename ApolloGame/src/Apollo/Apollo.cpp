#include <Apollo/Window.h>
#include <Apollo/logger/Logger.h>

int main()
{
	{
		Apollo::Window window;

		if (window.createWindow() != APOLLO_NO_ERROR)
		{
			return -1;
		}

		while (!window.shouldClose())
		{
			window.updateWindow();
			window.swapBuffers();
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.3f, 0.9f, 0.0f, 1.0f);
		}
	}

#if APOLLO_WIN32
	system("pause");
#endif
}