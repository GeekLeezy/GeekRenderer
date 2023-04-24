#include "renderer.h"
#include <iomanip>

bool insideTriangle(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p)
{


	float r1 = (p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y) * (p2.x - p1.x);
	float r2 = (p.x - p2.x) * (p3.y - p2.y) - (p.y - p2.y) * (p3.x - p2.x);
	float r3 = (p.x - p3.x) * (p1.y - p3.y) - (p.y - p3.y) * (p1.x - p3.x);

	if (r1 >= 0 && r2 >= 0 && r3 >= 0)
		return true;
	return false;
}

void Renderer::render()
{
	for (int i = 0; i < model->objects.size(); i++)
	{
		Object obj = model->objects[i];

		if (obj.mesh.EBO.empty())
			return;

		mat4 m = mat4(1.0f);
		mat4 v = camera->getViewMatrix();		
		mat4 p = camera->getProjectMatrix();

		cull->setFrustumPlanes(p * v);

		for (auto j = 0; j < obj.mesh.EBO.size(); j += 3)
		{
			Vertex v1, v2, v3;
			v1 = obj.mesh.VBO[obj.mesh.EBO[j]];
			v2 = obj.mesh.VBO[obj.mesh.EBO[j + 1]];
			v3 = obj.mesh.VBO[obj.mesh.EBO[j + 2]];

			//视锥剔除
			if (!cull->frustumCull(v1.position, v2.position, v3.position))
				continue;

			//1.顶点着色器
			VerToFrag f1, f2, f3;

			shader->modelMatrix = m;
			shader->viewMatrix = v;
			shader->projectMatrix = p;
			shader->material = &obj.material;

			f1 = shader->vertexShader(v1);
			f2 = shader->vertexShader(v2);
			f3 = shader->vertexShader(v3);

			//片元装配
			//裁剪空间剔除
			if (!cull->clipSpaceCull(f1.fragPos, f2.fragPos, f3.fragPos))
				continue;
			//裁剪空间下的裁剪
			std::vector<VerToFrag> clipVertices = clip->clipSpaceClip(f1, f2, f3);


			//屏幕映射
			//透视除法
			for (int k = 0; k < clipVertices.size(); k++)
				perspectiveDivision(clipVertices[k]);

			int triCount = clipVertices.size() - 3 + 1;
			for (auto k = 0; k < triCount; k++)
			{
				f1 = clipVertices[0];
				f2 = clipVertices[k + 1];
				f3 = clipVertices[k + 2];

				//视口变换
				viewportTrans(f1, f2, f3);

				if (!cull->faceCull(f1.fragPos, f2.fragPos, f3.fragPos))
					continue;

				//光栅化 & 片元着色
				if (renderMod == Fill)
				{
					drawTriangle(f1, f2, f3);
				}
				else
				{
					drawLine(f1, f2);
					drawLine(f2, f3);
					drawLine(f3, f1);
				}

			}

		}
	}
}

void Renderer::perspectiveDivision(VerToFrag& v2f)
{
	v2f.Z = 1.0 / v2f.fragPos.w;
	v2f.fragPos /= v2f.fragPos.w;
	v2f.fragPos.z = (v2f.fragPos.z + 1.0) * 0.5;
}

void Renderer::viewportTrans(VerToFrag& f1, VerToFrag& f2, VerToFrag& f3)
{
	mat4 viewPortMat = mat4(1.0f);
	viewPortMat[0][0] = width / 2.0f;
	viewPortMat[3][0] = width / 2.0f;
	viewPortMat[1][1] = height / 2.0f;
	viewPortMat[3][1] = height / 2.0f;

	f1.fragPos = viewPortMat * f1.fragPos;
	f2.fragPos = viewPortMat * f2.fragPos;
	f3.fragPos = viewPortMat * f3.fragPos;
}

void Renderer::drawLine(const VerToFrag& f1, const VerToFrag& f2)
{


	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	
	int stepX = 0;
	int stepY = 0;

	VerToFrag f = f1;
	vec4 p = f.fragPos;

	
	if (p2.x > p1.x) stepX = 1;
	else stepX = -1;

	if (p2.y > p1.y) stepY = 1;
	else stepY = -1;

	
	int pk = 0;
	bool useX = true;
	int sumStep = 0;

	if (deltaY > deltaX) {
		useX = false;
		sumStep = deltaY;
		std::swap(deltaX, deltaY);
	}
	else {
		useX = true;
		sumStep = deltaX;
	}
	pk = 2 * deltaY - deltaX;

	for (int i = 0; i <= sumStep; i++) {
		frontBuffer->writeColor((int)p.x, (int)p.y, f.color);

		float scale = 0.0f;

		if (pk > 0) {
			if (useX) {
				p.y += stepY;
			}
			else {
				p.x += stepX;
			}

			pk -= 2 * deltaX;
		}

		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		pk += 2 * deltaY;

		if (useX) {
			if (p2.x == p1.x)
				scale = 0.0f;
			else 
				scale = (p.x - p1.x) / (p2.x - p1.x);
		}
		else {
			if (p2.y == p1.y)
				scale = 0.0f;
			else 
				scale = (p.y - p1.y) / (p2.y - p1.y);
		}
		f.color = lerp(f1, f2, scale).color;
	}

}

void Renderer::drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	vec2 p1 = vec2(f1.fragPos);
	vec2 p2 = vec2(f2.fragPos);
	vec2 p3 = vec2(f3.fragPos);

	int max_x = max(p1.x, max(p2.x, p3.x));
	int min_x = min(p1.x, min(p2.x, p3.x));
	int max_y = max(p1.y, max(p2.y, p3.y));
	int min_y = min(p1.y, min(p2.y, p3.y));



	for (auto i = max(0, min_x); i <= min(width, max_x); i++)
	{
		for (auto j = max(0, min_y); j <= min(height, max_y); j++)
		{

			//MSAA(f1, f2, f3, i, j);
			if (insideTriangle(p1, p2, p3, vec2(i + 0.5, j + 0.5)))
			{
				VerToFrag f = barycenterLerp(f1, f2, f3, vec2(i, j));
				
				float depth = frontBuffer->getDepth(i, j);

				if (depth > f.fragPos.z)
				{
					frontBuffer->writeColor(i, j, shader->fragmentShader(f));
					frontBuffer->writeDepth(i, j, f.fragPos.z);
				}
			}
		}
	}
}

void Renderer::changeRenderMode()
{

	renderMod = renderMod == Line ? Fill : Line;
}

void Renderer::clearBuffer()
{
	frontBuffer->clearColorBuffer();
	frontBuffer->clearDepthBuffer();
}