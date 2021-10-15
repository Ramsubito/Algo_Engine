#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	if (App->editor->wireframe == true) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//-----Start cube
	//Cube c1(1, 1, 1);
	//c1.InnerRender();

	//c1.CubeDirectModeRender(-2, -2, -2, 1);
	//c1.CubeVertexArraysRender(2, 2, 2, 1); //Por alguna razon no se ve el innerRender si pongo este



	return UPDATE_CONTINUE;
}

