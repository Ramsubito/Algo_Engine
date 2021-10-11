#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glewInit();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Initialize Blend
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{	Color c = App->camera->background;
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{	
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::CubeDirectMode(float x, float y, float z, float size)
{
	glBegin(GL_TRIANGLES);

	// front face =================
	glVertex3f(x + size, y + size, z + size);    // v0-v1-v2
	glVertex3f(x, y + size, z + size);
	glVertex3f(x, y, z + size);

	glVertex3f(x, y, z + size);    // v2-v3-v0
	glVertex3f(x + size, y, z + size);
	glVertex3f(x + size, y + size, z + size);

	// right face =================
	glVertex3f(x + size, y + size, z + size);    // v0-v3-v4
	glVertex3f(x + size, y, z + size);
	glVertex3f(x + size, y, z);

	glVertex3f(x + size, y, z);    // v4-v5-v0
	glVertex3f(x + size, y + size, z);
	glVertex3f(x + size, y + size, z + size);

	// top face ===================
	glVertex3f(x + size, y + size, z + size);    // v0-v5-v6
	glVertex3f(x + size, y + size, z);
	glVertex3f(x, y + size, z);

	glVertex3f(x, y + size, z);    // v6-v1-v0
	glVertex3f(x, y + size, z + size);
	glVertex3f(x + size, y + size, z + size);

	//left face ===================
	glVertex3f(x, y + size, z + size);    // v1-v6-v7
	glVertex3f(x, y + size, z);
	glVertex3f(x, y, z);

	glVertex3f(x, y, z);    // v7-v2-v1
	glVertex3f(x, y, z + size);
	glVertex3f(x, y + size, z + size);

	//bottom face ===================
	glVertex3f(x, y, z);    // v7-v2-v3
	glVertex3f(x + size, y, z);
	glVertex3f(x + size, y, z + size);

	glVertex3f(x + size, y, z + size);    // v3-v4-v7
	glVertex3f(x, y, z + size);
	glVertex3f(x, y, z);

	//back face ===================
	glVertex3f(x, y, z);    // v7-v4-v5
	glVertex3f(x, y + size, z);
	glVertex3f(x + size, y + size, z);

	glVertex3f(x + size, y + size, z);    // v5-v6-v7
	glVertex3f(x + size, y, z);
	glVertex3f(x, y, z);

	glRotatef(0.1f, 1.0f, 1.0f, 0.0f);

	glEnd();
}

void ModuleRenderer3D::CubeVertexArrays(float x, float y, float z, float size)
{
	float cubeVectors[] = 
	{
	x + size, y + size, z + size,   // v0-v1-v2
	x, y + size, z + size,
	x, y, z + size,

	x, y, z + size,  // v2-v3-v0
	x + size, y, z + size,
	x + size, y + size, z + size,

	// right face =================
	x + size, y + size, z + size,    // v0-v3-v4
	x + size, y, z + size,
	x + size, y, z,

	x + size, y, z,    // v4-v5-v0
	x + size, y + size, z,
	x + size, y + size, z + size,

	// top face ===================
	x + size, y + size, z + size,   // v0-v5-v6
	x + size, y + size, z,
	x, y + size, z,

	x, y + size, z,    // v6-v1-v0
	x, y + size, z + size,
	x + size, y + size, z + size,

	//left face ===================
	x, y + size, z + size,    // v1-v6-v7
	x, y + size, z,
	x, y, z,

	x, y, z,    // v7-v2-v1
	x, y, z + size,
	x, y + size, z + size,

	//bottom face ===================
	x, y, z,   // v7-v2-v3
	x + size, y, z,
	x + size, y, z + size,

	x + size, y, z + size,    // v3-v4-v7
	x, y, z + size,
	x, y, z,

	//back face ===================
	x, y, z,    // v7-v4-v5
	x, y + size, z,
	x + size, y + size, z,

	x + size, y + size, z,    // v5-v6-v7
	x + size, y, z,
	x, y, z
	};

	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, cubeVectors, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// � bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::CubeVertexArraysWithIndices(float x, float y, float z, float size)
{
	float cubeVectors[] =
	{
	x + size, y + size, z + size,   // v0-v1-v2
	x, y + size, z + size,
	x, y, z + size,

	x, y, z + size,  // v2-v3-v0
	x + size, y, z + size,
	x + size, y + size, z + size,

	// right face =================
	x + size, y + size, z + size,    // v0-v3-v4
	x + size, y, z + size,
	x + size, y, z,

	x + size, y, z,    // v4-v5-v0
	x + size, y + size, z,
	x + size, y + size, z + size,

	// top face ===================
	x + size, y + size, z + size,   // v0-v5-v6
	x + size, y + size, z,
	x, y + size, z,

	x, y + size, z,    // v6-v1-v0
	x, y + size, z + size,
	x + size, y + size, z + size,

	//left face ===================
	x, y + size, z + size,    // v1-v6-v7
	x, y + size, z,
	x, y, z,

	x, y, z,    // v7-v2-v1
	x, y, z + size,
	x, y + size, z + size,

	//bottom face ===================
	x, y, z,   // v7-v2-v3
	x + size, y, z,
	x + size, y, z + size,

	x + size, y, z + size,    // v3-v4-v7
	x, y, z + size,
	x, y, z,

	//back face ===================
	x, y, z,    // v7-v4-v5
	x, y + size, z,
	x + size, y + size, z,

	x + size, y + size, z,    // v5-v6-v7
	x + size, y, z,
	x, y, z
	};

	int cubeIndices[] =
	{
		0,1,2
	};

	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, cubeVectors, GL_STATIC_DRAW);

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 8, cubeVectors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, NULL);
}


// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
