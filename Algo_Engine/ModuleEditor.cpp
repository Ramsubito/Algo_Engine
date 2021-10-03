#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

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

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

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

	//Mouse position and motion
	mouse_motion_x = mouse_x;
	mouse_motion_y = mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_motion_x = mouse_x - mouse_motion_x;
	mouse_motion_y = mouse_y - mouse_motion_y;

	//Mouse wheel
	mouse_wheel_input = 0;
	while (SDL_PollEvent(&mouse_wheel))
	{
		if (mouse_wheel.type == SDL_MOUSEWHEEL)
		{
			if (mouse_wheel.wheel.y > 0) mouse_wheel_input = 1;
			else if (mouse_wheel.wheel.y < 0) mouse_wheel_input = -1;
		}
	}

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
			ImGui::Text("app");
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
				ImGui::SliderInt("Height", &i_height, 1, 2160, "%.d");

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
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ".");
				ImGui::Text("Read Paths:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ".");
				ImGui::Text("Write Path:");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ".");
			}
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Checkbox("Active", &input_is_active);
			if (input_is_active)
			{
				ImGui::Text("Mouse Position:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d, %d", mouse_x, mouse_y);

				ImGui::Text("Mouse Motion:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d, %d", mouse_motion_x, mouse_motion_y);

				ImGui::Text("Mouse Wheel:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mouse_wheel_input);
				ImGui::Separator();
				ImGui::Text("TODO: poner los imputs de los botones del raton que se pulsan");
			}
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Checkbox("Active", &hardware_is_active);
			if (hardware_is_active)
			{

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
