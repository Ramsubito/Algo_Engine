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
		ImGui::Text("Hello from configuration!");
		if (ImGui::Button("Close configuration"))
			show_configuration = false;
		ImGui::End();
	}
	if (show_about)
	{
		ImGui::Begin("About Algo Engine", &show_about);
		ImGui::Text("Version 0.1-alpha");
		ImGui::Separator();
		ImGui::Text("By Marc Pavon and Marc Ramos for study purposes.");
		ImGui::Text("Algo Engine is licensed under the Public Domain, see LICENSE for more informaton.");
		ImGui::Text(" ");
		//LIBRARIES USED(To update versions)
		ImGui::Text("3rd Party Libraries used:");
		ImGui::BulletText("SDL 2.0.6");
		ImGui::BulletText("SDL Mixer 2.0.0");
		ImGui::BulletText("ImGui 1.51");
		ImGui::BulletText("MathGeoLib 1.5");
		ImGui::BulletText("OpenGL 3.1");
		ImGui::BulletText("Glew 2.0.0");
		//MIT LICENSE
		ImGui::Text(" ");
		ImGui::Text("License:");
		ImGui::Text(" ");
		ImGui::Text("MIT License");
		ImGui::Text(" ");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\n"
			"of this software and associated documentation files (the 'Software'), to deal\n"
			"in the Software without restriction, including without limitation the rights\n"
			"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
			"copies of the Software, and to permit persons to whom the Software is\n"
			"furnished to do so, subject to the following conditions:\n");
		ImGui::Text(" ");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\n"
			"copies or substantial portions of the Software.\n");
		ImGui::Text(" ");
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
			"SOFTWARE.");


		;


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
