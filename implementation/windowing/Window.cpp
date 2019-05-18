#include "Window.h"

namespace sherry::gui::window
{
	Window::Window(const std::string& title, SDL_Window* window, SDL_GLContext glcontext)
		: mWindow(window)
		, mGLcontext(glcontext)
	{
		mContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(mContext);
		ImGui::StyleColorsDark();
		mSDLConnector.ImGui_ImplSDL2_InitForOpenGL(mWindow);
		mGLConnector.ImGui_ImplOpenGL3_Init("#version 430 core");

		mWindowId = SDL_GetWindowID(mWindow);
	}

	Window::~Window() {
		SDL_GL_MakeCurrent(mWindow, mGLcontext);
		// Cleanup
		ImGui::SetCurrentContext(mContext);
		mGLConnector.ImGui_ImplOpenGL3_Shutdown();
		mSDLConnector.ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(mGLcontext);
		SDL_DestroyWindow(mWindow);
	}

	Uint32 Window::GetWindowId()
	{
		return mWindowId;
	}

	void Window::Render() {
		ImGui::SetCurrentContext(mContext);
		SDL_GL_MakeCurrent(mWindow, mGLcontext);
		ImGuiIO& io = ImGui::GetIO();
		mGLConnector.ImGui_ImplOpenGL3_NewFrame();
		mSDLConnector.ImGui_ImplSDL2_NewFrame();

		ImGui::NewFrame();
		for (auto& gui : mGuis)
		{
			gui();
		}

		ImGui::Render();
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		mGLConnector.ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(mWindow);
	}

	std::pair<int, int> Window::GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(mWindow, &w, &h);
		return std::make_pair(w, h);
	}

	void Window::HandleEvent(const void* event)
	{
		ImGui::SetCurrentContext(mContext);
		mSDLConnector.ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
	}

	void Window::AddRender(const std::function<void()>& gui)
	{
		mGuis.push_back(gui);
	}
}
