#pragma once

#include <cstdint>
#include <imgui.h>

struct SDL_Window;
typedef union SDL_Event SDL_Event;
struct SDL_Cursor;

namespace sherry::gui::window
{
	class ImGuiSDLConnector
	{
	public:
		bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window);
		void ImGui_ImplSDL2_Shutdown();
		void ImGui_ImplSDL2_NewFrame();
		bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
		void ImGui_ImplSDL2_UpdateMouseCursor();
		void ImGui_ImplSDL2_UpdateMousePosAndButtons();
		bool ImGui_ImplSDL2_Init(SDL_Window* window);

	private:
		SDL_Window* mWindow = NULL;
		uint64_t mTime = 0;
		bool mMousePressed[3] = { false, false, false };
		SDL_Cursor* mMouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
	};
}
