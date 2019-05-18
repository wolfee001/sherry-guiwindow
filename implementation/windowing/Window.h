#pragma once

#include <string>
#include <functional>

#include <SDL.h>

#include "../imgui/ImGuiGLConnector.h"
#include "../imgui/ImGuiSDLConnector.h"

#include "../../interface/IWindow.h"

namespace sherry::gui::window
{
	class Window : public IWindow
	{
	public:
		Window(const std::string& title, SDL_Window* window, SDL_GLContext glcontext);
		~Window();

		virtual uint32_t GetWindowId() override;
		virtual void HandleEvent(const void* event) override;
		virtual void AddRender(const std::function<void()>& gui) override;
		virtual void Render() override;
		virtual std::pair<int, int> GetWindowSize() override;

	private:
		SDL_Window* mWindow;
		SDL_GLContext mGLcontext;

		ImGuiGLConnector mGLConnector;
		ImGuiSDLConnector mSDLConnector;
		ImGuiContext* mContext = nullptr;

		uint32_t mWindowId = 0;
		std::vector<std::function<void()>> mGuis;
	};
}
