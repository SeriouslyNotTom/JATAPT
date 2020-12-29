#pragma once

#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Debug_Metrics
{
public:
	const char* Monitor_Name = "666";
	int Monitor_Refresh_Rate = 666;
	float Monitor_Scale = 666;
	const char* GPU_NAME = "666";
	const char* GLFW_NAME = "666";
	const char* OPENGL_VER = "666";
	//void GLFW3_DONE(Window* window);

	int getMemoryUsage();
};