#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class Cull {
public:
	float cull_near;
	float cull_far;

	mat4 vp;
	std::vector<vec4> frustumPlanes;

	Cull(const mat4& viewMat, const mat4& projectMat)
	{
		cull_near = projectMat[3][2] / (projectMat[2][2] - 1);
		cull_far = projectMat[3][2] / (projectMat[2][2] + 1);
		vp = projectMat * viewMat;
		frustumPlanes = setFrustumPlanes(vp);
	}

	//йсв╤лчЁЩ
	bool frustumCull(const vec4& v1, const vec4& v2, const vec4& v3);

	//йс©злчЁЩ
	bool clipSpaceCull(const vec4& v1, const vec4& v2, const vec4& v3);

	//цФлчЁЩ
	bool faceCull(const vec4& v1, const vec4& v2, const vec4& v3);

	std::vector<vec4> setFrustumPlanes(const mat4& vp);
	
};



