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
	float ms = (float)ms_timer.Read();
	ms_timer.Start();
	
if (msLog[59] != 0.0f)
	{
		std::rotate(msLog.begin(), msLog.begin() + 1, msLog.end());
		std::rotate(fpsLog.begin(), fpsLog.begin() + 1, fpsLog.end());
	}
	msLog.at(AsignVec) = ms;
	fpsLog.at(AsignVec) = (int)(1000.0f / ms);
	if (AsignVec < 59) AsignVec++;
	//dt
	dt = ms / 1000.0f;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float ms_max = (1000.0f / maxFPS);
	float ms_now = ms_timer.Read();

	if (ms_now < ms_max)
		SDL_Delay((Uint32)(ms_max - ms_now)); // Delay to cap framerate
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