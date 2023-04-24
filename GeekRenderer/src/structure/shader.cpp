#include "shader.h"


//顶点着色器
VerToFrag Shader::vertexShader(const Vertex& v)
{
	VerToFrag v2f;
	v2f.worldPos = modelMatrix * v.position;
	v2f.fragPos = projectMatrix * viewMatrix * v2f.worldPos;

	v2f.normal = mat3(transpose(inverse(modelMatrix))) * v.normal;
	v2f.color = v.color;
	v2f.texture = v.texture;
	v2f.Z = 1.0;

	return v2f;
}

//片元着色器
vec4 Shader::fragmentShader(const VerToFrag& v2f)
{
	vec4 color = material->mainTexture->sampler2D(v2f.texture);
	return color;
}

