#include "../../interface/WindowManager.h"

#include "ConcreteWindowManager.h"

namespace sherry::gui::window
{
	/*static*/ IWindow* WindowManager::Create(const std::string& title, int width, int height)
	{
		return ConcreteWindowManager::GetInstance().Create(title, width, height);
	}

	/*static*/ bool WindowManager::Update()
	{
		return ConcreteWindowManager::GetInstance().Update();
	}

	/*static*/ void WindowManager::Destroy(const IWindow* window)
	{
		ConcreteWindowManager::GetInstance().Destroy(window);
	}
}
