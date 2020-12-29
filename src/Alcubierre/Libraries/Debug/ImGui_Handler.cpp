 #include <Alcubierre/Libraries/Debug/ImGui_Handler.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void ImGui_Handler::ImGui_Frame_Start()
{

	for (ImGui_Render* obj : this->ImGuiObjs)
	{
		obj->ImGuiFrameStart();
	}

	for (void(*cbf)() : frame_start_callbacks_)
	{
		cbf();
	}
}

void ImGui_Handler::AcceptWindow(Alcubierre::Engine::Window::WindowInstance* window_)
{
	this->window = window_;
}

void ImGui_Handler::ApplyColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.01f, 0.10f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.71f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.38f, 0.16f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.77f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.77f, 0.26f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.38f, 0.16f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.69f, 0.24f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.77f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.71f, 0.06f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.77f, 0.26f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.77f, 0.26f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.56f, 0.10f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.10f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.77f, 0.26f, 0.98f, 0.0f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.77f, 0.26f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.77f, 0.26f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.46f, 0.18f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.77f, 0.26f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.54f, 0.20f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.07f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.34f, 0.14f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.81f, 0.35f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.64f, 0.00f, 0.90f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.81f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.77f, 0.26f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.71f, 0.00f, 1.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void ImGui_Handler::ApplyStyles()
{
	ImGuiStyle* current_style = &ImGui::GetStyle();
	current_style->FrameRounding = 0;
	current_style->WindowRounding = 0;
	current_style->TabRounding = 0;
	current_style->ScrollbarRounding = 0;
	current_style->WindowBorderSize = 1;
	current_style->ChildBorderSize = 0;
	current_style->FrameBorderSize = 0;
	current_style->PopupBorderSize = 0;
	current_style->WindowPadding = ImVec2(4 * 1, 4 * 1);
}

void ImGui_Handler::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->glfw_window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	//ImGui::GetIO().FontGlobalScale = window_->scaling_factor;
	ImGui_Handler::ApplyColors();
	ImGui_Handler::ApplyStyles();
}

bool ImGui_Handler::OnRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->ImGui_Frame_Start();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

	return true;
}

void ImGui_Handler::AddFrameStart(void(*cbf)())
{
	this->frame_start_callbacks_.push_back(cbf);
}

void ImGui_Handler::AddImGuiRenderableOBJ(ImGui_Render* obj)
{
	this->ImGuiObjs.push_back(obj);
}