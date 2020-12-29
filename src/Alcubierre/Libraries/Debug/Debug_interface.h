#pragma once

#include <Alcubierre/Libraries/Debug/ImGui_Handler.h>
#include <stdlib.h>
#include <map>
#include <unordered_map>

using namespace std;

class Debug_Menu
{
public:
	virtual void TestMenuStart() {};
	virtual void DebugMenuStart() {};
};

class Debug_Interface : public ImGui_Render
{
private:
	static std::vector<Debug_Menu*> menu_objs;
public:
	static void Render();
	static void init();
	static bool showDemo;
	static bool Test1;
	static unordered_map<string, bool>* Tests;
	static void AddDebugMenuHook(Debug_Menu* obj);
	void ImGuiFrameStart();
};