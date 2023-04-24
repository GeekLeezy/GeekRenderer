#pragma once
#include "../core/framebuffer.h"
#include "../core/window.h"

#include "../structure/vertex.h"
#include "../structure/v2f.h"

#include "../structure/model.h"
#include "../structure/camera.h"
#include "../structure/shader.h"
#include "cull.h"
#include "clip.h"

#include <glm/glm.hpp>
#include <vector>

enum RenderMode {
	Line,
	Fill
};

class Renderer
{
private:
	int width;
	int height;
	FrameBuffer* frontBuffer;
	Cull* cull;
	Clip* clip;

	RenderMode renderMod;

public:
	Model* model;
	Camera* camera;
	Shader* shader;

	Renderer(const int& w, const int& h, Model* m, Camera* cam, Shader* s,Window* win) : width(w), height(h)
	{
		frontBuffer = new FrameBuffer(win->width, win->height, (unsigned char*)win->bmpBuffer);
		
		model = m;
		camera = cam;
		shader = s;

		cull = new Cull(camera->getViewMatrix(), camera->getProjectMatrix());
		clip = new Clip();

		renderMod = Fill;
	}

	~Renderer() 
	{
		if (frontBuffer)
			delete frontBuffer;
		if (model)
			delete model;
		if (camera)
			delete camera;
		if (shader)
			delete shader;
		if (cull)
			delete cull;
		if (clip)
			delete clip;

		frontBuffer = nullptr;
		model = nullptr;
		camera = nullptr;
		shader = nullptr;
		cull = nullptr;
		clip = nullptr;
	}

	void changeRenderMode();

	void clearBuffer();	

#pragma region pipeLine

	void render();

	void perspectiveDivision(VerToFrag& v2f);

	void viewportTrans(VerToFrag& f1, VerToFrag& f2, VerToFrag& f3);

#pragma region Rasterization
	//void MSAA(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3, const int& x, const int& y);

	void drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);

	void drawLine(const VerToFrag& f1, const VerToFrag& f2);
#pragma endregion Rasterization

#pragma endregion pipeLine
};

