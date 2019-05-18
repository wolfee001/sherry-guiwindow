cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(sherry-guiwindow)

# download conan.cmake to have an easy use of conan in Visual Studio
# see: https://docs.conan.io/en/latest/howtos/vs2017_cmake.html
if(NOT EXISTS "${CMAKE_HOME_DIRECTORY}/conan.cmake")
message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake"
		"${CMAKE_HOME_DIRECTORY}/conan.cmake")
endif()
include(${CMAKE_HOME_DIRECTORY}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt BUILD missing BASIC_SETUP)

add_library(guiwindow STATIC
	implementation/windowing/ConcreteWindowManager.cpp
	implementation/windowing/WindowManager.cpp
	implementation/windowing/Window.cpp
	implementation/imgui/ImGuiGLConnector.cpp
	implementation/imgui/ImGuiSDLConnector.cpp
)
conan_target_link_libraries(guiwindow PUBLIC ${CONAN_LIBS})
