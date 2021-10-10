#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool show_console = false;
	bool show_configuration = false;
	bool show_guidemo = false;
	bool show_about = false;
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool window_is_active = true;
	bool file_is_active = true;
	bool input_is_active = true;
	bool hardware_is_active = true;
	bool fullscreen = false;
	bool resiseable = false;
	bool borderless = false;
	bool full_desktop = false;
	int width = 1280;
	int height = 1024;
	int max_width = 1500;
	int max_height = 1500;
	float f_brightness = 1.0f;
	int i_width = 1280;
	int i_height = 1024;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool Start();
	SDL_version compiled;
	char caps;

	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


private:


};