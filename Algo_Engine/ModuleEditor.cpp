#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include <stdio.h>


ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{}

// -----------------------------------------------------------------
bool ModuleEditor::Start()
{
	LOG("Setting up the editor");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	SDL_VERSION(&compiled);
	
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return ret;
}

// -----------------------------------------------------------------
bool ModuleEditor::CleanUp()
{
	LOG("Cleaning editor");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleEditor::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		show_console = !show_console;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		show_configuration = !show_configuration;
	}
	SDL_SetWindowBrightness(App->window->window, f_brightness);

	if (resiseable) SDL_SetWindowSize(App->window->window, i_width, i_height);
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	if (show_console)
	{
		ImGui::Begin("Console", &show_console);
		ImGui::Checkbox("Configuration", &show_configuration);
		if (ImGui::Button("Close console"))
			show_console = false;
		ImGui::End();

	}
	if (show_guidemo)
	{
		ImGui::ShowDemoWindow(&show_guidemo);
	}
	if (show_configuration)
	{
		ImGui::Begin("Configuration", &show_configuration);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Set Defaults")) return UPDATE_STOP; //TODO: Que set defaults load y save cambien/guarden la configuracion
			if (ImGui::MenuItem("Load")) return UPDATE_STOP;
			if (ImGui::MenuItem("Save")) return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			//Show FPS
			ImGui::Text("Engine Name: %s", TITLE);

			static char organization[9] = "UPC CITM";
			ImGui::Text("Organization: %s", organization);

			if (vector_fps.size() != 100)
			{
				vector_fps.push_back(App->GetFPS());
				vector_ms.push_back(App->GetMS());
			}
			else
			{
				vector_fps.erase(vector_fps.begin());
				vector_fps.push_back(App->GetFPS());

				vector_ms.erase(vector_ms.begin());
				vector_ms.push_back(App->GetMS());
			}


			int zero = 0;
			ImGui::SliderInt("Max FPS", &App->maxFPS, 1, 60);
			ImGui::Text("Framerate %.1f", vector_fps[vector_fps.size() - 1]);
			ImGui::PlotHistogram("##framerate", &vector_fps[0], vector_fps.size(), 0, NULL, 0.0f, 100.0f, ImVec2(310, 100));
			ImGui::Text("Milliseconds %.1f", vector_ms[vector_ms.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &vector_ms[0], vector_ms.size(), 0, NULL, 0.0f, 40.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Checkbox("Active", &window_is_active);
			if (window_is_active)
			{
				// TODO: poder cambiar el icono de la app abriendo la carpeta de proyecto desde otra ventana
				ImGui::Text("Icon: ");

				ImGui::SliderFloat("Brightness", &f_brightness, 0.0f, 1.0f, "%.3f");
				SDL_SetWindowBrightness(App->window->window, f_brightness);
				ImGui::SliderInt("Width", &i_width, 1, 3840, "%.d");
				SDL_SetWindowSize(App->window->window, i_width, i_height);
				ImGui::SliderInt("Height", &i_height, 1, 2160, "%.d");
				SDL_SetWindowSize(App->window->window, i_width, i_height);

				//ImGui::Text("Refresh rate: 60"); //TODO: preguntar al profe que es el refresh rate

				if (ImGui::Checkbox("Fullscreen", &fullscreen))
				{
					App->window->SetFullscreen(fullscreen);
					if (full_desktop || resiseable) full_desktop = false, resiseable = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Resiseable", &resiseable))
				{
					App->window->SetResiseable(resiseable);
					if (full_desktop || fullscreen) full_desktop = false, fullscreen = false;
				}
				if (ImGui::Checkbox("Borderless", &borderless))
					App->window->SetBorderless(borderless);
				if (fullscreen || full_desktop) borderless = false;
				ImGui::SameLine();
				if (ImGui::Checkbox("Full Desktop", &full_desktop))
				{
					App->window->SetFullDesktop(full_desktop);
					if (fullscreen || resiseable) fullscreen = false, resiseable = false;;
				}
			}
		}
		if (ImGui::CollapsingHeader("File System"))
		{
			ImGui::Checkbox("Active", &file_is_active);
			if (file_is_active)
			{
				ImGui::Text("Base Path:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", SDL_GetBasePath());
				ImGui::Text("Read Paths:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ".");
				ImGui::Text("Write Path:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ".");
			}
		}
		//------------Input detection
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Checkbox("Active", &input_is_active);
			if (input_is_active)
			{
				int mouse_button = 0;
				
				ImGui::Text("Mouse Position:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d, %d", App->input->GetMouseX(), App->input->GetMouseY());

				ImGui::Text("Mouse Motion:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d, %d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

				ImGui::Text("Mouse Wheel:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->input->GetMouseZ());
				ImGui::Separator();
				ImGui::Text("Mouse Button pressed:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s",mouseButton);
				ImGui::Separator();
				
				if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN) 
				{
					strcpy(mouseButton, "Right button");
				}
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					strcpy(mouseButton, "Left button");
				}
				if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN)
				{
					strcpy(mouseButton, "Middle button");
				}
				
				
			}
		}
		//--------------Hardware
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Checkbox("Active", &hardware_is_active);
			if (hardware_is_active)
			{
				ImGui::Text("SDL Version");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
				ImGui::Separator();
				ImGui::Text("CPUs:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d (%dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
				ImGui::Text("System Ram:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dGb", SDL_GetSystemRAM()/1000);
				ImGui::Text("Caps:");
				ImGui::SameLine();
				if (SDL_Has3DNow()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3DNow");
				ImGui::SameLine();
				if (SDL_HasAltiVec()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiVec");
				ImGui::SameLine();
				if (SDL_HasAVX()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX");
				ImGui::SameLine();
				if (SDL_HasAVX2()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX2"); 
				ImGui::SameLine();
				if (SDL_HasMMX()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX");
				ImGui::SameLine();
				if (SDL_HasRDTSC()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiRDTSC");
				ImGui::SameLine();
				if (SDL_HasSSE()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE");
				ImGui::SameLine();
				if (SDL_HasSSE2()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2");
				ImGui::SameLine();
				if (SDL_HasSSE3()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3");
				ImGui::SameLine();
				if (SDL_HasSSE41()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE41");
				ImGui::SameLine();
				if (SDL_HasSSE42()) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE42");
				ImGui::Separator();
				ImGui::Text("GPU:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));
				ImGui::Text("Brand:");
				ImGui::SameLine();	
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
			}
		}
		ImGui::End();
	}
	if (show_about)
	{
		ImGui::Begin("About Algo Engine", &show_about);
		ImGui::Text("Version 0.1-alpha");
		ImGui::Separator();
		ImGui::Text("By Marc Pavon and Marc Ramos for study purposes.\n" 
			"Algo Engine is licensed under the Public Domain, see LICENSE for more informaton.");
		ImGui::NewLine();
		//LIBRARIES USED(To update versions)
		ImGui::Text("3rd Party Libraries used:");
		ImGui::BulletText("SDL 2.0.6");
		ImGui::BulletText("SDL Mixer 2.0.0");
		ImGui::BulletText("ImGui 1.51");
		ImGui::BulletText("MathGeoLib 1.5");
		ImGui::BulletText("OpenGL 3.1");
		ImGui::BulletText("Glew 2.0.0");
		//MIT LICENSE
		ImGui::NewLine();
		ImGui::Text("License:");
		ImGui::NewLine();
		ImGui::Text("MIT License");
		ImGui::NewLine();
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\n"
			"of this software and associated documentation files (the 'Software'), to deal\n"
			"in the Software without restriction, including without limitation the rights\n"
			"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
			"copies of the Software, and to permit persons to whom the Software is\n"
			"furnished to do so, subject to the following conditions:\n");
		ImGui::NewLine();
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\n"
			"copies or substantial portions of the Software.\n");
		ImGui::NewLine();
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
			"SOFTWARE.");

		ImGui::End();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "esc")) return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Render"))
		{
			if (ImGui::MenuItem("Depth Test", "", depth_test))
			{
				depth_test = !depth_test;
				if (depth_test) glEnable(GL_DEPTH_TEST);
				else glDisable(GL_DEPTH_TEST);
			}
			if (ImGui::MenuItem("Cull Face", "", cull_face))
			{
				cull_face = !cull_face;
				if (cull_face) glEnable(GL_CULL_FACE);
				else glDisable(GL_CULL_FACE);
			}
			if (ImGui::MenuItem("Lighting", "", lighting))
			{
				lighting = !lighting;
				if (lighting) glEnable(GL_LIGHTING);
				else glDisable(GL_LIGHTING);
			}
			if (ImGui::MenuItem("Color Material", "", color_material))
			{
				color_material = !color_material;
				if (color_material) glEnable(GL_COLOR_MATERIAL);
				else glDisable(GL_COLOR_MATERIAL);
			}
			if (ImGui::MenuItem("Textures", "", texture_2d))
			{
				texture_2d = !texture_2d;
				if (depth_test) glEnable(GL_TEXTURE_2D);
				else glDisable(GL_TEXTURE_2D);
			}
			if (ImGui::MenuItem("Wirefrane", "", wireframe))
			{
				//wireframe todo
				wireframe = !wireframe;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1", show_console))
				show_console = !show_console;
			if (ImGui::MenuItem("Configuration", "4", show_configuration))
				show_configuration = !show_configuration;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
				show_guidemo = !show_guidemo;
			if (ImGui::MenuItem("Documendation"))
				ShellExecuteA(NULL, "open", "https://github.com/Ramsubito/Algo_Engine", NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("Download latest"))
				ShellExecuteA(NULL, "open", "https://github.com/Ramsubito/Algo_Engine/releases", NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("Report a bug"))
				ShellExecuteA(NULL, "open", "https://github.com/Ramsubito/Algo_Engine/issues", NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("About"))
				show_about = !show_about;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}
