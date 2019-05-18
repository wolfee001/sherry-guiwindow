#pragma once

#include <string>
#include "IWindow.h"

namespace sherry::gui::window
{
	class WindowManager
	{
	public:
		static IWindow* Create(const std::string& title, int width, int height);
		static bool Update();
		static void Destroy(const IWindow* window);
	};
}
