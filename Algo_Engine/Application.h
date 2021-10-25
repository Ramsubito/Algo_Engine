#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

#include <vector>
#include <list>
#include <stdio.h>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;

private:

	Timer	ms_timer;
	float	dt = 0;
	float last_FPS = 0.0f;
	float last_ms = 0.0f;
	std::vector<Module*> list_modules;

	int AsignVec = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetMS();
	float GetFPS();

	std::vector<float> fpsLog;
	std::vector<float> msLog;
	int maxFPS = 60;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;