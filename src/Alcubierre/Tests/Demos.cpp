#pragma once
#define GLFW_INCLUDE_NONE
#include <Alcubierre/Tests/TestProgram_handler.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Alcubierre/Alcubierre.h>
#include <imgui.h>
#include <Alcubierre/Libraries/Utilities/lodepng.h>
#include <Alcubierre/Libraries/Utilities/file.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <Alcubierre/Libraries/Utilities/stb_image.h>


//class Test1 : public Test_Program, public ImGui_Render
//{
//private:
//	bool Enabled = false;
//	bool initDone = false;
//
//	float xPos = 0.0;
//	float yPos = 0.0;
//
//	float verts[9] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
//	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//	unsigned int VBO, VAO;
//
//	double timeStart, timeEnd;
//
//	GLint shader, shad_col;
//
//
//	void init()
//	{
//		if (!initDone)
//		{
//
//			shader = Alcubierre::RenderManager.shaders[string("Standard")];
//			glUseProgram(shader);
//			shad_col = glGetUniformLocation(shader, "aColor");
//			glGenBuffers(1, &VBO);
//			glGenVertexArrays(1, &VAO);
//			glBindVertexArray(VAO);
//			glBindBuffer(GL_ARRAY_BUFFER, VBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//			glEnableVertexAttribArray(0);
//		}
//		initDone = true;
//	}
//
//public:
//	char* getTestName() { return "Test 1"; }
//	bool* getEnableStatusPTR() { return &Enabled; }
//
//	void performRender()
//	{
//		if (Enabled)
//		{
//			if (initDone)
//			{
//				glUseProgram(shader);
//				glBindVertexArray(VAO);
//				verts[0] = xPos + -0.5F;
//				verts[1] = yPos + -0.5F;
//				verts[3] = xPos;
//				verts[4] = yPos + 0.5f;
//				verts[6] = xPos + 0.5f;
//				verts[7] = yPos + -0.5f;
//				glUniform4fv(shad_col, 1, color);
//				glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
//				glDrawArrays(GL_TRIANGLES, 0, 3);
//			}
//			else { init(); }
//		}
//	}
//
//	void ImGuiFrameStart()
//	{
//		if (Enabled)
//		{
//			if (ImGui::Begin("[Test1] Debug",&Enabled))
//			{
//				ImGui::SliderFloat("xPos", &xPos, -1, 1);
//				ImGui::SliderFloat("yPos", &yPos, -1, 1);
//
//				ImGui::ColorEdit4("Color", color);
//
//				ImGui::End();
//			}
//		}
//	}
//};
//
//class Test2 : public Test_Program, public ImGui_Render
//{
//private:
//	bool Enabled = false;
//	bool initDone = false;
//
//	float xPos = 0.0;
//	float yPos = 0.0;
//
//	float* verts;
//
//	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//	unsigned int VBO, VAO, EBO;
//	int image_width, image_height, image_channels;
//	GLuint shader, texture, shad_color;
//
//public:
//	unsigned char* data;
//
//	void init()
//	{
//		if (!initDone)
//		{
//			//init
//			glGenTextures(1, &texture);
//			glBindTexture(GL_TEXTURE_2D, texture);
//			glEnable(GL_TEXTURE_2D);
//			
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//			std::string img = asset_path+"thing.png";
//			unsigned char* data = stbi_load(img.c_str(), &image_width, &image_height, &image_channels, STBI_rgb_alpha);
//			if (data)
//			{
//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//			}
//			else
//			{
//				std::cout << "Failed to load texture" << std::endl;
//			}
//			stbi_image_free(data);
//
//			int fwidth, fheight;
//			glfwGetFramebufferSize(my_window->glfw_window, &fwidth, &fheight);
//
//			float n_w, n_h;
//
//			n_w = ((float)2 / fwidth); n_h = ((float)2 / fheight);
//
//			//float w = (float)(n_w * fwidth / 2) - 1;
//
//			float verts[] = {
//				// positions          // texture coords
//				(0.0f+(n_w*(image_width/2))),  (0.0f+(n_h*(image_height/2))), 0.0f,   1.0f, 0.0f, // top right
//				(0.0f+(n_w*(image_width/2))),  (0.0f-(n_h*(image_height/2))), 0.0f,   1.0f, 1.0f, // bottom right
//				(0.0f-(n_w*(image_width/2))),  (0.0f-(n_h*(image_height/2))), 0.0f,   0.0f, 1.0f, // bottom left
//				(0.0f-(n_w*(image_width/2))),  (0.0f+(n_h*(image_height/2))), 0.0f,   0.0f, 0.0f  // top left 
//			};
//
//			unsigned int indices[] = {
//				0, 1, 3, // first triangle
//				1, 2, 3  // second triangle
//			};
//
//			glGenVertexArrays(1, &VAO);
//			glGenBuffers(1, &VBO);
//			glGenBuffers(1, &EBO);
//			glBindVertexArray(VAO);
//			glBindBuffer(GL_ARRAY_BUFFER, VBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//			
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//			glEnableVertexAttribArray(0);
//			// texture coord attribute
//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//			glEnableVertexAttribArray(1);
//
//			shader = Alcubierre::RenderManager.shaders[string("Texture")];
//			shad_color = glGetUniformLocation(shader, "aColor");
//
//		}
//		initDone = true;
//	}
//
//public:
//	char* getTestName() { return "Image test"; }
//	bool* getEnableStatusPTR() { return &Enabled; }
//
//	void performRender()
//	{
//		if (Enabled)
//		{
//			if (initDone)
//			{
//				glBindTexture(GL_TEXTURE_2D, texture);
//				glUseProgram(shader);
//				glUniform4fv(shad_color, 1, color);
//				glBindVertexArray(VAO);
//				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			}
//			else { init(); }
//		}
//	}
//
//	void ImGuiFrameStart()
//	{
//		if (Enabled)
//		{
//			if (ImGui::Begin("Image test"))
//			{
//				ImGui::ColorEdit4("Color", color, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_AlphaBar);
//				ImGui::End();
//			}
//		}
//	}
//};
//
//class Test3 : public Test_Program, public ImGui_Render
//{
//private:
//	bool Enabled = false;
//	bool initDone = false;
//
//	float xPos = 0.0;
//	float yPos = 0.0;
//
//	float* verts;
//
//	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//	unsigned int VBO, VAO, EBO;
//	int image_width, image_height, image_channels;
//	GLuint shader, texture, shad_color;
//
//public:
//	unsigned char* data;
//
//	void init()
//	{
//		if (!initDone)
//		{
//			//init
//			glGenTextures(1, &texture);
//			glBindTexture(GL_TEXTURE_2D, texture);
//			glEnable(GL_TEXTURE_2D);
//
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//			unsigned char* data = stbi_load("thing.png", &image_width, &image_height, &image_channels, STBI_rgb_alpha);
//			if (data)
//			{
//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//			}
//			else
//			{
//				std::cout << "Failed to load texture" << std::endl;
//			}
//			stbi_image_free(data);
//
//			int fwidth, fheight;
//			glfwGetFramebufferSize(my_window->glfw_window, &fwidth, &fheight);
//
//			float n_w, n_h;
//
//			n_w = ((float)2 / fwidth); n_h = ((float)2 / fheight);
//
//			//float w = (float)(n_w * fwidth / 2) - 1;
//
//			float verts[] = {
//				// positions          // texture coords
//				(0.0f + (n_w * (image_width / 2))),  (0.0f + (n_h * (image_height / 2))), 0.0f,   1.0f, 0.0f, // top right
//				(0.0f + (n_w * (image_width / 2))),  (0.0f - (n_h * (image_height / 2))), 0.0f,   1.0f, 1.0f, // bottom right
//				(0.0f - (n_w * (image_width / 2))),  (0.0f - (n_h * (image_height / 2))), 0.0f,   0.0f, 1.0f, // bottom left
//				(0.0f - (n_w * (image_width / 2))),  (0.0f + (n_h * (image_height / 2))), 0.0f,   0.0f, 0.0f  // top left 
//			};
//
//			unsigned int indices[] = {
//				0, 1, 3, // first triangle
//				1, 2, 3  // second triangle
//			};
//
//			glGenVertexArrays(1, &VAO);
//			glGenBuffers(1, &VBO);
//			glGenBuffers(1, &EBO);
//			glBindVertexArray(VAO);
//			glBindBuffer(GL_ARRAY_BUFFER, VBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//			glEnableVertexAttribArray(0);
//			// texture coord attribute
//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//			glEnableVertexAttribArray(1);
//
//			shader = Alcubierre::RenderManager.shaders[string("Texture")];
//			shad_color = glGetUniformLocation(shader, "aColor");
//
//		}
//		initDone = true;
//	}
//
//public:
//	char* getTestName() { return "3d cube test"; }
//	bool* getEnableStatusPTR() { return &Enabled; }
//
//	void performRender()
//	{
//		if (Enabled)
//		{
//			if (initDone)
//			{
//				glBindTexture(GL_TEXTURE_2D, texture);
//				glUseProgram(shader);
//				glUniform4fv(shad_color, 1, color);
//				glBindVertexArray(VAO);
//				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			}
//			else { init(); }
//		}
//	}
//
//	void ImGuiFrameStart()
//	{
//		if (Enabled)
//		{
//			if (ImGui::Begin("3d cube test"))
//			{
//				ImGui::ColorEdit4("Color", color, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_AlphaBar);
//				ImGui::End();
//			}
//		}
//	}
//};
//
//
//class Demos
//{
//	Test1 new_Test1;
//	Test2 new_Test2;
//	Test3 new_Test3;
//public:
//	void LoadTestPrograms()
//	{
//		new_Test1 = Test1();
//		Alcubierre::TestPrograms.AddProgram(&new_Test1);
//		Alcubierre::TestPrograms.AddProgramImGui(&new_Test1);
//
//		new_Test2 = Test2();
//		Alcubierre::TestPrograms.AddProgram(&new_Test2);
//		Alcubierre::TestPrograms.AddProgramImGui(&new_Test2);
//
//		new_Test3 = Test3();
//		Alcubierre::TestPrograms.AddProgram(&new_Test3);
//		Alcubierre::TestPrograms.AddProgramImGui(&new_Test3);
//	}
//};
