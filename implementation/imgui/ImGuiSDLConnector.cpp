#include "ImGuiSDLConnector.h"

#include <imgui.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

namespace sherry::gui::window
{
	static char* g_ClipboardTextData = NULL;

	static const char* ImGui_ImplSDL2_GetClipboardText(void*)
	{
		if (g_ClipboardTextData)
			SDL_free(g_ClipboardTextData);
		g_ClipboardTextData = SDL_GetClipboardText();
		return g_ClipboardTextData;
	}

	static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
	{
		SDL_SetClipboardText(text);
	}

	bool ImGuiSDLConnector::ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (event->type)
		{
		case SDL_MOUSEWHEEL:
		{
			if (event->wheel.x > 0) io.MouseWheelH += 1;
			if (event->wheel.x < 0) io.MouseWheelH -= 1;
			if (event->wheel.y > 0) io.MouseWheel += 1;
			if (event->wheel.y < 0) io.MouseWheel -= 1;
			return true;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event->button.button == SDL_BUTTON_LEFT) mMousePressed[0] = true;
			if (event->button.button == SDL_BUTTON_RIGHT) mMousePressed[1] = true;
			if (event->button.button == SDL_BUTTON_MIDDLE) mMousePressed[2] = true;
			return true;
		}
		case SDL_TEXTINPUT:
		{
			io.AddInputCharactersUTF8(event->text.text);
			return true;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			int key = event->key.keysym.scancode;
			IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[key] = (event->type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			return true;
		}
		}
		return false;
	}

	bool ImGuiSDLConnector::ImGui_ImplSDL2_Init(SDL_Window * window)
	{
		mWindow = window;

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendPlatformName = "imgui_impl_sdl";

		io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
		io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
		io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
		io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
		io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
		io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
		io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
		io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

		io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
		io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
		io.ClipboardUserData = NULL;

		mMouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		mMouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
		mMouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
		mMouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
		mMouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
		mMouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
		mMouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		mMouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

#ifdef _WIN32
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);
		io.ImeWindowHandle = wmInfo.info.win.window;
#else
		(void)window;
#endif

		return true;
	}

	bool ImGuiSDLConnector::ImGui_ImplSDL2_InitForOpenGL(SDL_Window * window)
	{
		return ImGui_ImplSDL2_Init(window);
	}

	void ImGuiSDLConnector::ImGui_ImplSDL2_Shutdown()
	{
		mWindow = NULL;

		if (g_ClipboardTextData)
			SDL_free(g_ClipboardTextData);
		g_ClipboardTextData = NULL;

		for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
			SDL_FreeCursor(mMouseCursors[cursor_n]);
		memset(mMouseCursors, 0, sizeof(mMouseCursors));
	}

	void ImGuiSDLConnector::ImGui_ImplSDL2_UpdateMousePosAndButtons()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
		if (io.WantSetMousePos)
			SDL_WarpMouseInWindow(mWindow, (int)io.MousePos.x, (int)io.MousePos.y);
		else
			io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

		int mx, my;
		Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
		io.MouseDown[0] = mMousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[1] = mMousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		io.MouseDown[2] = mMousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
		mMousePressed[0] = mMousePressed[1] = mMousePressed[2] = false;

#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
		SDL_Window * focused_window = SDL_GetKeyboardFocus();
		if (mWindow == focused_window)
		{
			// SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
			// The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
			int wx, wy;
			SDL_GetWindowPosition(focused_window, &wx, &wy);
			SDL_GetGlobalMouseState(&mx, &my);
			mx -= wx;
			my -= wy;
			io.MousePos = ImVec2((float)mx, (float)my);
		}

		// SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
		// The function is only supported from SDL 2.0.4 (released Jan 2016)
		bool any_mouse_button_down = ImGui::IsAnyMouseDown();
		SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
		if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_INPUT_FOCUS)
			io.MousePos = ImVec2((float)mx, (float)my);
#endif
	}

	void ImGuiSDLConnector::ImGui_ImplSDL2_UpdateMouseCursor()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
			return;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			SDL_ShowCursor(SDL_FALSE);
		}
		else
		{
			// Show OS mouse cursor
			SDL_SetCursor(mMouseCursors[imgui_cursor] ? mMouseCursors[imgui_cursor] : mMouseCursors[ImGuiMouseCursor_Arrow]);
			SDL_ShowCursor(SDL_TRUE);
		}
	}

	void ImGuiSDLConnector::ImGui_ImplSDL2_NewFrame()
	{
		ImGuiIO& io = ImGui::GetIO();
		IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

		// Setup display size (every frame to accommodate for window resizing)
		int w, h;
		int display_w, display_h;
		SDL_GetWindowSize(mWindow, &w, &h);
		SDL_GL_GetDrawableSize(mWindow, &display_w, &display_h);
		io.DisplaySize = ImVec2((float)w, (float)h);
		if (w > 0 && h > 0)
			io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

		// Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
		static Uint64 frequency = SDL_GetPerformanceFrequency();
		Uint64 current_time = SDL_GetPerformanceCounter();
		io.DeltaTime = mTime > 0 ? (float)((double)(current_time - mTime) / frequency) : (float)(1.0f / 60.0f);
		mTime = current_time;

		ImGui_ImplSDL2_UpdateMousePosAndButtons();
		ImGui_ImplSDL2_UpdateMouseCursor();
	}
}
