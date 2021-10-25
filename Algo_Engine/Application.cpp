#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);

	msLog.resize(60);
	fpsLog.resize(60);
}

Application::~Application()
{
	std::vector<Module*>* item = &list_modules;

	int i = item->size() - 1;
	while(item != NULL && i >= 0)
	{
		delete item->at(i);
		i--;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::vector<Module*>* item = &list_modules;

	int i = 0;

	while(item != NULL && ret == true && i < item->size())
	{
		ret = item->at(i)->Init();
		i++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	
	i = 0;

	while(item != NULL && ret == true && i < item->size())
	{
		ret = item->at(i)->Start();
		i++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	last_FPS = 1.0f / dt;
	last_ms = (float)ms_timer.Read();

	float maxMsFrame = (1000.0f / maxFPS); // Read 
	float MS_now = ms_timer.Read();

	if (MS_now < maxMsFrame)
		SDL_Delay((Uint32)(maxMsFrame - MS_now)); // Cap framerate
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>* item = &list_modules;
	
	int i = 0;
	while(item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->PreUpdate(dt);
		i++;
	}

	i = 0;

	while(item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->Update(dt);
		i++;
	}

	i = 0;

	while(item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->PostUpdate(dt);
		i++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::vector<Module*>* item = &list_modules;

	int i = item->size() - 1;
	while(item != NULL && ret == true && i >= 0)
	{
		ret = item->at(i)->CleanUp();
		i--;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

float Application::GetMS()
{
	return last_ms;
}

float Application::GetFPS()
{
	return last_FPS;
}