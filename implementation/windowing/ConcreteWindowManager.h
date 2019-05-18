#pragma once

#include <memory>
#include <vector>

#include "../../interface/IWindow.h"

namespace sherry::gui::window
{
	class ConcreteWindowManager
	{
	public:
		static ConcreteWindowManager& GetInstance();
		IWindow* Create(const std::string& title, int width, int height);
		bool Update();
		void Destroy(const IWindow* window);

	private:
		void Init();
		void Teardown();
		ConcreteWindowManager();
		~ConcreteWindowManager();

	private:
		static ConcreteWindowManager* mInstance;
		std::vector<std::unique_ptr<IWindow>> mWindows;
		bool mGlewInited = false;
		bool mInited = false;
	};
}
