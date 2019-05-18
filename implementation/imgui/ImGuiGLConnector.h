#pragma once

#include "../glinclude.h"
#include <imgui.h>

#ifndef IMGUI_IMPL_OPENGL_LOADER_GLEW
	#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#endif

namespace sherry::gui::window
{
	class ImGuiGLConnector
	{
	public:
		bool ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
		void ImGui_ImplOpenGL3_Shutdown();
		void ImGui_ImplOpenGL3_NewFrame();
		void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

		bool ImGui_ImplOpenGL3_CreateFontsTexture();
		void ImGui_ImplOpenGL3_DestroyFontsTexture();
		bool ImGui_ImplOpenGL3_CreateDeviceObjects();
		void ImGui_ImplOpenGL3_DestroyDeviceObjects();
		bool CheckProgram(GLuint handle, const char* desc);
		bool CheckShader(GLuint handle, const char* desc);

	private:
		char ImGuiGLConnector::mGlslVersionString[32] = "";
		GLuint ImGuiGLConnector::mFontTexture = 0;
		GLuint ImGuiGLConnector::mShaderHandle = 0;
		GLuint ImGuiGLConnector::mVertHandle = 0;
		GLuint ImGuiGLConnector::mFragHandle = 0;
		int ImGuiGLConnector::mAttribLocationTex = 0;
		int ImGuiGLConnector::mAttribLocationProjMtx = 0;
		int ImGuiGLConnector::mAttribLocationVtxPos = 0;
		int ImGuiGLConnector::mAttribLocationVtxUV = 0;
		int ImGuiGLConnector::mAttribLocationVtxColor = 0;
		unsigned int ImGuiGLConnector::mVboHandle = 0;
		unsigned int ImGuiGLConnector::mElementsHandle = 0;
	};
}
