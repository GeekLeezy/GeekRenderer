#pragma once

#include "v2f.h"
#include "material.h"
#include <glm/glm.hpp>

using namespace glm;
class Shader
{
public:

	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectMatrix;

	Material* material;

	Shader() = default;
	virtual VerToFrag vertexShader(const Vertex& v);

	virtual vec4 fragmentShader(const VerToFrag& v2f);

	~Shader() = default;
};
