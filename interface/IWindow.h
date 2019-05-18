#pragma once

#include <functional>

namespace sherry::gui::window
{
	class IWindow
	{
	public:
		virtual uint32_t GetWindowId() = 0;
		virtual void HandleEvent(const void* event) = 0;
		virtual void AddRender(const std::function<void()>& gui) = 0;
		virtual void Render() = 0;
		virtual std::pair<int, int> GetWindowSize() = 0;
	};
}
