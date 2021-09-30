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
		ImGui::Checkbox("Configuration", &show_configuration);
		if (ImGui::Button("Close console"))
			show_console = false;

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
