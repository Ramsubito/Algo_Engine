#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew/include/glew.h"
#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	void CubeDirectMode(float x, float y, float z, float size);
	void CubeVertexArrays(float x, float y, float z, float size);
	void CubeVertexArraysWithIndices(float x, float y, float z, float size);
	bool CleanUp();

	void OnResize(int width, int height);

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};