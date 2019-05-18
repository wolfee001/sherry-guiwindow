#include "ConcreteWindowManager.h"

#include <SDL.h>
#include <algorithm>
#include <cassert>

#include "Window.h"

namespace sherry::gui::window
{
	/*static*/ ConcreteWindowManager* ConcreteWindowManager::mInstance = nullptr;

	ConcreteWindowManager& ConcreteWindowManager::GetInstance()
	{
		if (!mInstance)
		{
			mInstance = new ConcreteWindowManager();
		}
		return *mInstance;
	}

	IWindow* ConcreteWindowManager::Create(const std::string& title, int width, int height)
	{
		if (mWindows.empty())
		{
			Init();
		}
		SDL_Window* w = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_GLContext c = SDL_GL_CreateContext(w);
		SDL_GL_MakeCurrent(w, c);
		IWindow* newWindow = new Window(title, w, c);
		if (!mGlewInited)
		{
			GLenum err = glewInit();
			if (err)
			{
				assert(false);
			}
			mGlewInited = true;
		}
		mWindows.emplace_back(newWindow);
		return mWindows.back().get();
	}

	bool ConcreteWindowManager::Update()
	{
		std::vector<IWindow*> windowsToDelete;
		windowsToDelete.reserve(mWindows.size());
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			for (std::unique_ptr<IWindow>& window : mWindows)
			{
				if (window->GetWindowId() == event.window.windowID)
				{
					if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
					{
						windowsToDelete.push_back(window.get());
						continue;
					}
					window->HandleEvent((void*)&event);
				}
			}
		}
		for (IWindow* w : windowsToDelete)
		{
			Destroy(w);
		}
		for (std::unique_ptr<IWindow>& window : mWindows)
		{
			window->Render();
		}

		return !mWindows.empty();
	}

	void ConcreteWindowManager::Destroy(const IWindow* window)
	{
		mWindows.erase(std::find_if(mWindows.begin(), mWindows.end(), [&](const std::unique_ptr<IWindow> & w) { return w.get() == window; }));
		if (mWindows.empty())
		{
			Teardown();
		}
	}

	void ConcreteWindowManager::Init()
	{
		if (!mInited)
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
			{
				assert(false);
			}

			// Setup window
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

			mInited = true;
		}
	}

	void ConcreteWindowManager::Teardown()
	{
		if (mInited)
		{
			SDL_Quit();
			mInited = false;
		}
	}

	ConcreteWindowManager::ConcreteWindowManager()
	{
	}

	ConcreteWindowManager::~ConcreteWindowManager()
	{
		Teardown();
	}
}
