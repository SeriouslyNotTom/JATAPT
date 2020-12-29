#pragma once

#include <Alcubierre/Engine.h>

class ImGui_Render
{
public:
	virtual void ImGuiFrameStart() {};
	virtual void ImGuiFrameEnd() {};
};

class ImGui_Handler : public Alcubierre::Renderer::IRenderable
{
private:
	std::vector<ImGui_Render*> ImGuiObjs;
	Alcubierre::Engine::Window::WindowInstance* window;
	void ImGui_Frame_Start();
	void ImGui_Frame_End();
public:
	void AcceptWindow(Alcubierre::Engine::Window::WindowInstance* window);
	void ApplyColors();
	void ApplyStyles();
	void Init();
	bool OnRender();
	const typedef void (*FrameStartCB)();
	std::vector<void(*)()> frame_start_callbacks_;
	void AddFrameStart(void(*cbf)());
	void AddImGuiRenderableOBJ(ImGui_Render* obj);
};