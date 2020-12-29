#include <Alcubierre/Libraries/Debug/Debug_interface.h>
#include <imgui.h>
#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/Libraries/Utilities/convert.h>

bool Debug_Interface::showDemo = false;;
bool Debug_Interface::Test1 = false;
std::vector<Debug_Menu*> Debug_Interface::menu_objs = std::vector<Debug_Menu*>();

void Debug_Interface::init()
{
	
}

void Debug_Interface::ImGuiFrameStart()
{
	Render();
}

void Debug_Interface::AddDebugMenuHook(Debug_Menu* new_obj)
{
	menu_objs.push_back(new_obj);
}

void Debug_Interface::Render()
{

	if (ImGui::Begin("Debug Metrics"))
	{
		/*ImGui::Text("Monitor Name: %s", Alcubierre::DebugMetrics.Monitor_Name);
		ImGui::Text("Monitor Refresh Rate: %iHz", Alcubierre::DebugMetrics.Monitor_Refresh_Rate);
		ImGui::Text("Monitor Scale: %f", Alcubierre::DebugMetrics.Monitor_Scale);
		ImGui::Text("GL_RENDERER : %s", Alcubierre::DebugMetrics.GPU_NAME);
		ImGui::Text("GL_VERSION : %s", Alcubierre::DebugMetrics.OPENGL_VER);
		ImGui::Text("Memory Usage : %s", util_convert::format_size(Alcubierre::DebugMetrics.getMemoryUsage()).c_str());
		ImGui::End();*/
	}

	if (showDemo) { ImGui::ShowDemoWindow(&Debug_Interface::showDemo); }
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		ImGui::Text("fuck you buddy");
		ImGui::EndMenu();
	}
	ImGui::Separator();
	ImGui::SetNextItemWidth(100);
	ImGui::Spacing();
	
	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::Checkbox("DearImGui Demo Program", &Debug_Interface::showDemo);
		for (int k=0;k<menu_objs.size();k++)
		{
			menu_objs[k]->DebugMenuStart();
		}
		if (ImGui::BeginMenu("Tests"))
		{
			for (int i=0;i<menu_objs.size();i++)
			{
				menu_objs[i]->TestMenuStart();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}