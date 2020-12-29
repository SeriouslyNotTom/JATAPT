#pragma once

#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/Debug/Log.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <unordered_map>
#include <functional>
#include <vector>

namespace Alcubierre
{
	namespace Engine
	{

		namespace Window
		{
			class WindowInstance
			{
			public:
				GLFWmonitor* glfw_monitor;
				GLFWwindow* glfw_window;
				GLFWwindow* glfw_share_window;
				int window_width, window_height;
				char* window_title;
				void CenterWindow();
				void CenterWindow(GLFWmonitor* monitor);
			};

			const typedef std::function<void(WindowInstance*)> WindowCreationCallback;
			static WindowInstance* createWindow(WindowCreationCallback* window_cb);
		}

		void Initialize();
		extern Window::WindowInstance* PrimaryWindow;
		void SpawnWindow(Window::WindowCreationCallback* NewWindowCallback);
		void SetupContext();

	}

	namespace Renderer
	{

		class IRenderable
		{
		public:
			virtual bool PreRender() { return false; };
			virtual bool OnRender() { return false; };
			virtual bool PostRender() { return false; };
		};

		class RenderQueueOBJ
		{
		public:
			IRenderable* IOBJ;
			std::vector<IRenderable*> PreRenderHook;
		};

		extern std::unordered_map<char*, RenderQueueOBJ*> *RenderQueue;
		extern std::vector<IRenderable*>* RenderPointerQueue;
		static bool ListDirty;

		bool Initialize();
		void RebuildList();
		void AddToQueue(IRenderable RenderableOBJ);
		void AddPreRenderHook(char* RendererName, RenderQueueOBJ* HookOBJ);
		void AddPreRenderHook(RenderQueueOBJ* Renderer, RenderQueueOBJ* HookOBJ);
	}

}