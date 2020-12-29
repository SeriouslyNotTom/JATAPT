#include <Alcubierre/Tests/TestProgram_handler.h>
#include <imgui.h>
#include <glad/glad.h>

void TestProgram_handler::ImGuiFrameStart()
{
	for(int i=0;i<TestPrograms_ImGui_.size();i++)
	{
		TestPrograms_ImGui_[i]->ImGuiFrameStart();
		TestPrograms_ImGui_[i]->ImGuiFrameEnd();
	}
}

void TestProgram_handler::TestMenuStart()
{
	if (ImGui::BeginMenu("Demo Programs"))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "Placeholder Text");
		for(int i=0;i<TestPrograms_.size();i++)
		{
			ImGui::Checkbox(TestPrograms_[i]->getTestName(), this->EnabledStatus[TestPrograms_[i]]);
			//fprintf(stdout, "DEBUG TESTMENU ITEM : %s\n", prg->getTestName());
		}
		ImGui::EndMenu();
	}
	
}

void TestProgram_handler::Init()
{
	this->EnabledStatus = std::unordered_map<Test_Program*, bool*>();
	//debug here, disapears here
	for(int i=0;i<TestPrograms_.size();i++)
	{
		this->EnabledStatus[TestPrograms_[i]] = TestPrograms_[i]->getEnableStatusPTR();
	}
}

bool TestProgram_handler::OnRender()
{
	for(int i=0;i<TestPrograms_.size();i++)
	{
		if (this->EnabledStatus[TestPrograms_[i]]) { TestPrograms_[i]->performRender(); }
	}
	return true;
}

void TestProgram_handler::AddProgram(Test_Program* program)
{
	this->TestPrograms_.push_back(program);
}

void TestProgram_handler::AddProgramImGui(ImGui_Render* obj)
{
	this->TestPrograms_ImGui_.push_back(obj);
}